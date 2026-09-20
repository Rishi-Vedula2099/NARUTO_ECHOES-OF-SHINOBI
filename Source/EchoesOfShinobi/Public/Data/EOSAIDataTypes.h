#pragma once

#include "../CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSAIDataTypes.generated.h"

/**
 * 3-Tier AI Archetype Classification
 */
UENUM(BlueprintType)
enum class EEOSAITier : uint8
{
	Minion    UMETA(DisplayName = "Tier 1: Minion (Squad Role / Zero Adaptation)"),
	MiniBoss  UMETA(DisplayName = "Tier 2: Mini-Boss (Phase Graph / Small Budget)"),
	MajorBoss UMETA(DisplayName = "Tier 3: Major Boss (Full Adaptation & Nemesis Memory)")
};

/**
 * Tier 1 Minion Squad Roles
 */
UENUM(BlueprintType)
enum class EEOSMinionRole : uint8
{
	MeleeRush,
	RangedPressure,
	SupportHealer,
	ShieldGuard,
	Swarm,
	Explosive
};

/**
 * Feature Extractor Snapshot of Player Telemetry
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSTelemetryFeatureSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	int32 PlayerDodgeBackwardCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	int32 PlayerGuardCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	int32 RangedJutsuSpamCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	float PreferredDistanceUnits = 500.0f; // Melee < 300, Mid 300-800, Ranged > 800

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	EEOSChakraElement FavoredElement = EEOSChakraElement::Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telemetry")
	float AggressionIndex = 0.8f;
};

/**
 * Boss Nemesis Memory Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossMemoryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossMemory")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossMemory")
	int32 TotalRunCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossMemory")
	FString PreferredCounterStrategyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossMemory")
	int32 SuccessfulStrategyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossMemory")
	int32 FailedStrategyCount = 0;
};

/**
 * Bounded Strategy Directive issued by Adaptation Director
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSAdaptationDirective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	FString SelectedStrategyID = "ST_Action_Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	float GapCloserWeightMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	bool bTriggerProjectileDefense = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	bool bTriggerCloseRangePunish = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adaptation")
	bool bObeysFairnessConstraints = true;
};
