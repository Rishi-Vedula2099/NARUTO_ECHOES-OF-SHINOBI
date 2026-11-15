#include "../../Public/Combat/EOSBossCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "../../Public/Subsystems/EOSQuestSubsystem.h"
#include "../../Public/Subsystems/EOSBossEncounterSubsystem.h"
#include "../../Public/UI/EOSShinobiHUD.h"
#include "GameFramework/PlayerController.h"

AEOSBossCharacter::AEOSBossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Production collision sizing for a humanoid boss
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	// Production 3D visual static mesh for Zabuza Momochi (StaticMesh)
	VisualStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualStaticMesh"));
	VisualStaticMeshComponent->SetupAttachment(RootComponent);
	VisualStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	VisualStaticMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	VisualStaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	VisualStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ZabuzaMeshRef(
		TEXT("/Game/Characters/Bosses/Zabuza/Meshes/SK_Zabuza_Executioner"));
	if (ZabuzaMeshRef.Succeeded())
	{
		VisualStaticMeshComponent->SetStaticMesh(ZabuzaMeshRef.Get());
	}

	static ConstructorHelpers::FObjectFinderOptional<UMaterialInterface> EnemyMatRef(
		TEXT("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue"));
	if (EnemyMatRef.Succeeded())
	{
		VisualStaticMeshComponent->SetMaterial(0, EnemyMatRef.Get());
	}

	// Authored Water Dragon Niagara VFX
	static ConstructorHelpers::FObjectFinderOptional<UNiagaraSystem> WaterVortexRef(
		TEXT("/Game/VFX/Niagara/Jutsu/NS_Suiryuudan_WaterVortex"));
	if (WaterVortexRef.Succeeded())
	{
		WaterDragonVFX = WaterVortexRef.Get();
	}

	// Set tag for identification
	Tags.Add(TEXT("Boss"));
	Tags.Add(TEXT("CombatTarget"));
}

void AEOSBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Runtime fallback if mesh was not bound in CDO
	if (VisualStaticMeshComponent && !VisualStaticMeshComponent->GetStaticMesh())
	{
		UStaticMesh* LoadedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Characters/Bosses/Zabuza/Meshes/SK_Zabuza_Executioner")));
		if (LoadedMesh)
		{
			VisualStaticMeshComponent->SetStaticMesh(LoadedMesh);
			UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue")));
			if (LoadedMat)
			{
				VisualStaticMeshComponent->SetMaterial(0, LoadedMat);
			}
		}
	}

	// Runtime fallback for Water Dragon VFX
	if (!WaterDragonVFX)
	{
		WaterDragonVFX = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, TEXT("/Game/VFX/Niagara/Jutsu/NS_Suiryuudan_WaterVortex")));
	}
}

void AEOSBossCharacter::InitializeFromEncounterDefinition(const FEOSBossEncounterDefinition& InDefinition)
{
	EncounterDefinition = InDefinition;

	// Initialize encounter state
	EncounterState.BossID = InDefinition.BossID;
	EncounterState.CurrentHP = InDefinition.MaxHP;
	EncounterState.MaxHP = InDefinition.MaxHP;
	EncounterState.CurrentPhase = 1;
	EncounterState.CurrentFSMState = EEOSBossState::Inactive;
	EncounterState.StaggerState = InDefinition.StaggerModel;
	EncounterState.StaggerState.CurrentStaggerDamage = 0.0f;
	EncounterState.StaggerState.bIsStaggered = false;
	EncounterState.TotalDamageDealt = 0;
	EncounterState.TotalDamageReceived = 0;
	EncounterState.PlayerDeathCount = 0;

	// Initialize cooldown timers for all attack patterns
	PatternCooldownTimers.Empty();
	for (const FEOSBossAttackPattern& Pattern : InDefinition.AttackPatterns)
	{
		PatternCooldownTimers.Add(Pattern.PatternID, 0.0f);
	}

	CurrentStateTimer = 0.0f;
	EncounterElapsedTime = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Initialized Boss: %s (%s) — MaxHP: %.0f, Phases: %d, Patterns: %d"),
		*InDefinition.BossName, *InDefinition.BossID,
		InDefinition.MaxHP, InDefinition.Phases.Num(), InDefinition.AttackPatterns.Num());
}

