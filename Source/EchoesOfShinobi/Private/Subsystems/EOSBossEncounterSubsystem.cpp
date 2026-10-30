#include "../../Public/Subsystems/EOSBossEncounterSubsystem.h"
#include "../../Public/Combat/EOSBossCharacter.h"
#include "../../Public/Subsystems/EOSAdaptiveAISubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UEOSBossEncounterSubsystem::UEOSBossEncounterSubsystem()
{
}

void UEOSBossEncounterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitializeGoldenBossRegistry();

	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS_SUBSYSTEM] Initialized Boss Encounter Subsystem with %d registered bosses"),
		BossEncounterRegistry.Num());
}

void UEOSBossEncounterSubsystem::Deinitialize()
{
	if (ActiveBossCharacter)
	{
		EndEncounter();
	}
	BossEncounterRegistry.Empty();
	Super::Deinitialize();
}

void UEOSBossEncounterSubsystem::RegisterBossEncounter(const FEOSBossEncounterDefinition& Definition)
{
	BossEncounterRegistry.Add(Definition.BossID, Definition);
	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS_SUBSYSTEM] Registered Boss: %s (%s) — MaxHP: %.0f, Phases: %d"),
		*Definition.BossName, *Definition.BossID, Definition.MaxHP, Definition.Phases.Num());
}

FEOSBossEncounterDefinition UEOSBossEncounterSubsystem::GetBossEncounterDefinition(const FString& BossID) const
{
	const FEOSBossEncounterDefinition* Found = BossEncounterRegistry.Find(BossID);
	if (Found)
	{
		return *Found;
	}
	return FEOSBossEncounterDefinition();
}

bool UEOSBossEncounterSubsystem::IsBossRegistered(const FString& BossID) const
{
	return BossEncounterRegistry.Contains(BossID);
}

TArray<FString> UEOSBossEncounterSubsystem::GetAllRegisteredBossIDs() const
{
	TArray<FString> OutIDs;
	BossEncounterRegistry.GetKeys(OutIDs);
	return OutIDs;
}

void UEOSBossEncounterSubsystem::InitializeGoldenBossRegistry()
{
	// Golden Boss 1: Zabuza Momochi — Demon of the Hidden Mist
	FEOSBossEncounterDefinition ZabuzaDef = BuildZabuzaEncounterDefinition();
	RegisterBossEncounter(ZabuzaDef);
}

