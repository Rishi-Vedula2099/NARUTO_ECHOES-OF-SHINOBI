#include "../../Public/Subsystems/EOSAdaptiveAISubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSAdaptiveAISubsystem::UEOSAdaptiveAISubsystem()
{
}

void UEOSAdaptiveAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PlayerTelemetryRegistry.Empty();
	BossMemoryRegistry.Empty();
}

void UEOSAdaptiveAISubsystem::Deinitialize()
{
	PlayerTelemetryRegistry.Empty();
	BossMemoryRegistry.Empty();
	Super::Deinitialize();
}

void UEOSAdaptiveAISubsystem::RecordCombatEvent(const FString& PlayerID, const FString& EventType, float Value)
{
	FEOSTelemetryFeatureSnapshot& Snapshot = PlayerTelemetryRegistry.FindOrAdd(PlayerID);

	if (EventType == "DODGE_BACKWARD")
	{
		Snapshot.PlayerDodgeBackwardCount += FMath::RoundToInt(Value);
	}
	else if (EventType == "RANGED_JUTSU")
	{
		Snapshot.RangedJutsuSpamCount += FMath::RoundToInt(Value);
	}
	else if (EventType == "GUARD")
	{
		Snapshot.PlayerGuardCount += FMath::RoundToInt(Value);
	}
	else if (EventType == "DISTANCE")
	{
		Snapshot.PreferredDistanceUnits = Value;
	}
}

FEOSTelemetryFeatureSnapshot UEOSAdaptiveAISubsystem::ExtractBehaviorFeatures(const FString& PlayerID) const
{
	const FEOSTelemetryFeatureSnapshot* Snapshot = PlayerTelemetryRegistry.Find(PlayerID);
	if (Snapshot)
	{
		return *Snapshot;
	}
	return FEOSTelemetryFeatureSnapshot();
}

FEOSBossMemoryRecord UEOSAdaptiveAISubsystem::GetBossMemory(const FString& BossID) const
{
	const FEOSBossMemoryRecord* Record = BossMemoryRegistry.Find(BossID);
	if (Record)
	{
		return *Record;
	}
	return FEOSBossMemoryRecord();
}

void UEOSAdaptiveAISubsystem::SaveBossMemory(const FEOSBossMemoryRecord& Record)
{
	if (!Record.BossID.IsEmpty())
	{
		BossMemoryRegistry.Add(Record.BossID, Record);
	}
}

FEOSAdaptationDirective UEOSAdaptiveAISubsystem::EvaluateAdaptationDirective(const FString& BossID, EEOSAITier Tier, const FEOSTelemetryFeatureSnapshot& Snapshot)
{
	FEOSAdaptationDirective Directive;
	Directive.BossID = BossID;
	Directive.bObeysFairnessConstraints = true;

	// Tier 1 Minions have zero adaptation budget
	if (Tier == EEOSAITier::Minion)
	{
		Directive.SelectedStrategyID = "ST_Minion_SquadDefault";
		return Directive;
	}

	// Adaptation Strategy Selection Logic based on Player Telemetry Features
	if (Snapshot.PlayerDodgeBackwardCount >= 5)
	{
		// Player dodges backward repeatedly -> Boost gap-closer action weights
		Directive.SelectedStrategyID = "ST_Action_GapCloser_DashStrike";
		Directive.GapCloserWeightMultiplier = 2.0f;
	}
	else if (Snapshot.RangedJutsuSpamCount >= 5 || Snapshot.PreferredDistanceUnits > 700.0f)
	{
		// Player spams ranged jutsu -> Trigger projectile defense & teleport close
		Directive.SelectedStrategyID = "ST_Action_Barrier_TeleportClose";
		Directive.bTriggerProjectileDefense = true;
	}
	else if (Snapshot.PreferredDistanceUnits < 300.0f && Snapshot.PlayerGuardCount >= 5)
	{
		// Player relies on close-range guard -> Trigger close-range punish pattern
		Directive.SelectedStrategyID = "ST_Action_MeleeGuardBreak_Punish";
		Directive.bTriggerCloseRangePunish = true;
	}
	else
	{
		Directive.SelectedStrategyID = "ST_Action_Balanced_Standard";
	}

	return Directive;
}

bool UEOSAdaptiveAISubsystem::ValidateFairnessConstraints(const FEOSAdaptationDirective& Directive, bool bAttemptsInputReading, bool bAttemptsDamageFormulaTampering)
{
	// Strictly prohibit reading frame 0 inputs or altering damage formulas
	if (bAttemptsInputReading || bAttemptsDamageFormulaTampering)
	{
		return false;
	}

	return Directive.bObeysFairnessConstraints;
}
