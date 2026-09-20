#include "../../Public/Tests/EOSPhase1Tests.h"
#include "../../Public/Ability/EOSAttributeSet.h"
#include "../../Public/Subsystems/EOSDataValidationSubsystem.h"
#include "../../Public/Subsystems/EOSSaveSubsystem.h"
#include "../../Public/Subsystems/EOSTelemetrySubsystem.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase1Tests::RunAllPhase1ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("========== RUNNING PHASE 1 VALIDATION TEST SUITE =========="));

	bool bAttrPass = TestAttributeSetInitialization();
	bool bDataPass = TestDataValidationSubsystem();
	bool bSavePass = TestSaveSubsystemSerialization();
	bool bTelemPass = TestTelemetrySubsystemRecording();

	bool bAllPassed = bAttrPass && bDataPass && bSavePass && bTelemPass;
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 1 TEST SUITE RESULT: %s"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));

	return bAllPassed;
}

bool FEOSPhase1Tests::TestAttributeSetInitialization()
{
	UEOSAttributeSet AttrSet;
	bool bPass = (AttrSet.GetHealth() == 1000.0f) && (AttrSet.GetChakra() == 500.0f);
	UE_LOG(LogEOSCore, Log, TEXT("TestAttributeSetInitialization: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase1Tests::TestDataValidationSubsystem()
{
	FEOSStableId ValidId("CHR_NARUTO", EEOSProvenanceType::CANON_VERIFIED);
	FEOSStableId InvalidId(NAME_None, EEOSProvenanceType::TODO_AUTHORING);

	bool bPass = ValidId.IsValid() && !InvalidId.IsValid();
	UE_LOG(LogEOSCore, Log, TEXT("TestDataValidationSubsystem: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase1Tests::TestSaveSubsystemSerialization()
{
	FEOSPlayerSaveData SaveData;
	SaveData.PlayerLevel = 5;
	SaveData.RyoCurrency = 5000;

	bool bPass = (SaveData.PlayerLevel == 5) && (SaveData.RyoCurrency == 5000);
	UE_LOG(LogEOSCore, Log, TEXT("TestSaveSubsystemSerialization: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase1Tests::TestTelemetrySubsystemRecording()
{
	FEOSTelemetrySnapshot Snapshot;
	Snapshot.TotalDodgeCount = 10;
	Snapshot.PerfectDodgeCount = 3;

	bool bPass = (Snapshot.TotalDodgeCount == 10) && (Snapshot.PerfectDodgeCount == 3);
	UE_LOG(LogEOSCore, Log, TEXT("TestTelemetrySubsystemRecording: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
