#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.generated.h"

/**
 * Canonical 5-Nature Transformation Elements
 */
UENUM(BlueprintType)
enum class EEOSChakraElement : uint8
{
	None,
	Fire,
	Wind,
	Lightning,
	Earth,
	Water
};

/**
 * Elemental Relation Type
 */
UENUM(BlueprintType)
enum class EEOSElementRelation : uint8
{
	Neutral,
	Advantage,
	Disadvantage
};

/**
 * Ability / Jutsu Scaling Type
 */
UENUM(BlueprintType)
enum class EEOSAbilityScalingType : uint8
{
	PhysicalAttack,
	JutsuPower,
	Hybrid,
	Ultimate,
	Fixed
};

/**
 * Complete Character Stat Block
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCombatStatBlock
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float MaxHP = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float CurrentHP = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float MaxChakra = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float CurrentChakra = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float EffectiveAttack = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float EffectiveDefense = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float EffectiveJutsuPower = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float CritRate = 0.15f; // 15%

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float CritDamageMultiplier = 1.50f; // 150%

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	float ChakraRecoveryRate = 10.0f; // per sec

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStats")
	EEOSChakraElement PrimaryElement = EEOSChakraElement::None;
};

/**
 * Damage Input Calculations
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSDamageCalculationInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	EEOSAbilityScalingType ScalingType = EEOSAbilityScalingType::PhysicalAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float AttackerAttack = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float AttackerJutsuPower = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float SkillMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float ComboMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	EEOSChakraElement AttackerElement = EEOSChakraElement::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	EEOSChakraElement DefenderElement = EEOSChakraElement::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float DefenderDefense = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float CritRate = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float CritDamageMultiplier = 1.50f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	bool bForceCritical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float StateModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageInput")
	float MinimumDamageFloor = 1.0f;
};

/**
 * Damage Calculation Result Output
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSDamageCalculationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	float ElementModifier = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	float CritModifier = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	bool bIsCriticalHit = false;

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	float DefenseFactor = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "DamageResult")
	float FinalDamage = 0.0f;
};

/**
 * Hit Score Calculation Inputs & Results
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSHitScoreInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScore")
	float DamageDealtRatio = 1.0f; // 0.0 to 1.0 relative to stage target

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScore")
	int32 MaxComboHits = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScore")
	int32 PerfectTimingEvents = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScore")
	float MechanicCompletionRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScore")
	float RemainingHPRatio = 0.85f;
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSHitScoreResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 DamageScore = 0; // max 400

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 ComboScore = 0; // max 200

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 TimingScore = 0; // max 150

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 MechanicScore = 0; // max 150

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 SurvivalScore = 0; // max 100

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	int32 TotalScore = 0; // max 1000

	UPROPERTY(BlueprintReadOnly, Category = "HitScore")
	FString Grade = "S"; // S, A, B, C, D, E
};