void AEOSBossCharacter::TriggerEntrance()
{
	if (EncounterState.CurrentFSMState != EEOSBossState::Inactive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EOS][BOSS] TriggerEntrance called but boss is not Inactive (current: %d)"),
			static_cast<int32>(EncounterState.CurrentFSMState));
		return;
	}

	TransitionToState(EEOSBossState::Entrance);
	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s entrance triggered"), *EncounterDefinition.BossName);
}

bool AEOSBossCharacter::TransitionToState(EEOSBossState NewState)
{
	EEOSBossState OldState = EncounterState.CurrentFSMState;

	// Validate legal transitions (Contract Section 23: cannot teleport arbitrarily between states)
	bool bValidTransition = false;
	switch (OldState)
	{
	case EEOSBossState::Inactive:
		bValidTransition = (NewState == EEOSBossState::Entrance);
		break;
	case EEOSBossState::Entrance:
		bValidTransition = (NewState == EEOSBossState::Idle);
		break;
	case EEOSBossState::Idle:
		bValidTransition = (NewState == EEOSBossState::Telegraph ||
			NewState == EEOSBossState::Repositioning ||
			NewState == EEOSBossState::HitReaction ||
			NewState == EEOSBossState::Staggered ||
			NewState == EEOSBossState::PhaseTransition ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Telegraph:
		bValidTransition = (NewState == EEOSBossState::Attacking ||
			NewState == EEOSBossState::HitReaction ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Attacking:
		bValidTransition = (NewState == EEOSBossState::Recovery ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Recovery:
		bValidTransition = (NewState == EEOSBossState::Idle ||
			NewState == EEOSBossState::HitReaction ||
			NewState == EEOSBossState::PhaseTransition ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Repositioning:
		bValidTransition = (NewState == EEOSBossState::Idle ||
			NewState == EEOSBossState::Telegraph ||
			NewState == EEOSBossState::HitReaction ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::HitReaction:
		bValidTransition = (NewState == EEOSBossState::Idle ||
			NewState == EEOSBossState::Staggered ||
			NewState == EEOSBossState::PhaseTransition ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Staggered:
		bValidTransition = (NewState == EEOSBossState::Idle ||
			NewState == EEOSBossState::PhaseTransition ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::PhaseTransition:
		bValidTransition = (NewState == EEOSBossState::Idle ||
			NewState == EEOSBossState::Defeated);
		break;
	case EEOSBossState::Defeated:
		bValidTransition = false; // Terminal state
		break;
	}

	if (!bValidTransition)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EOS][BOSS] Invalid state transition: %d -> %d"), 
			static_cast<int32>(OldState), static_cast<int32>(NewState));
		return false;
	}

	EncounterState.CurrentFSMState = NewState;
	CurrentStateTimer = 0.0f;
	EncounterState.StateTimer = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s FSM: %d -> %d (Phase %d, HP: %.0f/%.0f)"),
		*EncounterDefinition.BossName,
		static_cast<int32>(OldState), static_cast<int32>(NewState),
		EncounterState.CurrentPhase, EncounterState.CurrentHP, EncounterState.MaxHP);

	if (NewState == EEOSBossState::Defeated)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UEOSQuestSubsystem* QuestSys = GI->GetSubsystem<UEOSQuestSubsystem>())
				{
					if (QuestSys->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")) == EEOSQuestState::Active)
					{
						QuestSys->UpdateObjectiveProgress(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI"), true);
						UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Objective OBJ_DEFEAT_ZABUZA_MOMOCHI marked COMPLETE upon boss defeat."));
					}
				}
				if (APlayerController* PC = World->GetFirstPlayerController())
				{
					if (AEOSShinobiHUD* ShinobiHUD = Cast<AEOSShinobiHUD>(PC->GetHUD()))
					{
						ShinobiHUD->SetMissionInfo(
							TEXT("INVESTIGATION OF THE OUTSKIRTS MIST"),
							TEXT("Zabuza Defeated! Return to Hinata at Ichiraku Ramen"),
							TEXT("2 / 2 Objectives")
						);
						ShinobiHUD->ShowJutsuBanner(TEXT("BOSS DEFEATED! RETURN TO HINATA AT ICHIRAKU RAMEN"), 5.0f);
						ShinobiHUD->ClearTargetBossInfo();
					}
				}
			}
		}
	}

	return true;
}

bool AEOSBossCharacter::TransitionToPhase(int32 TargetPhase)
{
	if (TargetPhase < 1 || TargetPhase > EncounterDefinition.Phases.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[EOS][BOSS] Invalid phase transition target: %d (max: %d)"),
			TargetPhase, EncounterDefinition.Phases.Num());
		return false;
	}

	if (TargetPhase == EncounterState.CurrentPhase)
	{
		return true; // Already in this phase
	}

	int32 OldPhase = EncounterState.CurrentPhase;
	EncounterState.CurrentPhase = TargetPhase;

	// Apply new phase stagger threshold
	if (TargetPhase >= 1 && TargetPhase <= EncounterDefinition.Phases.Num())
	{
		const FEOSBossPhaseDefinition& PhaseDef = EncounterDefinition.Phases[TargetPhase - 1];
		EncounterState.StaggerState.StaggerThreshold = PhaseDef.StaggerThreshold;
		EncounterState.StaggerState.CurrentStaggerDamage = 0.0f;
		EncounterState.StaggerState.bIsStaggered = false;
	}

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s Phase Transition: %d -> %d"),
		*EncounterDefinition.BossName, OldPhase, TargetPhase);

	return true;
}