FEOSBossEncounterDefinition UEOSBossEncounterSubsystem::BuildZabuzaEncounterDefinition() const
{
	FEOSBossEncounterDefinition Def;
	Def.BossID = TEXT("BOSS_ZABUZA");
	Def.BossName = TEXT("Zabuza Momochi");
	Def.BossSubtitle = TEXT("Demon of the Hidden Mist");
	Def.AITier = EEOSAITier::MajorBoss;
	Def.MaxHP = 4500.0f;
	Def.BaseDefense = 75.0f;
	Def.PrimaryElement = EEOSChakraElement::Water;

	// Stagger Model
	Def.StaggerModel.CurrentStaggerDamage = 0.0f;
	Def.StaggerModel.StaggerThreshold = 250.0f;
	Def.StaggerModel.StaggerDuration = 3.5f;
	Def.StaggerModel.StaggerDecayRate = 18.0f;
	Def.StaggerModel.bIsStaggered = false;
	Def.StaggerModel.RemainingStaggerTime = 0.0f;

	// ──── Phase 1: Silent Killing ────
	FEOSBossPhaseDefinition Phase1;
	Phase1.PhaseNumber = 1;
	Phase1.PhaseName = TEXT("Phase 1: Silent Killing");
	Phase1.HPThresholdPercent = 1.0f;
	Phase1.AttackSpeedMultiplier = 1.0f;
	Phase1.MovementSpeedMultiplier = 1.0f;
	Phase1.DamageMultiplier = 1.0f;
	Phase1.StaggerThreshold = 250.0f;
	Phase1.AvailablePatternIDs = {
		TEXT("ZAB_KUBIKIRIBOCHO_SLASH"),
		TEXT("ZAB_WATER_CLONE_FEINT"),
		TEXT("ZAB_MIST_AMBUSH")
	};
	Def.Phases.Add(Phase1);

	// ──── Phase 2: Hidden Mist & Water Prison ────
	FEOSBossPhaseDefinition Phase2;
	Phase2.PhaseNumber = 2;
	Phase2.PhaseName = TEXT("Phase 2: Hidden Mist & Water Prison");
	Phase2.HPThresholdPercent = 0.65f;
	Phase2.AttackSpeedMultiplier = 1.25f;
	Phase2.MovementSpeedMultiplier = 1.2f;
	Phase2.DamageMultiplier = 1.2f;
	Phase2.StaggerThreshold = 350.0f;
	Phase2.AvailablePatternIDs = {
		TEXT("ZAB_KUBIKIRIBOCHO_SLASH"),
		TEXT("ZAB_WATER_CLONE_FEINT"),
		TEXT("ZAB_MIST_AMBUSH"),
		TEXT("ZAB_WATER_PRISON_TRAP"),
		TEXT("ZAB_WATER_DRAGON_BARRAGE")
	};
	Def.Phases.Add(Phase2);

	// ──── Phase 3: Demon's Final Stand ────
	FEOSBossPhaseDefinition Phase3;
	Phase3.PhaseNumber = 3;
	Phase3.PhaseName = TEXT("Phase 3: Demon's Final Stand");
	Phase3.HPThresholdPercent = 0.30f;
	Phase3.AttackSpeedMultiplier = 1.45f;
	Phase3.MovementSpeedMultiplier = 1.35f;
	Phase3.DamageMultiplier = 1.4f;
	Phase3.StaggerThreshold = 450.0f;
	Phase3.AvailablePatternIDs = {
		TEXT("ZAB_KUBIKIRIBOCHO_SLASH"),
		TEXT("ZAB_DEMON_BERSERK_COMBO"),
		TEXT("ZAB_WATER_DRAGON_BARRAGE"),
		TEXT("ZAB_HIDDEN_MIST_EXECUTOR")
	};
	Def.Phases.Add(Phase3);

	// ──── Attack Patterns ────

	// 1. Kubikiribocho Executioner's Slash
	FEOSBossAttackPattern Slash;
	Slash.PatternID = TEXT("ZAB_KUBIKIRIBOCHO_SLASH");
	Slash.PatternName = TEXT("Kubikiribocho Cleave");
	Slash.AttackType = EEOSBossAttackType::MeleeSwing;
	Slash.TelegraphType = EEOSBossTelegraphType::WindUp;
	Slash.TelegraphDuration = 0.6f;
	Slash.BaseDamage = 75.0f;
	Slash.EffectiveRange = 240.0f;
	Slash.Cooldown = 2.0f;
	Slash.RecoveryDuration = 0.9f;
	Slash.bCanBeBlocked = true;
	Slash.bCanBeDodged = true;
	Slash.Element = EEOSChakraElement::Water;
	Slash.AvailableInPhases = { 1, 2, 3 };
	Def.AttackPatterns.Add(Slash);

	// 2. Water Clone Feint
	FEOSBossAttackPattern CloneFeint;
	CloneFeint.PatternID = TEXT("ZAB_WATER_CLONE_FEINT");
	CloneFeint.PatternName = TEXT("Water Clone Feint Strike");
	CloneFeint.AttackType = EEOSBossAttackType::MeleeCombo;
	CloneFeint.TelegraphType = EEOSBossTelegraphType::PostureTell;
	CloneFeint.TelegraphDuration = 0.7f;
	CloneFeint.BaseDamage = 60.0f;
	CloneFeint.EffectiveRange = 300.0f;
	CloneFeint.Cooldown = 4.5f;
	CloneFeint.RecoveryDuration = 1.1f;
	CloneFeint.bCanBeBlocked = true;
	CloneFeint.bCanBeDodged = true;
	CloneFeint.Element = EEOSChakraElement::Water;
	CloneFeint.AvailableInPhases = { 1, 2 };
	Def.AttackPatterns.Add(CloneFeint);

	// 3. Silent Killing Mist Ambush
	FEOSBossAttackPattern MistAmbush;
	MistAmbush.PatternID = TEXT("ZAB_MIST_AMBUSH");
	MistAmbush.PatternName = TEXT("Silent Killing Strike");
	MistAmbush.AttackType = EEOSBossAttackType::MeleeCombo;
	MistAmbush.TelegraphType = EEOSBossTelegraphType::AudioCue;
	MistAmbush.TelegraphDuration = 1.0f;
	MistAmbush.BaseDamage = 110.0f;
	MistAmbush.EffectiveRange = 500.0f;
	MistAmbush.Cooldown = 6.0f;
	MistAmbush.RecoveryDuration = 1.4f;
	MistAmbush.bCanBeBlocked = false; // Must be dodged or countered
	MistAmbush.bCanBeDodged = true;
	MistAmbush.Element = EEOSChakraElement::Water;
	MistAmbush.AvailableInPhases = { 1, 2 };
	Def.AttackPatterns.Add(MistAmbush);

	// 4. Water Prison Jutsu Trap
	FEOSBossAttackPattern WaterPrison;
	WaterPrison.PatternID = TEXT("ZAB_WATER_PRISON_TRAP");
	WaterPrison.PatternName = TEXT("Water Prison Jutsu");
	WaterPrison.AttackType = EEOSBossAttackType::AreaOfEffect;
	WaterPrison.TelegraphType = EEOSBossTelegraphType::GroundIndicator;
	WaterPrison.TelegraphDuration = 1.2f;
	WaterPrison.BaseDamage = 85.0f;
	WaterPrison.EffectiveRange = 350.0f;
	WaterPrison.Cooldown = 8.0f;
	WaterPrison.RecoveryDuration = 1.6f;
	WaterPrison.bCanBeBlocked = false;
	WaterPrison.bCanBeDodged = true;
	WaterPrison.Element = EEOSChakraElement::Water;
	WaterPrison.AvailableInPhases = { 2 };
	Def.AttackPatterns.Add(WaterPrison);

	// 5. Water Style: Water Dragon Jutsu
	FEOSBossAttackPattern WaterDragon;
	WaterDragon.PatternID = TEXT("ZAB_WATER_DRAGON_BARRAGE");
	WaterDragon.PatternName = TEXT("Water Style: Water Dragon Jutsu");
	WaterDragon.AttackType = EEOSBossAttackType::JutsuSignature;
	WaterDragon.TelegraphType = EEOSBossTelegraphType::ChakraFlare;
	WaterDragon.TelegraphDuration = 1.5f;
	WaterDragon.BaseDamage = 160.0f;
	WaterDragon.EffectiveRange = 800.0f;
	WaterDragon.Cooldown = 10.0f;
	WaterDragon.RecoveryDuration = 2.0f;
	WaterDragon.bCanBeBlocked = true; // Can be guarded with high stamina drain
	WaterDragon.bCanBeDodged = true;
	WaterDragon.Element = EEOSChakraElement::Water;
	WaterDragon.AvailableInPhases = { 2, 3 };
	Def.AttackPatterns.Add(WaterDragon);

	// 6. Demon Berserk Combo
	FEOSBossAttackPattern Berserk;
	Berserk.PatternID = TEXT("ZAB_DEMON_BERSERK_COMBO");
	Berserk.PatternName = TEXT("Demon Flurry Combo");
	Berserk.AttackType = EEOSBossAttackType::MeleeCombo;
	Berserk.TelegraphType = EEOSBossTelegraphType::WindUp;
	Berserk.TelegraphDuration = 0.8f;
	Berserk.BaseDamage = 190.0f;
	Berserk.EffectiveRange = 220.0f;
	Berserk.Cooldown = 7.0f;
	Berserk.RecoveryDuration = 1.8f;
	Berserk.bCanBeBlocked = true;
	Berserk.bCanBeDodged = true;
	Berserk.Element = EEOSChakraElement::Water;
	Berserk.AvailableInPhases = { 3 };
	Def.AttackPatterns.Add(Berserk);

	// 7. Hidden Mist Executioner Ultimate
	FEOSBossAttackPattern Ultimate;
	Ultimate.PatternID = TEXT("ZAB_HIDDEN_MIST_EXECUTOR");
	Ultimate.PatternName = TEXT("Hidden Mist Decapitation");
	Ultimate.AttackType = EEOSBossAttackType::JutsuUltimate;
	Ultimate.TelegraphType = EEOSBossTelegraphType::ChakraFlare;
	Ultimate.TelegraphDuration = 1.8f;
	Ultimate.BaseDamage = 240.0f;
	Ultimate.EffectiveRange = 400.0f;
	Ultimate.Cooldown = 12.0f;
	Ultimate.RecoveryDuration = 2.5f;
	Ultimate.bCanBeBlocked = false; // Ultimate cannot be blocked — dodge required
	Ultimate.bCanBeDodged = true;
	Ultimate.Element = EEOSChakraElement::Water;
	Ultimate.AvailableInPhases = { 3 };
	Def.AttackPatterns.Add(Ultimate);

	return Def;
}

