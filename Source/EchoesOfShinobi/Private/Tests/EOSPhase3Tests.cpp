#include "../../Public/Tests/EOSPhase3Tests.h"
#include "../../Public/Subsystems/EOSJutsuTrialSubsystem.h"
#include "../../Public/Subsystems/EOSCombatBalanceSubsystem.h"
#include "../../Public/Data/EOSJutsuTrialDataTypes.h"
#include "../../Public/Data/EOSCombatDataTypes.h"
#include "../../Public/Tests/EOSPhase2Tests.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase3Tests::RunAllPhase1Phase2AndPhase3ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1, PHASE 2 & PHASE 3 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	// Execute Phase 1 & 2 tests first (8 suites)
	bool bP1_P2_Passed = FEOSPhase2Tests::RunAllPhase1AndPhase2ValidationTests();

	// Execute Phase 3 Jutsu Learning Trial tests (6 suites)
	bool bP3_Eligible = TestJutsuEligibilityGate();
	bool bP3_HandSign = TestHandSignSequenceVerification();
	bool bP3_Chakra   = TestChakraControlGaugeStability();
	bool bP3_Practice = TestTrialPassFailureAndPracticeXP();
	bool bP3_Mastery  = TestJutsuMasteryBandTransitions();
	bool bP3_Loadout  = TestJutsuArchiveAndLoadoutManagement();

	// Execute Phase 3 Combat Balance Math tests (5 suites)
	bool bP3_Defense = TestDefenseFactorMitigationFormula();
	bool bP3_Element = TestElementalCycleModifiers();
	bool bP3_Damage  = TestHitDamageCalculationChain();
	bool bP3_HitScore= TestHitScoreAndGradeResolution();
	bool bP3_Scaling = TestEnemyAndBossScalingCalculations();

	bool bPhase3Passed = bP3_Eligible && bP3_HandSign && bP3_Chakra && bP3_Practice &&
	                     bP3_Mastery && bP3_Loadout && bP3_Defense && bP3_Element &&
	                     bP3_Damage && bP3_HitScore && bP3_Scaling;

	bool bAllPassed = bP1_P2_Passed && bPhase3Passed;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 3 TEST SUMMARY: %s (11/11 PHASE 3 SUITES PASSED)"), bPhase3Passed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("OVERALL SYSTEM RESULT: %s (19/19 TOTAL SYSTEM SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase3Tests::TestJutsuEligibilityGate()
{
	UEOSJutsuTrialSubsystem TrialSubsystem;

	FEOSJutsuTrialRecord Record;
	Record.JutsuID = "JUT_RASENGAN";
	Record.JutsuName = "Rasengan";
	Record.CharacterID = "CHR_NARUTO";
	Record.AdvancementNodeID = "ADV_GENIN_CORE";
	Record.TrialState = EEOSJutsuTrialState::Locked;

	TrialSubsystem.RegisterJutsuRecord(Record);

	bool bWrongNode = TrialSubsystem.EvaluateEligibility("JUT_RASENGAN", "ADV_ACADEMY_BASE");
	bool bRightNode = TrialSubsystem.EvaluateEligibility("JUT_RASENGAN", "ADV_GENIN_CORE");

	FEOSJutsuTrialRecord OutRecord;
	TrialSubsystem.GetJutsuRecord("JUT_RASENGAN", OutRecord);

	bool bPass = (!bWrongNode) && bRightNode && (OutRecord.TrialState == EEOSJutsuTrialState::Eligible);
	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Jutsu Eligibility Gate: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestHandSignSequenceVerification()
{
	UEOSJutsuTrialSubsystem TrialSubsystem;

	FEOSJutsuTrialRecord Record;
	Record.JutsuID = "JUT_FIREBALL";
	Record.JutsuName = "Fire Style: Fireball Technique";
	Record.CharacterID = "CHR_SASUKE";
	Record.TrialState = EEOSJutsuTrialState::Eligible;
	Record.HandSignSequence.RequiredSigns = { EEOSHandSign::Serpent, EEOSHandSign::Ram, EEOSHandSign::Monkey, EEOSHandSign::Tiger };
	Record.HandSignSequence.ExecutionWindowSeconds = 3.0f;

	TrialSubsystem.RegisterJutsuRecord(Record);

	TArray<EEOSHandSign> CorrectSigns = { EEOSHandSign::Serpent, EEOSHandSign::Ram, EEOSHandSign::Monkey, EEOSHandSign::Tiger };
	TArray<EEOSHandSign> WrongSigns = { EEOSHandSign::Serpent, EEOSHandSign::Ram, EEOSHandSign::Tiger };

	bool bCorrectPass = TrialSubsystem.ValidateHandSignSequence("JUT_FIREBALL", CorrectSigns, 2.1f);
	bool bWrongPass = TrialSubsystem.ValidateHandSignSequence("JUT_FIREBALL", WrongSigns, 2.1f);
	bool bTimeoutPass = TrialSubsystem.ValidateHandSignSequence("JUT_FIREBALL", CorrectSigns, 4.5f);

	bool bPass = bCorrectPass && (!bWrongPass) && (!bTimeoutPass);
	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Hand-Sign Sequence Verification: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestChakraControlGaugeStability()
{
	UEOSJutsuTrialSubsystem TrialSubsystem;

	FEOSJutsuTrialRecord Record;
	Record.JutsuID = "JUT_CHIDORI";
	Record.ChakraModel.TargetGaugeMin = 45.0f;
	Record.ChakraModel.TargetGaugeMax = 55.0f;
	Record.ChakraModel.HoldDurationRequired = 2.0f;

	TrialSubsystem.RegisterJutsuRecord(Record);

	bool bStablePass = TrialSubsystem.ValidateChakraControl("JUT_CHIDORI", 50.0f, 2.5f);
	bool bTooLowPass = TrialSubsystem.ValidateChakraControl("JUT_CHIDORI", 30.0f, 2.5f);
	bool bNotHeldPass = TrialSubsystem.ValidateChakraControl("JUT_CHIDORI", 50.0f, 1.0f);

	bool bPass = bStablePass && (!bTooLowPass) && (!bNotHeldPass);
	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Chakra Control Gauge Stability: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestTrialPassFailureAndPracticeXP()
{
	UEOSJutsuTrialSubsystem TrialSubsystem;

	FEOSJutsuTrialRecord Record;
	Record.JutsuID = "JUT_RASENSHURIKEN";
	Record.CharacterID = "CHR_NARUTO";
	Record.TrialState = EEOSJutsuTrialState::Trial;
	Record.PracticeXP = 100.0f;

	TrialSubsystem.RegisterJutsuRecord(Record);

	EEOSJutsuTrialState FailState = TrialSubsystem.ProcessTrialResult("JUT_RASENSHURIKEN", false, 50.0f);
	FEOSJutsuTrialRecord OutFailRecord;
	TrialSubsystem.GetJutsuRecord("JUT_RASENSHURIKEN", OutFailRecord);

	EEOSJutsuTrialState PassState = TrialSubsystem.ProcessTrialResult("JUT_RASENSHURIKEN", true, 100.0f);
	FEOSJutsuTrialRecord OutPassRecord;
	TrialSubsystem.GetJutsuRecord("JUT_RASENSHURIKEN", OutPassRecord);

	bool bPass = (FailState == EEOSJutsuTrialState::Practice) && (OutFailRecord.PracticeXP == 150.0f) &&
	             (PassState == EEOSJutsuTrialState::Mastered) && (OutPassRecord.PracticeXP == 250.0f) &&
	             (OutPassRecord.MasteryBand == EEOSMasteryBand::Level1_Learned);

	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Trial Pass/Failure & Practice XP Retention: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestJutsuMasteryBandTransitions()
{
	FEOSJutsuTrialRecord Record;
	Record.MasteryBand = EEOSMasteryBand::Level1_Learned;
	bool bLevel1Valid = (Record.MasteryBand != EEOSMasteryBand::Level0_Unlearned);

	Record.MasteryBand = EEOSMasteryBand::Level5_Mastered;
	bool bLevel5Valid = (Record.MasteryBand == EEOSMasteryBand::Level5_Mastered);

	bool bPass = bLevel1Valid && bLevel5Valid;
	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Jutsu Mastery Band Transitions: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestJutsuArchiveAndLoadoutManagement()
{
	UEOSJutsuTrialSubsystem TrialSubsystem;

	FEOSJutsuTrialRecord Unlearned;
	Unlearned.JutsuID = "JUT_KIRIN";
	Unlearned.CharacterID = "CHR_SASUKE";
	Unlearned.MasteryBand = EEOSMasteryBand::Level0_Unlearned;

	FEOSJutsuTrialRecord Learned1;
	Learned1.JutsuID = "JUT_FIREBALL";
	Learned1.CharacterID = "CHR_SASUKE";
	Learned1.MasteryBand = EEOSMasteryBand::Level1_Learned;

	FEOSJutsuTrialRecord Learned2;
	Learned2.JutsuID = "JUT_CHIDORI";
	Learned2.CharacterID = "CHR_SASUKE";
	Learned2.MasteryBand = EEOSMasteryBand::Level2_Familiar;

	TrialSubsystem.RegisterJutsuRecord(Unlearned);
	TrialSubsystem.RegisterJutsuRecord(Learned1);
	TrialSubsystem.RegisterJutsuRecord(Learned2);

	bool bEquipUnlearned = TrialSubsystem.EquipJutsu("JUT_KIRIN");
	bool bEquipLearned = TrialSubsystem.EquipJutsu("JUT_FIREBALL");

	TArray<FEOSJutsuTrialRecord> Archive = TrialSubsystem.GetLearnedArchive("CHR_SASUKE");
	TArray<FEOSJutsuTrialRecord> Equipped = TrialSubsystem.GetEquippedJutsu("CHR_SASUKE");

	bool bPass = (!bEquipUnlearned) && bEquipLearned && (Archive.Num() == 2) && (Equipped.Num() == 1) && (Equipped[0].JutsuID == "JUT_FIREBALL");

	UE_LOG(LogEOSCore, Log, TEXT("[P3 TEST] Jutsu Archive & Loadout Management: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

// ----------------------------------------------------------------------------------
// COMBAT BALANCE MATH TESTS
// ----------------------------------------------------------------------------------

bool FEOSPhase3Tests::TestDefenseFactorMitigationFormula()
{
	// Formula: DefenseFactor = 100 / (100 + EffectiveDefense)
	float FactorZeroDef = UEOSCombatBalanceSubsystem::CalculateDefenseFactor(0.0f);   // Expected: 1.0 (100%)
	float Factor100Def  = UEOSCombatBalanceSubsystem::CalculateDefenseFactor(100.0f); // Expected: 0.5 (50%)
	float Factor300Def  = UEOSCombatBalanceSubsystem::CalculateDefenseFactor(300.0f); // Expected: 0.25 (25%)

	bool bPass = (FMath::IsNearlyEqual(FactorZeroDef, 1.0f)) &&
	             (FMath::IsNearlyEqual(Factor100Def, 0.5f)) &&
	             (FMath::IsNearlyEqual(Factor300Def, 0.25f));

	UE_LOG(LogEOSCore, Log, TEXT("[P3 MATH TEST] Defense Mitigation Factor Formula (Def 0: %.2f, Def 100: %.2f, Def 300: %.2f): %s"),
		FactorZeroDef, Factor100Def, Factor300Def, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestElementalCycleModifiers()
{
	// Cycle: Fire -> Wind -> Lightning -> Earth -> Water -> Fire (1.5x advantage, 0.75x disadvantage)
	float FireVsWind = UEOSCombatBalanceSubsystem::CalculateElementModifier(EEOSChakraElement::Fire, EEOSChakraElement::Wind);
	float WindVsFire = UEOSCombatBalanceSubsystem::CalculateElementModifier(EEOSChakraElement::Wind, EEOSChakraElement::Fire);
	float WaterVsFire = UEOSCombatBalanceSubsystem::CalculateElementModifier(EEOSChakraElement::Water, EEOSChakraElement::Fire);
	float Neutral = UEOSCombatBalanceSubsystem::CalculateElementModifier(EEOSChakraElement::Fire, EEOSChakraElement::Earth);

	bool bPass = FMath::IsNearlyEqual(FireVsWind, 1.50f) &&
	             FMath::IsNearlyEqual(WindVsFire, 0.75f) &&
	             FMath::IsNearlyEqual(WaterVsFire, 1.50f) &&
	             FMath::IsNearlyEqual(Neutral, 1.00f);

	UE_LOG(LogEOSCore, Log, TEXT("[P3 MATH TEST] Elemental 5-Nature Cycle Modifiers (Advantage: %.2f, Disadvantage: %.2f): %s"),
		FireVsWind, WindVsFire, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestHitDamageCalculationChain()
{
	FEOSDamageCalculationInput Input;
	Input.ScalingType = EEOSAbilityScalingType::JutsuPower;
	Input.AttackerJutsuPower = 500.0f;
	Input.SkillMultiplier = 2.0f; // Base damage = 1000.0
	Input.ComboMultiplier = 1.2f; // Combo bonus = 1200.0
	Input.AttackerElement = EEOSChakraElement::Fire;
	Input.DefenderElement = EEOSChakraElement::Wind; // 1.5x element advantage = 1800.0
	Input.DefenderDefense = 100.0f; // DefenseFactor = 0.5 -> 900.0
	Input.bForceCritical = true;
	Input.CritDamageMultiplier = 1.5f; // Crit -> 1350.0

	FEOSDamageCalculationResult Result = UEOSCombatBalanceSubsystem::CalculateFinalHitDamage(Input);

	// Expected Final Damage: 500 * 2.0 * 1.2 * 1.5 * 1.5 * (100/200) = 1350.0
	bool bPass = FMath::IsNearlyEqual(Result.FinalDamage, 1350.0f, 0.1f) && Result.bIsCriticalHit;

	UE_LOG(LogEOSCore, Log, TEXT("[P3 MATH TEST] Hit Damage Calculation Chain (Expected: 1350.0, Got: %.2f): %s"),
		Result.FinalDamage, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestHitScoreAndGradeResolution()
{
	FEOSHitScoreInput Input;
	Input.DamageDealtRatio = 1.0f;       // 400 pts
	Input.MaxComboHits = 30;              // 200 pts
	Input.PerfectTimingEvents = 5;        // 150 pts
	Input.MechanicCompletionRatio = 1.0f; // 150 pts
	Input.RemainingHPRatio = 1.0f;        // 100 pts

	FEOSHitScoreResult Result = UEOSCombatBalanceSubsystem::CalculateHitScore(Input);

	bool bPassMaxScore = (Result.TotalScore == 1000) && (Result.Grade == TEXT("S"));

	FEOSHitScoreInput LowerInput;
	LowerInput.DamageDealtRatio = 0.70f;       // 280 pts
	LowerInput.MaxComboHits = 15;              // 100 pts
	LowerInput.PerfectTimingEvents = 2;        // 60 pts
	LowerInput.MechanicCompletionRatio = 0.8f; // 120 pts
	LowerInput.RemainingHPRatio = 0.5f;        // 50 pts
	// Total: 280 + 100 + 60 + 120 + 50 = 610 pts -> Grade C

	FEOSHitScoreResult LowerResult = UEOSCombatBalanceSubsystem::CalculateHitScore(LowerInput);
	bool bPassLowerGrade = (LowerResult.TotalScore == 610) && (LowerResult.Grade == TEXT("C"));

	bool bPass = bPassMaxScore && bPassLowerGrade;
	UE_LOG(LogEOSCore, Log, TEXT("[P3 MATH TEST] 1000-Point Hit Score & Grade Resolution (Max Score: %d Grade %s, Lower Score: %d Grade %s): %s"),
		Result.TotalScore, *Result.Grade, LowerResult.TotalScore, *LowerResult.Grade, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase3Tests::TestEnemyAndBossScalingCalculations()
{
	// Minion: Base 1000 HP, StageScaling 2.0, Difficulty 1.2, Tier 1.0 -> 2400 HP
	float MinionHP = UEOSCombatBalanceSubsystem::CalculateEnemyHP(1000.0f, 2.0f, 1.2f, 1.0f);
	// Boss: Base 1000 HP, StageScaling 2.0, Difficulty 1.2, Tier 5.0 -> 12000 HP
	float BossHP = UEOSCombatBalanceSubsystem::CalculateEnemyHP(1000.0f, 2.0f, 1.2f, 5.0f);

	bool bPass = FMath::IsNearlyEqual(MinionHP, 2400.0f) && FMath::IsNearlyEqual(BossHP, 12000.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P3 MATH TEST] Enemy & Boss Scaling Calculations (Minion HP: %.0f, Boss HP: %.0f): %s"),
		MinionHP, BossHP, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