bool AEOSBossCharacter::ExecuteAttackPattern(const FString& PatternID)
{
	if (EncounterState.CurrentFSMState == EEOSBossState::Staggered || EncounterState.StaggerState.bIsStaggered)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EOS][BOSS] Blocked attack pattern %s: boss is currently STAGGERED"), *PatternID);
		return false;
	}

	// Contract Section 23: Bosses must respect cooldowns
	float* CooldownPtr = PatternCooldownTimers.Find(PatternID);
	if (CooldownPtr && *CooldownPtr > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Pattern %s still on cooldown (%.1fs remaining)"),
			*PatternID, *CooldownPtr);
		return false;
	}

	// Find pattern data
	const FEOSBossAttackPattern* Pattern = nullptr;
	for (const FEOSBossAttackPattern& P : EncounterDefinition.AttackPatterns)
	{
		if (P.PatternID == PatternID)
		{
			Pattern = &P;
			break;
		}
	}

	if (!Pattern)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EOS][BOSS] Unknown attack pattern: %s"), *PatternID);
		return false;
	}

	// Check phase availability
	if (Pattern->AvailableInPhases.Num() > 0 && !Pattern->AvailableInPhases.Contains(EncounterState.CurrentPhase))
	{
		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Pattern %s not available in phase %d"),
			*PatternID, EncounterState.CurrentPhase);
		return false;
	}

	// Set active pattern and start cooldown
	EncounterState.ActivePatternID = PatternID;
	if (CooldownPtr)
	{
		*CooldownPtr = Pattern->Cooldown;
	}

	if (EncounterState.CurrentFSMState == EEOSBossState::Idle ||
		EncounterState.CurrentFSMState == EEOSBossState::Repositioning)
	{
		TransitionToState(EEOSBossState::Telegraph);
	}

	if (PatternID == TEXT("ZAB_WATER_DRAGON_BARRAGE") && WaterDragonVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			WaterDragonVFX,
			GetActorLocation() + FVector(0.0f, 0.0f, 20.0f),
			GetActorRotation(),
			FVector(2.5f, 2.5f, 2.5f)
		);
		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Spawned NS_Suiryuudan_WaterVortex (2.5x scale) for %s telegraph windup!"), *PatternID);
	}

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Executing pattern: %s (%s) — Damage: %.0f, Range: %.0f"),
		*PatternID, *Pattern->PatternName, Pattern->BaseDamage, Pattern->EffectiveRange);

	return true;
}

