#include "../../Public/Combat/EOSMistScout.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Combat/EOSShadowClone.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSMistScout::AEOSMistScout()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 480.0f;
	}

	Tags.AddUnique(TEXT("Enemy"));
	Tags.AddUnique(TEXT("MistScout"));

	// Production 3D visual static mesh for Mist Scout Shinobi
	ScoutStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScoutStaticMesh"));
	ScoutStaticMesh->SetupAttachment(RootComponent);
	ScoutStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	ScoutStaticMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ScoutStaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ScoutStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MeleeMeshRef(
		TEXT("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee"));
	if (MeleeMeshRef.Succeeded())
	{
		ScoutStaticMesh->SetStaticMesh(MeleeMeshRef.Get());
	}

	static ConstructorHelpers::FObjectFinderOptional<UMaterialInterface> EnemyMatRef(
		TEXT("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue"));
	if (EnemyMatRef.Succeeded())
	{
		ScoutStaticMesh->SetMaterial(0, EnemyMatRef.Get());
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	// Defeat smoke Niagara VFX
	static ConstructorHelpers::FObjectFinderOptional<UNiagaraSystem> SmokeFXRef(
		TEXT("/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst"));
	if (SmokeFXRef.Succeeded())
	{
		DefeatSmokeFX = SmokeFXRef.Get();
	}
}

void AEOSMistScout::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	const TCHAR* MeshPath = (ScoutRole == EEOSMistScoutRole::RangedScout)
		? TEXT("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Ranged")
		: TEXT("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee");

	if (USkeletalMesh* LoadedSkelMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, MeshPath)))
	{
		if (GetMesh())
		{
			GetMesh()->SetSkeletalMeshAsset(LoadedSkelMesh);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
			GetMesh()->SetVisibility(true);
			GetMesh()->SetCastShadow(true);
		}
		if (ScoutStaticMesh)
		{
			ScoutStaticMesh->SetVisibility(false);
		}
	}
	else if (ScoutStaticMesh && !ScoutStaticMesh->GetStaticMesh())
	{
		UStaticMesh* LoadedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, MeshPath));
		if (LoadedMesh)
		{
			ScoutStaticMesh->SetStaticMesh(LoadedMesh);
		}
		UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue")));
		if (LoadedMat)
		{
			ScoutStaticMesh->SetMaterial(0, LoadedMat);
		}
	}
}

void AEOSMistScout::SetScoutRole(EEOSMistScoutRole NewRole)
{
	ScoutRole = NewRole;

	const TCHAR* MeshPath = (ScoutRole == EEOSMistScoutRole::RangedScout)
		? TEXT("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Ranged")
		: TEXT("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee");

	if (ScoutRole == EEOSMistScoutRole::RangedScout)
	{
		MaxHealth = 280.0f;
		CurrentHealth = 280.0f;
		BaseAttackDamage = 20.0f;
	}
	else
	{
		MaxHealth = 350.0f;
		CurrentHealth = 350.0f;
		BaseAttackDamage = 25.0f;
	}

	if (USkeletalMesh* LoadedSkelMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, MeshPath)))
	{
		if (GetMesh())
		{
			GetMesh()->SetSkeletalMeshAsset(LoadedSkelMesh);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
			GetMesh()->SetVisibility(true);
			GetMesh()->SetCastShadow(true);
		}
		if (ScoutStaticMesh)
		{
			ScoutStaticMesh->SetVisibility(false);
		}
	}
	else if (ScoutStaticMesh)
	{
		UStaticMesh* LoadedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, MeshPath));
		if (LoadedMesh)
		{
			ScoutStaticMesh->SetStaticMesh(LoadedMesh);
		}
		UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue")));
		if (LoadedMat)
		{
			ScoutStaticMesh->SetMaterial(0, LoadedMat);
		}
	}
}

