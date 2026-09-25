#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSCombatDataTypes.h"
#include "EOSCombatBalanceSubsystem.generated.h"

/**
 * Subsystem providing runtime execution of the V5.2 Combat Balance, Damage, Elemental,
 * EXP, Mastery, Enemy Scaling, and 1000-Point Hit Score Formulas.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSCombatBalanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSCombatBalanceSubsystem();

	// Defense Factor Formula: DefenseFactor = 100 / (100 + EffectiveDefense)
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static float CalculateDefenseFactor(float EffectiveDefense);

	// Elemental Modifier Cycle Formula (Fire -> Wind -> Lightning -> Earth -> Water -> Fire)
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static float CalculateElementModifier(EEOSChakraElement Attacker, EEOSChakraElement Defender);

	// Final Hit Damage Formula
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static FEOSDamageCalculationResult CalculateFinalHitDamage(const FEOSDamageCalculationInput& Input);

	// Stage EXP Calculation Pipeline
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static int32 CalculateStageEXP(int32 BaseEXP, float DifficultyMultiplier, float PerformanceMultiplier, bool bFirstClear);

	// Jutsu Mastery XP Formula
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static float CalculateJutsuMasteryXP(float JutsuUseValue, float HitQuality, float StageMultiplier, bool bIsSpamming);

	// 1000-Point Hit Score Calculation
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static FEOSHitScoreResult CalculateHitScore(const FEOSHitScoreInput& Input);

	// Grade Resolution (S: 900-1000, A: 800-899, B: 650-799, C: 500-649, D: 300-499, E: <300)
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static FString ResolveCombatGrade(int32 HitScore);

	// Enemy HP & Attack Scaling
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static float CalculateEnemyHP(float BaseHP, float StageScaling, float DifficultyScaling, float TierMultiplier);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|CombatBalance")
	static float CalculateEnemyAttack(float BaseAttack, float StageScaling, float DifficultyScaling, float TierMultiplier);
};