AEOSBossCharacter* UEOSBossEncounterSubsystem::StartEncounter(const FString& BossID, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	const FEOSBossEncounterDefinition* Def = BossEncounterRegistry.Find(BossID);
	if (!Def)
	{
		UE_LOG(LogTemp, Error, TEXT("[EOS][BOSS_SUBSYSTEM] StartEncounter failed: Boss ID '%s' not registered!"), *BossID);
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[EOS][BOSS_SUBSYSTEM] StartEncounter failed: World is null!"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ActiveBossCharacter = World->SpawnActor<AEOSBossCharacter>(AEOSBossCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (ActiveBossCharacter)
	{
		ActiveBossCharacter->InitializeFromEncounterDefinition(*Def);
		ActiveBossCharacter->TriggerEntrance();

		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS_SUBSYSTEM] Started encounter with %s at %s"),
			*Def->BossName, *SpawnLocation.ToString());
	}

	return ActiveBossCharacter;
}

FEOSBossEncounterResult UEOSBossEncounterSubsystem::EndEncounter()
{
	FEOSBossEncounterResult Result;

	if (ActiveBossCharacter)
	{
		Result = ActiveBossCharacter->GenerateEncounterResult();

		// Record to encounter history
		EncounterHistory.Add(Result);

		// Push to Adaptive AI subsystem for nemesis memory
		RecordToAdaptiveAI(Result);

		UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS_SUBSYSTEM] Encounter with %s ended. Victory: %s, Duration: %.1fs"),
			*Result.BossID, Result.bPlayerVictory ? TEXT("YES") : TEXT("NO"), Result.EncounterDurationSeconds);

		ActiveBossCharacter = nullptr;
	}

	return Result;
}

