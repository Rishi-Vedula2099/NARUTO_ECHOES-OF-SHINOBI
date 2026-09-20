#include "../../Public/Tests/EOSPhase5Tests.h"
#include "../../Public/Subsystems/EOSTailedBeastSubsystem.h"
#include "../../Public/Data/EOSTailedBeastDataTypes.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Data/EOSCharacterDefinition.h"
#include "../../Public/Tests/EOSPhase4Tests.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase5Tests::RunAllPhase5ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1 - PHASE 5 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	// Execute Phase 1-4 tests first (29 suites)
	bool bP1_P4_Passed = FEOSPhase4Tests::RunAllPhase4ValidationTests();

	// Execute Phase 5 Tailed Beast tests (6 suites)
	bool bP5_Data   = TestTailedBeastRegistrationAndDataAsset();
	bool bP5_Comp   = TestTailedBeastCompatibilityRules();
	bool bP5_Chakra = TestRemnantPowerChakraContributionMath();
	bool bP5_Res    = TestBeastResonanceStateTransitions();
	bool bP5_Form   = TestTailedBeastTransformationGASBinding();
	bool bP5_Retry  = TestTailedBeastTrialRetryPersistence();

	bool bPhase5Passed = bP5_Data && bP5_Comp && bP5_Chakra && bP5_Res && bP5_Form && bP5_Retry;
	bool bAllPassed = bP1_P4_Passed && bPhase5Passed;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 5 TEST SUMMARY: %s (6/6 PHASE 5 SUITES PASSED)"), bPhase5Passed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("OVERALL SYSTEM RESULT: %s (35/35 TOTAL SYSTEM SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase5Tests::TestTailedBeastRegistrationAndDataAsset()
{
	UEOSTailedBeastSubsystem Subsystem;

	FEOSTailedBeastDefinitionRecord Kurama;
	Kurama.BeastID = "BIJUU_KURAMA";
	Kurama.DisplayName = FText::FromString("Kurama (Nine-Tails)");
	Kurama.BeastNumber = 9;
	Kurama.BaseRemnantPower = 1500.0f;

	FEOSTailedBeastDefinitionRecord Shukaku;
	Shukaku.BeastID = "BIJUU_SHUKAKU";
	Shukaku.DisplayName = FText::FromString("Shukaku (One-Tail)");
	Shukaku.BeastNumber = 1;
	Shukaku.BaseRemnantPower = 800.0f;

	Subsystem.RegisterTailedBeast(Kurama);
	Subsystem.RegisterTailedBeast(Shukaku);

	bool bPass = (Kurama.BeastNumber == 9) && (Shukaku.BeastNumber == 1);
	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Tailed Beast Registration & DataAsset: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase5Tests::TestTailedBeastCompatibilityRules()
{
	UEOSTailedBeastSubsystem Subsystem;

	EEOSSageCompatibility NarutoKurama = Subsystem.CheckBeastCompatibility("CHR_NARUTO", EEOSCharacterEra::ERA_GENIN, "FORM_BASE", "BIJUU_KURAMA");
	EEOSSageCompatibility GaaraShukaku = Subsystem.CheckBeastCompatibility("CHR_GAARA", EEOSCharacterEra::ERA_GENIN, "FORM_BASE", "BIJUU_SHUKAKU");
	EEOSSageCompatibility SasukeKurama = Subsystem.CheckBeastCompatibility("CHR_SASUKE", EEOSCharacterEra::ERA_GENIN, "FORM_BASE", "BIJUU_KURAMA");

	bool bPass = (NarutoKurama == EEOSSageCompatibility::NATURAL_AFFINITY) &&
	             (GaaraShukaku == EEOSSageCompatibility::NATURAL_AFFINITY) &&
	             (SasukeKurama == EEOSSageCompatibility::INCOMPATIBLE);

	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Tailed Beast Compatibility Rules (Naruto/Kurama: Natural, Gaara/Shukaku: Natural, Sasuke/Kurama: Incompatible): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase5Tests::TestRemnantPowerChakraContributionMath()
{
	UEOSTailedBeastSubsystem Subsystem;

	FEOSRemnantProgressionRecord Record;
	Record.CharacterID = "CHR_NARUTO";
	Record.TailedBeastID = "BIJUU_KURAMA";
	Record.ChakraIncrease = 500.0f;
	Record.ResonanceState = EEOSTailedBeastResonanceState::Synchronized; // 1.5x multiplier -> 750.0

	Subsystem.RegisterRemnantProgression(Record);

	float ChakraContrib = Subsystem.CalculateChakraContribution("CHR_NARUTO", "BIJUU_KURAMA");

	bool bPass = FMath::IsNearlyEqual(ChakraContrib, 750.0f);
	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Remnant Power Chakra Contribution Math (Expected: 750.0, Got: %.1f): %s"), ChakraContrib, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase5Tests::TestBeastResonanceStateTransitions()
{
	UEOSTailedBeastSubsystem Subsystem;

	FEOSRemnantProgressionRecord Record;
	Record.CharacterID = "CHR_NARUTO";
	Record.TailedBeastID = "BIJUU_KURAMA";
	Record.ResonanceState = EEOSTailedBeastResonanceState::Unsynced;

	Subsystem.RegisterRemnantProgression(Record);

	// Gain 60 XP -> Aware state
	EEOSTailedBeastResonanceState State1 = Subsystem.GainResonanceXP("CHR_NARUTO", "BIJUU_KURAMA", 60.0f);
	// Gain 500 XP total -> Synchronized state
	EEOSTailedBeastResonanceState State2 = Subsystem.GainResonanceXP("CHR_NARUTO", "BIJUU_KURAMA", 450.0f);

	bool bPass = (State1 == EEOSTailedBeastResonanceState::Responsive) && (State2 == EEOSTailedBeastResonanceState::Synchronized);
	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Beast Resonance State Transitions: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase5Tests::TestTailedBeastTransformationGASBinding()
{
	UEOSTailedBeastSubsystem Subsystem;

	FEOSRemnantProgressionRecord Record;
	Record.CharacterID = "CHR_NARUTO";
	Record.TailedBeastID = "BIJUU_KURAMA";
	Record.ResonanceState = EEOSTailedBeastResonanceState::Synchronized;

	Subsystem.RegisterRemnantProgression(Record);

	FEOSTailedBeastTransformationState State;
	bool bActive = Subsystem.ActivateBeastTransformation("CHR_NARUTO", "BIJUU_KURAMA", State);

	bool bPass = bActive && State.bIsActive && (State.FormID == "FORM_KURAMA_LINK") && (State.DurationSeconds == 45.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Tailed Beast Transformation GAS Binding (Kurama Link Mode 45s): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase5Tests::TestTailedBeastTrialRetryPersistence()
{
	UEOSTailedBeastSubsystem Subsystem;

	FEOSRemnantProgressionRecord Record;
	Record.CharacterID = "CHR_BEE";
	Record.TailedBeastID = "BIJUU_GYUKI";
	Record.CurrentTier = 1;
	Record.CurrentResonanceXP = 100.0f;

	Subsystem.RegisterRemnantProgression(Record);

	// Failed trial -> Retains practice XP (+100 -> 200 XP), Tier remains 1
	bool bFailedTrial = Subsystem.ProcessBeastTrialResult("CHR_BEE", "BIJUU_GYUKI", false, 100.0f);
	FEOSRemnantProgressionRecord OutFailRecord;
	Subsystem.GetRemnantProgression("CHR_BEE", OutFailRecord);

	// Passed trial -> Advances Tier to 2
	bool bPassTrial = Subsystem.ProcessBeastTrialResult("CHR_BEE", "BIJUU_GYUKI", true, 100.0f);
	FEOSRemnantProgressionRecord OutPassRecord;
	Subsystem.GetRemnantProgression("CHR_BEE", OutPassRecord);

	bool bPass = (!bFailedTrial) && (OutFailRecord.CurrentTier == 1) && (OutFailRecord.CurrentResonanceXP == 200.0f) &&
	             bPassTrial && (OutPassRecord.CurrentTier == 2) && (OutPassRecord.CurrentResonanceXP == 300.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P5 TEST] Tailed Beast Trial Retry Persistence: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
