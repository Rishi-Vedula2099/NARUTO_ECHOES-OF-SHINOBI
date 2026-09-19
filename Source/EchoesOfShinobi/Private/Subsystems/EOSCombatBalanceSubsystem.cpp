#include "../../Public/Subsystems/EOSCombatBalanceSubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSCombatBalanceSubsystem::UEOSCombatBalanceSubsystem()
{
}

float UEOSCombatBalanceSubsystem::CalculateDefenseFactor(float EffectiveDefense)
{
	float SafeDefense = FMath::Max(0.0f, EffectiveDefense);
	return 100.0f / (100.0f + SafeDefense);
}

float UEOSCombatBalanceSubsystem::CalculateElementModifier(EEOSChakraElement Attacker, EEOSChakraElement Defender)
{
	if (Attacker == EEOSChakraElement::None || Defender == EEOSChakraElement::None)
	{
		return 1.0f;
	}

	// Canonical 5-Nature Cycle: Fire -> Wind -> Lightning -> Earth -> Water -> Fire
	if ((Attacker == EEOSChakraElement::Fire && Defender == EEOSChakraElement::Wind) ||
		(Attacker == EEOSChakraElement::Wind && Defender == EEOSChakraElement::Lightning) ||
		(Attacker == EEOSChakraElement::Lightning && Defender == EEOSChakraElement::Earth) ||
		(Attacker == EEOSChakraElement::Earth && Defender == EEOSChakraElement::Water) ||
		(Attacker == EEOSChakraElement::Water && Defender == EEOSChakraElement::Fire))
	{
		return 1.50f; // Counter Advantage
	}

	// Reverse Disadvantage
	if ((Attacker == EEOSChakraElement::Wind && Defender == EEOSChakraElement::Fire) ||
		(Attacker == EEOSChakraElement::Lightning && Defender == EEOSChakraElement::Wind) ||
		(Attacker == EEOSChakraElement::Earth && Defender == EEOSChakraElement::Lightning) ||
		(Attacker == EEOSChakraElement::Water && Defender == EEOSChakraElement::Earth) ||
		(Attacker == EEOSChakraElement::Fire && Defender == EEOSChakraElement::Water))
	{
		return 0.75f; // Disadvantage
	}

	return 1.0f; // Neutral
}

FEOSDamageCalculationResult UEOSCombatBalanceSubsystem::CalculateFinalHitDamage(const FEOSDamageCalculationInput& Input)
{
	FEOSDamageCalculationResult Result;

	// 1. Calculate Base Damage based on Scaling Type
	switch (Input.ScalingType)
	{
	case EEOSAbilityScalingType::JutsuPower:
		Result.BaseDamage = Input.AttackerJutsuPower * Input.SkillMultiplier;
		break;
	case EEOSAbilityScalingType::Hybrid:
		Result.BaseDamage = (Input.AttackerAttack * 0.5f + Input.AttackerJutsuPower * 0.5f) * Input.SkillMultiplier;
		break;
	case EEOSAbilityScalingType::Ultimate:
		Result.BaseDamage = (Input.AttackerAttack * 0.4f + Input.AttackerJutsuPower * 0.6f) * Input.SkillMultiplier * 1.5f;
		break;
	case EEOSAbilityScalingType::Fixed:
		Result.BaseDamage = Input.SkillMultiplier;
		break;
	case EEOSAbilityScalingType::PhysicalAttack:
	default:
		Result.BaseDamage = Input.AttackerAttack * Input.SkillMultiplier;
		break;
	}

	// 2. Element Modifier
	Result.ElementModifier = CalculateElementModifier(Input.AttackerElement, Input.DefenderElement);

	// 3. Crit Modifier
	Result.bIsCriticalHit = Input.bForceCritical || (FMath::FRand() < FMath::Clamp(Input.CritRate, 0.0f, 0.80f));
	Result.CritModifier = Result.bIsCriticalHit ? FMath::Max(1.0f, Input.CritDamageMultiplier) : 1.0f;

	// 4. Defense Factor
	Result.DefenseFactor = CalculateDefenseFactor(Input.DefenderDefense);

	// 5. Final Damage Calculation Chain
	float CalculatedDamage = Result.BaseDamage * Input.ComboMultiplier * Result.ElementModifier * Result.CritModifier * Result.DefenseFactor * Input.StateModifier;

	Result.FinalDamage = FMath::Max(Input.MinimumDamageFloor, CalculatedDamage);

	return Result;
}

int32 UEOSCombatBalanceSubsystem::CalculateStageEXP(int32 BaseEXP, float DifficultyMultiplier, float PerformanceMultiplier, bool bFirstClear)
{
	float FirstClearMult = bFirstClear ? 1.50f : 1.0f;
	float TotalEXP = (float)BaseEXP * DifficultyMultiplier * PerformanceMultiplier * FirstClearMult;
	return FMath::RoundToInt(TotalEXP);
}

float UEOSCombatBalanceSubsystem::CalculateJutsuMasteryXP(float JutsuUseValue, float HitQuality, float StageMultiplier, bool bIsSpamming)
{
	float AntiSpamMult = bIsSpamming ? 0.25f : 1.0f; // Diminishing returns for spam
	return JutsuUseValue * HitQuality * StageMultiplier * AntiSpamMult;
}

FEOSHitScoreResult UEOSCombatBalanceSubsystem::CalculateHitScore(const FEOSHitScoreInput& Input)
{
	FEOSHitScoreResult Result;

	// 1000-Point Score Distribution Allocation:
	// Damage (Max 400)
	Result.DamageScore = FMath::Clamp(FMath::RoundToInt(Input.DamageDealtRatio * 400.0f), 0, 400);

	// Combo (Max 200, 30 hits = max score)
	Result.ComboScore = FMath::Clamp(FMath::RoundToInt((float)Input.MaxComboHits / 30.0f * 200.0f), 0, 200);

	// Timing (Max 150, 5 perfect timing events = max score)
	Result.TimingScore = FMath::Clamp(FMath::RoundToInt((float)Input.PerfectTimingEvents / 5.0f * 150.0f), 0, 150);

	// Mechanics (Max 150)
	Result.MechanicScore = FMath::Clamp(FMath::RoundToInt(Input.MechanicCompletionRatio * 150.0f), 0, 150);

	// Survival (Max 100)
	Result.SurvivalScore = FMath::Clamp(FMath::RoundToInt(Input.RemainingHPRatio * 100.0f), 0, 100);

	Result.TotalScore = Result.DamageScore + Result.ComboScore + Result.TimingScore + Result.MechanicScore + Result.SurvivalScore;

	Result.Grade = ResolveCombatGrade(Result.TotalScore);

	return Result;
}

FString UEOSCombatBalanceSubsystem::ResolveCombatGrade(int32 HitScore)
{
	if (HitScore >= 900) return TEXT("S");
	if (HitScore >= 800) return TEXT("A");
	if (HitScore >= 650) return TEXT("B");
	if (HitScore >= 500) return TEXT("C");
	if (HitScore >= 300) return TEXT("D");
	return TEXT("E");
}

float UEOSCombatBalanceSubsystem::CalculateEnemyHP(float BaseHP, float StageScaling, float DifficultyScaling, float TierMultiplier)
{
	return BaseHP * StageScaling * DifficultyScaling * TierMultiplier;
}

float UEOSCombatBalanceSubsystem::CalculateEnemyAttack(float BaseAttack, float StageScaling, float DifficultyScaling, float TierMultiplier)
{
	return BaseAttack * StageScaling * DifficultyScaling * TierMultiplier;
}