TArray<FEOSBossEncounterResult> UEOSBossEncounterSubsystem::GetEncounterHistory(const FString& BossID) const
{
	TArray<FEOSBossEncounterResult> Filtered;
	for (const FEOSBossEncounterResult& Record : EncounterHistory)
	{
		if (Record.BossID == BossID)
		{
			Filtered.Add(Record);
		}
	}
	return Filtered;
}

int32 UEOSBossEncounterSubsystem::GetTotalEncounterCount() const
{
	return EncounterHistory.Num();
}

void UEOSBossEncounterSubsystem::RecordToAdaptiveAI(const FEOSBossEncounterResult& Result)
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	UEOSAdaptiveAISubsystem* AISubsystem = GI->GetSubsystem<UEOSAdaptiveAISubsystem>();
	if (!AISubsystem)
	{
		return;
	}

	FEOSBossMemoryRecord Record = AISubsystem->GetBossMemory(Result.BossID);
	Record.BossID = Result.BossID;
	Record.TotalRunCount += 1;

	if (Result.bPlayerVictory)
	{
		Record.FailedStrategyCount += 1;
	}
	else
	{
		Record.SuccessfulStrategyCount += 1;
	}

	AISubsystem->SaveBossMemory(Record);
	UE_LOG(LogTemp, Log, TEXT("[EOS][BOSS_SUBSYSTEM] Updated AdaptiveAI memory for %s: Runs=%d, WinRate=%.1f%%"),
		*Result.BossID, Record.TotalRunCount,
		Record.TotalRunCount > 0 ? (float)Record.SuccessfulStrategyCount / (float)Record.TotalRunCount * 100.0f : 0.0f);
}