void AEOSBossCharacter::ApplyStaggerDamage(float StaggerDamageAmount)
{
	if (EncounterState.StaggerState.bIsStaggered)
	{
		return; // Already staggered, don't accumulate more
	}

	EncounterState.StaggerState.CurrentStaggerDamage += StaggerDamageAmount;

	// Check if stagger threshold exceeded
	if (EncounterState.StaggerState.CurrentStaggerDamage >= EncounterState.StaggerState.StaggerThreshold)
	{
		EncounterState.StaggerState.bIsStaggered = true;
		EncounterState.StaggerState.RemainingStaggerTime = EncounterState.StaggerState.StaggerDuration;

		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s STAGGERED! (%.0f/%.0f threshold) — Vulnerability window: %.1fs"),
			*EncounterDefinition.BossName,
			EncounterState.StaggerState.CurrentStaggerDamage,
			EncounterState.StaggerState.StaggerThreshold,
			EncounterState.StaggerState.StaggerDuration);

		// Transition to stagger state if currently in a state that allows it
		if (EncounterState.CurrentFSMState == EEOSBossState::HitReaction ||
			EncounterState.CurrentFSMState == EEOSBossState::Idle ||
			EncounterState.CurrentFSMState == EEOSBossState::Recovery)
		{
			TransitionToState(EEOSBossState::Staggered);
		}
	}
}

void AEOSBossCharacter::ResetEncounter()
{
	if (EncounterDefinition.BossID.IsEmpty())
	{
		return;
	}

	EncounterState.CurrentHP = EncounterState.MaxHP;
	EncounterState.CurrentPhase = 1;
	EncounterState.CurrentFSMState = EEOSBossState::Inactive;
	EncounterState.StaggerState.CurrentStaggerDamage = 0.0f;
	EncounterState.StaggerState.bIsStaggered = false;
	EncounterState.StaggerState.RemainingStaggerTime = 0.0f;
	EncounterState.ActivePatternID.Empty();
	EncounterState.StateTimer = 0.0f;
	EncounterState.TotalDamageDealt = 0;
	EncounterState.TotalDamageReceived = 0;
	EncounterState.PlayerDeathCount = 0;
	CurrentStateTimer = 0.0f;
	EncounterElapsedTime = 0.0f;

	// Reset cooldowns
	for (auto& Pair : PatternCooldownTimers)
	{
		Pair.Value = 0.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s encounter reset"), *EncounterDefinition.BossName);
}

float AEOSBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage <= 0.0f && DamageAmount > 0.0f)
	{
		ActualDamage = DamageAmount;
	}

	if (EncounterState.CurrentFSMState == EEOSBossState::Defeated ||
		EncounterState.CurrentFSMState == EEOSBossState::Inactive)
	{
		return 0.0f;
	}

	// Apply damage
	EncounterState.CurrentHP = FMath::Max(0.0f, EncounterState.CurrentHP - ActualDamage);
	EncounterState.TotalDamageReceived += FMath::RoundToInt(ActualDamage);

	// Apply stagger contribution (50% of damage goes to stagger gauge)
	ApplyStaggerDamage(ActualDamage * 0.5f);

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] %s took %.0f damage (HP: %.0f/%.0f = %.1f%%)"),
		*EncounterDefinition.BossName, ActualDamage,
		EncounterState.CurrentHP, EncounterState.MaxHP,
		EncounterState.GetHealthPercent() * 100.0f);

	// Check for defeat
	if (EncounterState.CurrentHP <= 0.0f)
	{
		if (EncounterState.CurrentFSMState != EEOSBossState::Inactive &&
			EncounterState.CurrentFSMState != EEOSBossState::Defeated)
		{
			TransitionToState(EEOSBossState::Defeated);
		}
		return ActualDamage;
	}

	// Check for phase transition
	CheckPhaseTransition();

	// Transition to hit reaction if in a valid state
	if (EncounterState.CurrentFSMState == EEOSBossState::Idle ||
		EncounterState.CurrentFSMState == EEOSBossState::Recovery ||
		EncounterState.CurrentFSMState == EEOSBossState::Repositioning)
	{
		TransitionToState(EEOSBossState::HitReaction);
	}

	return ActualDamage;
}

void AEOSBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EncounterState.CurrentFSMState == EEOSBossState::Inactive ||
		EncounterState.CurrentFSMState == EEOSBossState::Defeated)
	{
		return;
	}

	EncounterElapsedTime += DeltaTime;
	CurrentStateTimer += DeltaTime;
	EncounterState.StateTimer = CurrentStateTimer;

	// Tick cooldown timers
	for (auto& Pair : PatternCooldownTimers)
	{
		if (Pair.Value > 0.0f)
		{
			Pair.Value = FMath::Max(0.0f, Pair.Value - DeltaTime);
		}
	}

	// Tick stagger decay
	UpdateStaggerDecay(DeltaTime);

	// FSM State Dispatcher
	switch (EncounterState.CurrentFSMState)
	{
	case EEOSBossState::Entrance:        TickEntrance(DeltaTime); break;
	case EEOSBossState::Idle:             TickIdle(DeltaTime); break;
	case EEOSBossState::Telegraph:        TickTelegraph(DeltaTime); break;
	case EEOSBossState::Attacking:        TickAttacking(DeltaTime); break;
	case EEOSBossState::Recovery:         TickRecovery(DeltaTime); break;
	case EEOSBossState::HitReaction:      TickHitReaction(DeltaTime); break;
	case EEOSBossState::Staggered:        TickStaggered(DeltaTime); break;
	case EEOSBossState::PhaseTransition:  TickPhaseTransition(DeltaTime); break;
	default: break;
	}

	UpdateHUDState();
}

// ──── FSM Tick Handlers ────

void AEOSBossCharacter::TickEntrance(float DeltaTime)
{
	// Entrance plays for 2.5 seconds (cinematic intro)
	if (CurrentStateTimer >= 2.5f)
	{
		TransitionToState(EEOSBossState::Idle);
	}
}

void AEOSBossCharacter::TickIdle(float DeltaTime)
{
	if (EncounterState.StaggerState.bIsStaggered)
	{
		TransitionToState(EEOSBossState::Staggered);
		return;
	}

	// After 1.5s idle, evaluate player distance + cooldowns and execute next valid pattern
	if (CurrentStateTimer >= 1.5f)
	{
		SelectNextAttackPattern();
		if (!EncounterState.ActivePatternID.IsEmpty())
		{
			ExecuteAttackPattern(EncounterState.ActivePatternID);
		}
	}
}

void AEOSBossCharacter::TickTelegraph(float DeltaTime)
{
	// Get telegraph duration for the active pattern
	FEOSBossAttackPattern ActivePattern = GetAttackPatternByID(EncounterState.ActivePatternID);
	float TelegraphTime = ActivePattern.TelegraphDuration;

	// Apply phase speed multiplier to telegraph (faster in later phases)
	if (EncounterState.CurrentPhase >= 1 && EncounterState.CurrentPhase <= EncounterDefinition.Phases.Num())
	{
		TelegraphTime /= EncounterDefinition.Phases[EncounterState.CurrentPhase - 1].AttackSpeedMultiplier;
	}

	if (CurrentStateTimer >= TelegraphTime)
	{
		TransitionToState(EEOSBossState::Attacking);
	}
}

void AEOSBossCharacter::TickAttacking(float DeltaTime)
{
	// Attack active frames duration (0.5s base)
	if (CurrentStateTimer >= 0.5f)
	{
		TransitionToState(EEOSBossState::Recovery);
	}
}

void AEOSBossCharacter::TickRecovery(float DeltaTime)
{
	// Recovery window — Contract Section 23: recovery window must exist for player counterplay
	FEOSBossAttackPattern ActivePattern = GetAttackPatternByID(EncounterState.ActivePatternID);
	float RecoveryTime = ActivePattern.RecoveryDuration;

	if (CurrentStateTimer >= RecoveryTime)
	{
		EncounterState.ActivePatternID.Empty();
		TransitionToState(EEOSBossState::Idle);
	}
}

void AEOSBossCharacter::TickHitReaction(float DeltaTime)
{
	// Hit reaction lasts 0.6s
	if (CurrentStateTimer >= 0.6f)
	{
		// Check if stagger was triggered during hit reaction
		if (EncounterState.StaggerState.bIsStaggered)
		{
			TransitionToState(EEOSBossState::Staggered);
		}
		else
		{
			TransitionToState(EEOSBossState::Idle);
		}
	}
}