void AEOSMistScout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDefeated) return;

	// Check if current target is invalid or defeated clone
	if (CombatTarget.IsValid())
	{
		if (AEOSShadowClone* SC = Cast<AEOSShadowClone>(CombatTarget.Get()))
		{
			if (SC->IsPendingKillPending())
			{
				CombatTarget = nullptr;
			}
		}
	}

	// Find target: prefer nearest Shadow Clone (aggro draw) or Player
	if (!CombatTarget.IsValid())
	{
		TArray<AActor*> FoundClones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEOSShadowClone::StaticClass(), FoundClones);

		AActor* BestClone = nullptr;
		float BestDistSq = MAX_FLT;
		for (AActor* CloneActor : FoundClones)
		{
			if (AEOSShadowClone* SC = Cast<AEOSShadowClone>(CloneActor))
			{
				if (SC->CurrentTarget == this)
				{
					BestClone = SC;
					break;
				}
				const float DistSq = FVector::DistSquared(GetActorLocation(), SC->GetActorLocation());
				if (DistSq < BestDistSq)
				{
					BestDistSq = DistSq;
					BestClone = SC;
				}
			}
		}

		if (BestClone)
		{
			CombatTarget = BestClone;
		}
		else
		{
			CombatTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		}
	}

	if (!CombatTarget.IsValid()) return;

	const float DesiredRange = (ScoutRole == EEOSMistScoutRole::RangedScout) ? 650.0f : 180.0f;
	const float Dist = FVector::Dist2D(GetActorLocation(), CombatTarget->GetActorLocation());
	const FVector Direction = (CombatTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

	// Move toward target if outside attack range and not windup telegraphing
	if (!bIsTelegraphing)
	{
		if (Dist > DesiredRange)
		{
			AddMovementInput(Direction, 1.0f);
		}
		else if (bCanAttack)
		{
			StartAttackTelegraph();
		}
	}
}

void AEOSMistScout::StartAttackTelegraph()
{
	if (bIsTelegraphing || !bCanAttack || bIsDefeated) return;

	bIsTelegraphing = true;
	bCanAttack = false;

	const TCHAR* RoleStr = (ScoutRole == EEOSMistScoutRole::RangedScout) ? TEXT("Ranged Water Jutsu") : TEXT("Melee Dual Kunai");
	UE_LOG(LogEOSCombat, Log, TEXT("[MIST SCOUT] %s (%s) begins attack telegraph (%.1fs windup)!"),
		*GetName(), RoleStr, AttackTelegraphDuration);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandle_AttackTelegraph,
			this,
			&AEOSMistScout::ExecuteAttack,
			AttackTelegraphDuration,
			false
		);
	}
}

void AEOSMistScout::ExecuteAttack()
{
	bIsTelegraphing = false;

	if (bIsDefeated || !CombatTarget.IsValid()) return;

	const float DesiredMaxRange = (ScoutRole == EEOSMistScoutRole::RangedScout) ? 750.0f : 240.0f;
	const float Dist = FVector::Dist2D(GetActorLocation(), CombatTarget->GetActorLocation());

	if (Dist <= DesiredMaxRange)
	{
		UE_LOG(LogEOSCombat, Log, TEXT("[MIST SCOUT] %s executed attack dealing %.1f damage to %s!"),
			*GetName(), BaseAttackDamage, *CombatTarget->GetName());

		UGameplayStatics::ApplyDamage(
			CombatTarget.Get(),
			BaseAttackDamage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandle_AttackCooldown,
			this,
			&AEOSMistScout::ResetAttackCooldown,
			AttackCooldown,
			false
		);
	}
}

float AEOSMistScout::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDefeated) return 0.0f;

	CurrentHealth -= DamageAmount;

	// Knockback
	if (DamageCauser)
	{
		const FVector KnockDir = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal2D();
		LaunchCharacter(KnockDir * 280.0f + FVector(0.0f, 0.0f, 120.0f), true, true);

		if (DamageCauser->IsA<AEOSShadowClone>())
		{
			CombatTarget = DamageCauser;
			UE_LOG(LogEOSCombat, Log, TEXT("[MIST SCOUT] %s AGGRO SWITCH: Direct hit from Shadow Clone %s! Now engaging clone."),
				*GetName(), *DamageCauser->GetName());
		}
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[MIST SCOUT] %s took %.1f damage (Current HP: %.1f/%.1f)."),
		*GetName(), DamageAmount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		bIsDefeated = true;
		CurrentHealth = 0.0f;

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TimerHandle_AttackTelegraph);
			World->GetTimerManager().ClearTimer(TimerHandle_AttackCooldown);

			if (DefeatSmokeFX)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					World,
					DefeatSmokeFX,
					GetActorLocation(),
					GetActorRotation()
				);
			}
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogEOSCombat, Log, TEXT("[MIST SCOUT] %s has been DEFEATED!"), *GetName());

		OnScoutDefeated.Broadcast(this);
		SetLifeSpan(1.5f);
	}

	return DamageAmount;
}

