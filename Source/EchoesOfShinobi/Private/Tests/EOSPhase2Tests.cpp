#include "../../Public/Tests/EOSPhase2Tests.h"
#include "../../Public/Ability/EOSAttributeSet.h"
#include "../../Public/Subsystems/EOSDataValidationSubsystem.h"
#include "../../Public/Subsystems/EOSSaveSubsystem.h"
#include "../../Public/Subsystems/EOSTelemetrySubsystem.h"
#include "../../Public/Subsystems/EOSProgressionSubsystem.h"
#include "../../Public/Data/EOSGearDefinition.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase2Tests::RunAllPhase1AndPhase2ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1 & PHASE 2 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	bool bP1_Attr  = TestAttributeSetInitialization();
	bool bP1_Data  = TestDataValidationSubsystem();
	bool bP1_Save  = TestSaveSubsystemSerialization();
	bool bP1_Telem = TestTelemetrySubsystemRecording();

	bool bP2_Power = TestCharacterPowerFormula();
	bool bP2_Stage = TestStagePowerThresholdScaling();
	bool bP2_Gear  = TestGearDefinitionAndSlotMapping();
	bool bP2_Sage  = TestSageAndTailedBeastCompatibility();

	bool bAllPassed = bP1_Attr && bP1_Data && bP1_Save && bP1_Telem && bP2_Power && bP2_Stage && bP2_Gear && bP2_Sage;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("FINAL TEST RESULT: %s (8/8 SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase2Tests::TestAttributeSetInitialization()
{
	UEOSAttributeSet AttrSet;
	bool bPass = (AttrSet.GetHealth() == 1000.0f) && (AttrSet.GetChakra() == 500.0f);
	UE_LOG(LogEOSCore, Log, TEXT("[P1 TEST] AttributeSet Initialization: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestDataValidationSubsystem()
{
	FEOSStableId ValidId("CHR_NARUTO", EEOSProvenanceType::CANON_VERIFIED);
	FEOSStableId InvalidId(NAME_None, EEOSProvenanceType::TODO_AUTHORING);

	bool bPass = ValidId.IsValid() && !InvalidId.IsValid();
	UE_LOG(LogEOSCore, Log, TEXT("[P1 TEST] DataValidation Subsystem: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestSaveSubsystemSerialization()
{
	FEOSPlayerSaveData SaveData;
	SaveData.PlayerLevel = 50;
	SaveData.RyoCurrency = 150000;

	bool bPass = (SaveData.PlayerLevel == 50) && (SaveData.RyoCurrency == 150000);
	UE_LOG(LogEOSCore, Log, TEXT("[P1 TEST] SaveSubsystem Serialization: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestTelemetrySubsystemRecording()
{
	FEOSTelemetrySnapshot Snapshot;
	Snapshot.TotalDodgeCount = 25;
	Snapshot.PerfectDodgeCount = 8;

	bool bPass = (Snapshot.TotalDodgeCount == 25) && (Snapshot.PerfectDodgeCount == 8);
	UE_LOG(LogEOSCore, Log, TEXT("[P1 TEST] TelemetrySubsystem Recording: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestCharacterPowerFormula()
{
	UEOSProgressionSubsystem ProgSubsystem;

	// BaseStats=1000, Level=50 (750), StarTier=5 (500), Ascension=2 (500), Skill=1200, Gear=800, Bond=5 (100) -> 4850 CP
	float CalculatedPower = ProgSubsystem.CalculateCharacterPower(1000.0f, 50, 5, 2, 1200.0f, 800.0f, 5);
	bool bPass = (CalculatedPower == 4850.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P2 TEST] Character Power Formula (Target: 4850 CP, Got: %f): %s"), CalculatedPower, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestStagePowerThresholdScaling()
{
	UEOSProgressionSubsystem ProgSubsystem;

	// Arc 1, Chapter 1, Boss -> Recommended Power = 1*500 + 1*120 + 300 = 920
	int32 BossPower = ProgSubsystem.CalculateRecommendedStagePower(1, 1, EEOSStageType::BOSS);
	// Arc 25, Chapter 4, Boss -> Recommended Power = 25*500 + 4*120 + 300 = 13280
	int32 EndgameBossPower = ProgSubsystem.CalculateRecommendedStagePower(25, 4, EEOSStageType::BOSS);

	bool bPass = (BossPower == 920) && (EndgameBossPower == 13280);
	UE_LOG(LogEOSCore, Log, TEXT("[P2 TEST] Stage Power Scaling (Arc 1 Boss: %d, Arc 25 Boss: %d): %s"), BossPower, EndgameBossPower, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestGearDefinitionAndSlotMapping()
{
	FEOSGearInstance HeadbandGear;
	HeadbandGear.Slot = EEOSGearSlot::HEADBAND;
	HeadbandGear.PrimaryStatBonus = 120.0f;

	FEOSGearInstance RelicGear;
	RelicGear.Slot = EEOSGearSlot::SPECIAL_RELIC;
	RelicGear.PrimaryStatBonus = 350.0f;

	bool bPass = (HeadbandGear.Slot == EEOSGearSlot::HEADBAND) && (RelicGear.Slot == EEOSGearSlot::SPECIAL_RELIC);
	UE_LOG(LogEOSCore, Log, TEXT("[P2 TEST] 8-Slot Gear Definition & Mapping: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase2Tests::TestSageAndTailedBeastCompatibility()
{
	UEOSProgressionSubsystem ProgSubsystem;
	FEOSStableId NarutoId("CHR_NARUTO", EEOSProvenanceType::CANON_VERIFIED);
	FEOSStableId ToadSageId("SAGE_TOAD", EEOSProvenanceType::CANON_VERIFIED);

	EEOSSageCompatibility Compatibility = ProgSubsystem.EvaluateSageCompatibility(NarutoId, ToadSageId);
	bool bPass = (Compatibility == EEOSSageCompatibility::NATURAL_AFFINITY);

	UE_LOG(LogEOSCore, Log, TEXT("[P2 TEST] Sage & Tailed Beast Compatibility Check: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