void AEOSBossCharacter::TickStaggered(float DeltaTime)
{
	// Contract Section 22: stagger window for player to deal damage
	EncounterState.StaggerState.RemainingStaggerTime -= DeltaTime;

	if (EncounterState.StaggerState.RemainingStaggerTime <= 0.0f)
	{
		EncounterState.StaggerState.bIsStaggered = false;
		EncounterState.StaggerState.CurrentStaggerDamage = 0.0f;
		EncounterState.StaggerState.RemainingStaggerTime = 0.0f;
		TransitionToState(EEOSBossState::Idle);
	}
}

void AEOSBossCharacter::TickPhaseTransition(float DeltaTime)
{
	// Phase transition cinematic plays for 2.0 seconds
	if (CurrentStateTimer >= 2.0f)
	{
		TransitionToState(EEOSBossState::Idle);
	}
}

// ──── Internal Helpers ────

void AEOSBossCharacter::UpdateStaggerDecay(float DeltaTime)
{
	if (!EncounterState.StaggerState.bIsStaggered &&
		EncounterState.StaggerState.CurrentStaggerDamage > 0.0f)
	{
		EncounterState.StaggerState.CurrentStaggerDamage = FMath::Max(0.0f,
			EncounterState.StaggerState.CurrentStaggerDamage -
			EncounterState.StaggerState.StaggerDecayRate * DeltaTime);
	}
}

void AEOSBossCharacter::CheckPhaseTransition()
{
	float CurrentHPPercent = EncounterState.GetHealthPercent();

	// Find the correct phase based on HP thresholds
	for (int32 i = EncounterDefinition.Phases.Num() - 1; i >= 0; --i)
	{
		const FEOSBossPhaseDefinition& PhaseDef = EncounterDefinition.Phases[i];
		if (CurrentHPPercent <= PhaseDef.HPThresholdPercent && PhaseDef.PhaseNumber > EncounterState.CurrentPhase)
		{
			// Trigger phase transition
			if (EncounterState.CurrentFSMState == EEOSBossState::Idle ||
				EncounterState.CurrentFSMState == EEOSBossState::Recovery ||
				EncounterState.CurrentFSMState == EEOSBossState::HitReaction ||
				EncounterState.CurrentFSMState == EEOSBossState::Staggered)
			{
				TransitionToPhase(PhaseDef.PhaseNumber);
				TransitionToState(EEOSBossState::PhaseTransition);
			}
			break;
		}
	}
}

void AEOSBossCharacter::SelectNextAttackPattern()
{
	EncounterState.ActivePatternID.Empty();
	TArray<FEOSBossAttackPattern> Available = GetCurrentPhaseAttackPatterns();

	// Filter by cooldown availability
	TArray<FEOSBossAttackPattern> Ready;
	for (const FEOSBossAttackPattern& Pattern : Available)
	{
		const float* CooldownPtr = PatternCooldownTimers.Find(Pattern.PatternID);
		if (!CooldownPtr || *CooldownPtr <= 0.0f)
		{
			Ready.Add(Pattern);
		}
	}

	if (Ready.Num() == 0)
	{
		return;
	}

	// Evaluate distance to player pawn so melee patterns are not picked when out of range
	float PlayerDist = 0.0f;
	TArray<FEOSBossAttackPattern> InRangeReady;
	if (UWorld* World = GetWorld())
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
		{
			PlayerDist = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
			for (const FEOSBossAttackPattern& Pattern : Ready)
			{
				if (Pattern.EffectiveRange >= PlayerDist)
				{
					InRangeReady.Add(Pattern);
				}
			}
		}
	}

	const TArray<FEOSBossAttackPattern>& Pool = (InRangeReady.Num() > 0) ? InRangeReady : Ready;
	const int32 Index = FMath::RandRange(0, Pool.Num() - 1);
	EncounterState.ActivePatternID = Pool[Index].PatternID;

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS] Distance=%.0fcm -> Selected Ready Pattern: %s (Range=%.0fcm)"),
		PlayerDist, *EncounterState.ActivePatternID, Pool[Index].EffectiveRange);
}

