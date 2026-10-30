#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "../../Public/Tests/EOSPhase13Tests.h"
#include "../../Public/Data/EOSPresentationDataTypes.h"
#include "../../Public/Data/EOSVisualAssetGate.h"
#include "../../Public/AI/EOSVillageNPC.h"
#include "../../Public/Character/EOSCharacterBase.h"
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

/**
 * VISUAL-RECTIFICATION-MASTER-1.1 — Step 23: FEOSVisualAssetValidationTest.
 * Strict visual asset validation with VisualAssetAutomationReport.md generation.
 * MUST FAIL while required production assets are missing (Section 5 taxonomy).
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSVisualAssetValidationTest, "EchoesOfShinobi.VisualRectification.VisualAssetValidation",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSVisualAssetValidationTest::RunTest(const FString& Parameters)
{
	UEOSVisualAssetGate* Gate = NewObject<UEOSVisualAssetGate>();
	if (!TestTrue(TEXT("VisualAssetRegistry.csv loads from Data/Visual/"), Gate->LoadRegistryFromDisk()))
	{
		return false;
	}
	TestTrue(TEXT("Registry contains the full asset roster (>= 70 rows)"), Gate->GetRegistryRowCount() >= 70);

	const TArray<FEOSVisualAssetSlotVerdict> Verdicts = Gate->ValidateAllRegistryAssets(/*bLogResults*/ false);
	TestEqual(TEXT("Every registry row produced a verdict"), Verdicts.Num(), Gate->GetRegistryRowCount());

	// Strict Section 5 contract: the suite MUST fail while production assets are missing.
	// This is the honest gate that blocks 'VISUAL RUNTIME ACTIVATION' until real assets exist.
	const bool bAllAssetsProductionReady = (Gate->GetLastValidationFailCount() == 0);
	TestTrue(TEXT("All registry assets resolve to real authored packages (no MISSING_PACKAGE / forbidden placeholders)"), bAllAssetsProductionReady);

	const FString ReportPath = Gate->WriteAutomationReport(Verdicts);
	TestFalse(TEXT("VisualAssetAutomationReport.md written"), ReportPath.IsEmpty());

	return Gate->GetLastValidationFailCount() == 0;
}

/**
 * Step 6: runtime soft-reference resolver + Section 5 gate on the proof-of-pipeline character.
 * Verifies Naruto Genin resolves canonical /Game/ paths and reports honest MISSING_PACKAGE
 * verdicts (never placeholder geometry) until authored meshes are imported.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSCharacterVisualGateTest, "EchoesOfShinobi.VisualRectification.CharacterVisualGate",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSCharacterVisualGateTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("Editor world available for character spawn"), World))
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSCharacterBase* Shinobi = World->SpawnActor<AEOSCharacterBase>(FVector(0.0f, 0.0f, 200.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("AEOSCharacterBase spawned"), Shinobi))
	{
		return false;
	}

	// Canonical proof-of-pipeline defaults (Naruto Genin).
	const bool bMeshPathSet = Shinobi->VisualProfile.SkeletalMeshAsset.ToSoftObjectPath().ToString().Contains(TEXT("SK_Naruto_Genin"));
	TestTrue(TEXT("Naruto Genin skeletal mesh soft reference resolves canonical registry path"), bMeshPathSet);

	const bool bVFXPathSet = Shinobi->JutsuVFXAsset.ToSoftObjectPath().ToString().Contains(TEXT("NS_Rasengan_Orb_Master"));
	TestTrue(TEXT("Rasengan Niagara soft reference resolves canonical registry path"), bVFXPathSet);

	const TArray<FEOSVisualAssetSlotVerdict> Verdicts = Shinobi->ValidateVisualAssetIntegrity(/*bLogResults*/ false);
	TestTrue(TEXT("Character visual gate produced verdicts for every slot"), Verdicts.Num() >= 4);

	// Section 5: verification that SK_Naruto_Genin produces an honest gate verdict (Passes if authored, or FailMissingPackage if unauthored).
	bool bFoundMeshSlot = false;
	for (const FEOSVisualAssetSlotVerdict& Verdict : Verdicts)
	{
		if (Verdict.AssetSlot == TEXT("SkeletalMesh"))
		{
			bFoundMeshSlot = true;
			TestTrue(TEXT("SK_Naruto_Genin produces an authoritative visual gate verdict (honest gate)"),
				Verdict.bPassed || Verdict.FailureCode == FEOSVisualRectificationStatus::FailMissingPackage);
		}
	}
	TestTrue(TEXT("SkeletalMesh slot present in verdicts"), bFoundMeshSlot);

	Shinobi->Destroy();
	return true;
}

/**
 * Strict classifier coverage: forbidden placeholder taxonomy can never produce a passing verdict.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSVisualForbiddenClassifierTest, "EchoesOfShinobi.VisualRectification.ForbiddenAssetClassifiers",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSVisualForbiddenClassifierTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Engine/BasicShapes/Cube is forbidden"),
		FEOSVisualRectificationStatus::IsForbiddenEngineBasicShape(TEXT("/Engine/BasicShapes/Cube.Cube")));
	TestTrue(TEXT("Default mannequin SKM_Quinn is forbidden"),
		FEOSVisualRectificationStatus::IsForbiddenDefaultMannequin(TEXT("/Game/Characters/SKM_Quinn")));
	TestTrue(TEXT("Checkerboard material is forbidden"),
		FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(TEXT("/Engine/EngineMaterials/Checkerboard")));
	TestTrue(TEXT("WorldGridMaterial is forbidden"),
		FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(TEXT("/Engine/EngineMaterials/WorldGridMaterial")));
	TestFalse(TEXT("Authored cel-shading material is NOT forbidden"),
		FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(TEXT("/Game/Materials/Characters/MI_Naruto_Genin")));

	return true;
}

#endif
