#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "../../Public/Tests/EOSPhase13Tests.h"
#include "../../Public/Data/EOSPresentationDataTypes.h"
#include "../../Public/AI/EOSVillageNPC.h"
#include "../../EchoesOfShinobi.h"

#if WITH_DEV_AUTOMATION_TESTS || WITH_AUTOMATION_WORKER

/**
 * Authoritative Master Automation Test for ECHOES OF SHINOBI (Phase 1 through Phase 13).
 * Explicitly registered under the "EchoesOfShinobi" test namespace.
 * Prevents zero-test discovery failure in CI/headless runner.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSMasterValidationTest, "EchoesOfShinobi.AllPhasesMasterValidation",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSMasterValidationTest::RunTest(const FString& Parameters)
{
	UE_LOG(LogEOSCore, Log, TEXT("[AUTOMATION] Running EchoesOfShinobi.AllPhasesMasterValidation"));

	const bool bAllPhasesPassed = FEOSPhase13Tests::RunAllPhase13ValidationTests();
	TestTrue(TEXT("Phase 1 through Phase 13 Authoritative Validation Suite (114/114 Tests)"), bAllPhasesPassed);

	return bAllPhasesPassed;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSPhase13ProfilesTest, "EchoesOfShinobi.Phase13.CameraAndMovementProfiles",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSPhase13ProfilesTest::RunTest(const FString& Parameters)
{
	const bool bPassed = FEOSPhase13Tests::TestCameraAndMovementProfiles();
	TestTrue(TEXT("Phase 13 Camera, Movement & GTX 1650 Scalability Profiles"), bPassed);
	return bPassed;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSPhase13ProvenanceTest, "EchoesOfShinobi.Phase13.AssetProvenanceAndOriginality",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSPhase13ProvenanceTest::RunTest(const FString& Parameters)
{
	const bool bPassed = FEOSPhase13Tests::TestAssetProvenanceAndOriginalityGate();
	TestTrue(TEXT("Phase 13 Asset Provenance & Anti-Proprietary-Rip Originality Gate"), bPassed);
	return bPassed;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSPhase13VillageNPCTest, "EchoesOfShinobi.Phase13.KonohaVillageAndNPCs",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSPhase13VillageNPCTest::RunTest(const FString& Parameters)
{
	const bool bPassed = FEOSPhase13Tests::TestKonohaVillageBuilderAndNPCLODSimulation();
	TestTrue(TEXT("Phase 13 Authored Konoha District & Living NPC 4-Tier Simulation"), bPassed);
	return bPassed;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSPhase13JutsuCombatTest, "EchoesOfShinobi.Phase13.JutsuCombatAndTeam7",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSPhase13JutsuCombatTest::RunTest(const FString& Parameters)
{
	const bool bPassed = FEOSPhase13Tests::TestJutsuExecutionAndSubstitutionAction();
	TestTrue(TEXT("Phase 13 Jutsu Slots (Rasengan/Clone/Barrage/Substitution/Vermilion)"), bPassed);
	return bPassed;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSPhase13PlayableLoopTest, "EchoesOfShinobi.Phase13.CompleteVerticalSliceLoop",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSPhase13PlayableLoopTest::RunTest(const FString& Parameters)
{
	const bool bPassed = FEOSPhase13Tests::TestCompletePlayableVerticalSliceLoop();
	TestTrue(TEXT("Phase 13 Complete Playable Vertical Slice Loop (Konoha -> Minions -> Boss Kakashi -> Motion Manga -> Save)"), bPassed);
	return bPassed;
}

#endif