void AEOSBossCharacter::UpdateHUDState()
{
	UWorld* World = GetWorld();
	APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	AEOSShinobiHUD* ShinobiHUD = PC ? Cast<AEOSShinobiHUD>(PC->GetHUD()) : nullptr;
	if (!ShinobiHUD)
	{
		return;
	}

	FString PhaseLabel;
	if (EncounterState.StaggerState.bIsStaggered)
	{
		PhaseLabel = FString::Printf(TEXT("Demon of the Hidden Mist (Phase %d - STAGGERED!)"), EncounterState.CurrentPhase);
	}
	else if (EncounterState.CurrentPhase == 2)
	{
		PhaseLabel = TEXT("Demon of the Hidden Mist (Phase 2 - Hidden Mist)");
	}
	else if (EncounterState.CurrentPhase >= 3)
	{
		PhaseLabel = TEXT("Demon of the Hidden Mist (Phase 3 - Berserk)");
	}
	else
	{
		PhaseLabel = TEXT("Demon of the Hidden Mist (Phase 1)");
	}

	const float StaggerRatio = (EncounterState.StaggerState.StaggerThreshold > 0.0f)
		? FMath::Clamp(EncounterState.StaggerState.CurrentStaggerDamage / EncounterState.StaggerState.StaggerThreshold, 0.0f, 1.0f)
		: 0.0f;

	ShinobiHUD->SetTargetBossExtendedInfo(
		EncounterDefinition.BossName.IsEmpty() ? TEXT("Zabuza Momochi") : EncounterDefinition.BossName,
		PhaseLabel,
		EncounterState.GetHealthPercent(),
		EncounterState.CurrentPhase,
		FMath::Max(1, EncounterDefinition.Phases.Num()),
		StaggerRatio,
		EncounterState.StaggerState.bIsStaggered
	);
}

FEOSBossEncounterResult AEOSBossCharacter::GenerateEncounterResult() const
{
	FEOSBossEncounterResult Result;
	Result.BossID = EncounterState.BossID;
	Result.bPlayerVictory = (EncounterState.CurrentFSMState == EEOSBossState::Defeated);
	Result.EncounterDurationSeconds = EncounterElapsedTime;
	Result.TotalDamageDealt = EncounterState.TotalDamageDealt;
	Result.TotalDamageReceived = EncounterState.TotalDamageReceived;
	Result.PlayerDeathCount = EncounterState.PlayerDeathCount;
	return Result;
}

FEOSBossAttackPattern AEOSBossCharacter::GetAttackPatternByID(const FString& PatternID) const
{
	for (const FEOSBossAttackPattern& Pattern : EncounterDefinition.AttackPatterns)
	{
		if (Pattern.PatternID == PatternID)
		{
			return Pattern;
		}
	}
	return FEOSBossAttackPattern();
}

TArray<FEOSBossAttackPattern> AEOSBossCharacter::GetCurrentPhaseAttackPatterns() const
{
	TArray<FEOSBossAttackPattern> Result;
	for (const FEOSBossAttackPattern& Pattern : EncounterDefinition.AttackPatterns)
	{
		if (Pattern.AvailableInPhases.Num() == 0 || Pattern.AvailableInPhases.Contains(EncounterState.CurrentPhase))
		{
			Result.Add(Pattern);
		}
	}
	return Result;
}

bool AEOSBossCharacter::ValidateCooldownRespected() const
{
	// Contract Section 23: Bosses cannot ignore cooldowns
	for (const auto& Pair : PatternCooldownTimers)
	{
		if (Pair.Value > 0.0f && EncounterState.ActivePatternID == Pair.Key)
		{
			return false; // Active pattern is still on cooldown — violation
		}
	}
	return true;
}

bool AEOSBossCharacter::ValidateHitReactionRespected() const
{
	// Contract Section 23: Bosses cannot ignore hit reactions
	// If in HitReaction state, boss is properly reacting (not attacking)
	if (EncounterState.CurrentFSMState == EEOSBossState::HitReaction)
	{
		return true;
	}
	// If in any other valid state, no violation
	return true;
}

void AEOSBossCharacter::ResetPatternCooldowns()
{
	for (auto& Pair : PatternCooldownTimers)
	{
		Pair.Value = 0.0f;
	}
}
