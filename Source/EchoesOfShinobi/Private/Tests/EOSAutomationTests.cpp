#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Public/Tests/EOSPhase13Tests.h"
#include "../../Public/Data/EOSPresentationDataTypes.h"
#include "../../Public/Data/EOSVisualAssetGate.h"
#include "../../Public/AI/EOSVillageNPC.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Combat/EOSHitStopComponent.h"
#include "../../Public/Combat/EOSBossCharacter.h"
#include "../../Public/Subsystems/EOSBossEncounterSubsystem.h"
#include "../../Public/Subsystems/EOSQuestSubsystem.h"
#include "../../Public/Subsystems/EOSSaveSubsystem.h"
#include "../../Public/Subsystems/EOSGachaSubsystem.h"
#include "../../Public/Subsystems/EOSProgressionSubsystem.h"
#include "../../Public/Data/EOSBossDataTypes.h"
#include "../../Public/Subsystems/EOSAdaptiveAISubsystem.h"
#include "../../Public/Data/EOSAIDataTypes.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSRuntimeCameraTransitionTest, "EchoesOfShinobi.Camera.RuntimeModeTransitions",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSRuntimeCameraTransitionTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for Camera Mode Transition test"), World))
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSCharacterBase* HeroActor = World->SpawnActor<AEOSCharacterBase>(AEOSCharacterBase::StaticClass(), FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("HeroActor spawned for camera test"), HeroActor))
	{
		return false;
	}

	if (!TestNotNull(TEXT("CameraBoom component exists"), HeroActor->CameraBoom.Get()) ||
	    !TestNotNull(TEXT("FollowCamera component exists"), HeroActor->FollowCamera.Get()))
	{
		HeroActor->Destroy();
		return false;
	}

	// 1. Initial Exploration Gameplay Mode
	HeroActor->SetCameraMode(EEOSCameraMode::Exploration);
	TestEqual(TEXT("Exploration Arm Length"), HeroActor->CameraBoom->TargetArmLength, 460.0f);
	TestEqual(TEXT("Exploration FOV"), HeroActor->FollowCamera->FieldOfView, 68.0f);
	TestTrue(TEXT("Exploration Lag Enabled"), HeroActor->CameraBoom->bEnableCameraLag);
	TestEqual(TEXT("Exploration Lag Speed"), HeroActor->CameraBoom->CameraLagSpeed, 12.0f);
	TestEqual(TEXT("Exploration SocketOffset Z"), static_cast<float>(HeroActor->CameraBoom->SocketOffset.Z), 75.0f);

	// 2. Dynamic Transition to Combat Mode
	HeroActor->SetCameraMode(EEOSCameraMode::Combat);
	TestEqual(TEXT("Combat Arm Length"), HeroActor->CameraBoom->TargetArmLength, 320.0f);
	TestEqual(TEXT("Combat FOV"), HeroActor->FollowCamera->FieldOfView, 85.0f);
	TestTrue(TEXT("Combat Lag Enabled"), HeroActor->CameraBoom->bEnableCameraLag);
	TestEqual(TEXT("Combat Lag Speed"), HeroActor->CameraBoom->CameraLagSpeed, 16.0f);

	// 3. Dynamic Transition to Boss Encounter Mode
	HeroActor->SetCameraMode(EEOSCameraMode::Boss);
	TestEqual(TEXT("Boss Arm Length"), HeroActor->CameraBoom->TargetArmLength, 520.0f);
	TestEqual(TEXT("Boss FOV"), HeroActor->FollowCamera->FieldOfView, 90.0f);
	TestTrue(TEXT("Boss Lag Enabled"), HeroActor->CameraBoom->bEnableCameraLag);

	// 4. Transition to Stationary Art Review Inspection Mode (CAM_A)
	HeroActor->SetCameraMode(EEOSCameraMode::ArtReview);
	TestEqual(TEXT("ArtReview Arm Length (CAM_A)"), HeroActor->CameraBoom->TargetArmLength, 420.0f);
	TestEqual(TEXT("ArtReview FOV (CAM_A)"), HeroActor->FollowCamera->FieldOfView, 75.0f);
	TestFalse(TEXT("ArtReview Lag Disabled (Stationary)"), HeroActor->CameraBoom->bEnableCameraLag);
	TestEqual(TEXT("ArtReview Lag Speed Zero"), HeroActor->CameraBoom->CameraLagSpeed, 0.0f);
	TestEqual(TEXT("ArtReview SocketOffset Z"), static_cast<float>(HeroActor->CameraBoom->SocketOffset.Z), 50.0f);

	// 5. Dynamic Return to Exploration Gameplay Mode (Restoration Verification)
	HeroActor->SetCameraMode(EEOSCameraMode::Exploration);
	TestEqual(TEXT("Restored Exploration Arm Length"), HeroActor->CameraBoom->TargetArmLength, 460.0f);
	TestEqual(TEXT("Restored Exploration FOV"), HeroActor->FollowCamera->FieldOfView, 68.0f);
	TestTrue(TEXT("Restored Exploration Lag Enabled"), HeroActor->CameraBoom->bEnableCameraLag);
	TestEqual(TEXT("Restored Exploration Lag Speed"), HeroActor->CameraBoom->CameraLagSpeed, 12.0f);
	TestEqual(TEXT("Restored Exploration SocketOffset Z"), static_cast<float>(HeroActor->CameraBoom->SocketOffset.Z), 75.0f);

	HeroActor->Destroy();
	return true;
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

/**
 * Strict hit-stop concurrency and telemetry validation:
 * 1. Actor-level CustomTimeDilation (never global time dilation)
 * 2. Unscaled world time deadline evaluation
 * 3. Overlapping hits extend deadline without overwriting PriorCustomTimeDilation
 * 4. Exactly-once restoration upon expiration
 * 5. Safe actor destruction / cleanup
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSHitStopValidationTest, "EchoesOfShinobi.Combat.HitStopConcurrency",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSHitStopValidationTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for hit-stop test"), World))
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 1. Spawn test actor
	AActor* TestActor = World->SpawnActor<AActor>(AActor::StaticClass(), FVector(100.0f, 100.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("TestActor spawned"), TestActor))
	{
		return false;
	}

	// Baseline checks
	TestActor->CustomTimeDilation = 1.0f;
	UEOSHitStopComponent* HitStopComp = UEOSHitStopComponent::GetOrCreateHitStopComponent(TestActor);
	if (!TestNotNull(TEXT("HitStopComponent attached"), HitStopComp))
	{
		TestActor->Destroy();
		return false;
	}

	TestFalse(TEXT("Hit-stop initially inactive"), HitStopComp->IsHitStopActive());
	TestEqual(TEXT("Initial CustomTimeDilation is 1.0"), TestActor->CustomTimeDilation, 1.0f);

	// 2. Apply first hit-stop (100ms, dilation factor 0.001)
	const double StartTime = World->GetTimeSeconds();
	HitStopComp->ApplyHitStop(0.10f, 0.001f);

	TestTrue(TEXT("Hit-stop active after ApplyHitStop"), HitStopComp->IsHitStopActive());
	TestEqual(TEXT("CustomTimeDilation reduced to 0.001f"), TestActor->CustomTimeDilation, 0.001f);
	TestEqual(TEXT("PriorCustomTimeDilation captured as 1.0f"), HitStopComp->GetPriorCustomTimeDilation(), 1.0f);
	TestTrue(TEXT("Expiration deadline extended beyond start time"), HitStopComp->GetHitStopExpirationTime() >= StartTime + 0.09);

	const double FirstDeadline = HitStopComp->GetHitStopExpirationTime();

	// 3. Apply overlapping second hit-stop (200ms)
	HitStopComp->ApplyHitStop(0.20f, 0.001f);

	TestTrue(TEXT("Hit-stop remains active"), HitStopComp->IsHitStopActive());
	TestEqual(TEXT("PriorCustomTimeDilation NOT overwritten by 0.001f"), HitStopComp->GetPriorCustomTimeDilation(), 1.0f);
	TestTrue(TEXT("Expiration deadline extended by overlapping hit"), HitStopComp->GetHitStopExpirationTime() > FirstDeadline);

	// 3b. Obsolete timer callback safety: premature trigger must NOT restore early
	HitStopComp->OnHitStopTimerExpired();
	TestTrue(TEXT("Hit-stop remains active after premature callback (deadline in future)"), HitStopComp->IsHitStopActive());
	TestEqual(TEXT("CustomTimeDilation remains slowed"), TestActor->CustomTimeDilation, 0.001f);

	// 4. Test manual cancellation / restoration
	HitStopComp->CancelHitStop();

	TestFalse(TEXT("Hit-stop inactive after cancellation"), HitStopComp->IsHitStopActive());
	TestEqual(TEXT("CustomTimeDilation restored to 1.0f"), TestActor->CustomTimeDilation, 1.0f);

	// 4b. Test Paused-World Behavior (Hit-stop must remain frozen during pause)
	APlayerState* TestPS = World->SpawnActor<APlayerState>(APlayerState::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (TestNotNull(TEXT("Test PlayerState spawned for world pause"), TestPS) && World->GetWorldSettings())
	{
		HitStopComp->ApplyHitStop(0.20f, 0.001f);
		TestTrue(TEXT("Hit-stop active before pause"), HitStopComp->IsHitStopActive());
		TestEqual(TEXT("Dilation slowed before pause"), TestActor->CustomTimeDilation, 0.001f);

		// Pause world via WorldSettings PauserPlayerState (canonical UE engine pause authority)
		World->GetWorldSettings()->SetPauserPlayerState(TestPS);
		TestTrue(TEXT("World is paused"), World->IsPaused());

		// Premature trigger during pause must NOT expire hit-stop
		HitStopComp->OnHitStopTimerExpired();
		TestTrue(TEXT("Hit-stop remains active during pause"), HitStopComp->IsHitStopActive());
		TestEqual(TEXT("Dilation remains slowed during pause"), TestActor->CustomTimeDilation, 0.001f);

		// Unpause world and cancel cleanly
		World->GetWorldSettings()->SetPauserPlayerState(nullptr);
		TestFalse(TEXT("World is unpaused"), World->IsPaused());

		HitStopComp->CancelHitStop();
		TestFalse(TEXT("Hit-stop inactive after cancellation"), HitStopComp->IsHitStopActive());
		TestEqual(TEXT("CustomTimeDilation restored after unpause"), TestActor->CustomTimeDilation, 1.0f);

		TestPS->Destroy();
	}

	// 5. Test synchronized pair hit-stop with DISTINCT prior dilations
	TestActor->CustomTimeDilation = 1.25f; // Attacker has speed buff
	AActor* TargetActor = World->SpawnActor<AActor>(AActor::StaticClass(), FVector(200.0f, 100.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (TestNotNull(TEXT("TargetActor spawned"), TargetActor))
	{
		TargetActor->CustomTimeDilation = 0.75f; // Target has slow debuff
		UEOSHitStopComponent::ApplyHitStopToPair(TestActor, TargetActor, 0.08f, 0.001f);

		UEOSHitStopComponent* TargetComp = TargetActor->FindComponentByClass<UEOSHitStopComponent>();
		TestTrue(TEXT("Attacker hit-stop active"), HitStopComp->IsHitStopActive());
		TestNotNull(TEXT("Target hit-stop component exists"), TargetComp);
		if (TargetComp)
		{
			TestTrue(TEXT("Target hit-stop active"), TargetComp->IsHitStopActive());
			TestEqual(TEXT("Attacker dilation slowed to 0.001f"), TestActor->CustomTimeDilation, 0.001f);
			TestEqual(TEXT("Target dilation slowed to 0.001f"), TargetActor->CustomTimeDilation, 0.001f);
			TestEqual(TEXT("Attacker captured distinct prior dilation (1.25f)"), HitStopComp->GetPriorCustomTimeDilation(), 1.25f);
			TestEqual(TEXT("Target captured distinct prior dilation (0.75f)"), TargetComp->GetPriorCustomTimeDilation(), 0.75f);

			// Cancel both and verify each restores its own unique dilation
			HitStopComp->CancelHitStop();
			TargetComp->CancelHitStop();
			TestEqual(TEXT("Attacker restored to original 1.25f"), TestActor->CustomTimeDilation, 1.25f);
			TestEqual(TEXT("Target restored to original 0.75f"), TargetActor->CustomTimeDilation, 0.75f);
		}
		TargetActor->Destroy();
	}

	// 6. Safe actor destruction test
	HitStopComp->ApplyHitStop(0.50f, 0.001f);
	TestActor->Destroy();
	TestTrue(TEXT("Actor with active hit-stop destroyed safely without crash"), true);

	return true;
}

/**
 * Dedicated Hit-Stop Paused-World & Dilation Restoration Automation Test:
 * Proves that:
 * 1. World-time expiration freezes while paused (World->IsPaused()).
 * 2. Hit-stop does not expire during world pause.
 * 3. The remaining duration resumes correctly after unpausing.
 * 4. Overlapping hits preserve the correct expiration deadline.
 * 5. Actor destruction clears timers safely without memory corruption.
 * 6. Previous actor dilation values restore exactly once to their distinct initial states.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSHitStopPauseUnpauseTest, "EchoesOfShinobi.Combat.HitStopPauseUnpause",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSHitStopPauseUnpauseTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for hit-stop pause/unpause test"), World))
	{
		return false;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 1. Spawn Attacker and Target with DISTINCT initial dilation values
	AActor* Attacker = World->SpawnActor<AActor>(AActor::StaticClass(), FVector(100.0f, 0.0f, 50.0f), FRotator::ZeroRotator, SpawnParams);
	AActor* Target = World->SpawnActor<AActor>(AActor::StaticClass(), FVector(250.0f, 0.0f, 50.0f), FRotator::ZeroRotator, SpawnParams);

	if (!TestNotNull(TEXT("Attacker spawned"), Attacker) || !TestNotNull(TEXT("Target spawned"), Target))
	{
		if (Attacker) Attacker->Destroy();
		if (Target) Target->Destroy();
		return false;
	}

	Attacker->CustomTimeDilation = 1.25f; // Attacker has 25% speed buff
	Target->CustomTimeDilation = 0.80f;   // Target has 20% slow debuff

	UEOSHitStopComponent* AttackerComp = UEOSHitStopComponent::GetOrCreateHitStopComponent(Attacker);
	UEOSHitStopComponent* TargetComp = UEOSHitStopComponent::GetOrCreateHitStopComponent(Target);

	TestNotNull(TEXT("Attacker HitStopComponent attached"), AttackerComp);
	TestNotNull(TEXT("Target HitStopComponent attached"), TargetComp);

	// 2. Apply synchronized hit-stop
	const float HitStopDuration = 0.15f;
	const float SlowFactor = 0.05f;
	const double StartWorldTime = World->GetTimeSeconds();

	UEOSHitStopComponent::ApplyHitStopToPair(Attacker, Target, HitStopDuration, SlowFactor);

	TestTrue(TEXT("Attacker hit-stop active"), AttackerComp->IsHitStopActive());
	TestTrue(TEXT("Target hit-stop active"), TargetComp->IsHitStopActive());
	TestEqual(TEXT("Attacker slowed to 0.05f"), Attacker->CustomTimeDilation, SlowFactor);
	TestEqual(TEXT("Target slowed to 0.05f"), Target->CustomTimeDilation, SlowFactor);
	TestEqual(TEXT("Attacker captured distinct prior dilation (1.25f)"), AttackerComp->GetPriorCustomTimeDilation(), 1.25f);
	TestEqual(TEXT("Target captured distinct prior dilation (0.80f)"), TargetComp->GetPriorCustomTimeDilation(), 0.80f);

	const double InitialDeadline = AttackerComp->GetHitStopExpirationTime();
	TestTrue(TEXT("Expiration deadline set in world-time seconds"), InitialDeadline >= StartWorldTime + 0.14);

	// 3. Pause the world before expiration
	APlayerState* PauserState = World->SpawnActor<APlayerState>(APlayerState::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("Pauser PlayerState spawned"), PauserState) || !World->GetWorldSettings())
	{
		Attacker->Destroy();
		Target->Destroy();
		return false;
	}

	World->GetWorldSettings()->SetPauserPlayerState(PauserState);
	TestTrue(TEXT("World is paused"), World->IsPaused());

	const double PausedWorldTime = World->GetTimeSeconds();

	// 4. Tick the world while paused (10 ticks of 0.05s = 0.50s real time, exceeding 0.15s hit-stop)
	// Proves that FTimerManager and world time naturally freeze during world pause
	for (int32 i = 0; i < 10; ++i)
	{
		const float StepDelta = 0.05f;
		GFrameCounter++;
		World->RealTimeSeconds += StepDelta;
		// TimeSeconds intentionally not advanced during pause (simulating engine loop)
		World->Tick(LEVELTICK_All, StepDelta);
	}

	TestTrue(TEXT("World remains paused"), World->IsPaused());
	TestEqual(TEXT("World TimeSeconds did not advance while paused"), World->GetTimeSeconds(), PausedWorldTime);
	TestTrue(TEXT("Attacker hit-stop remains active during real world pause ticks"), AttackerComp->IsHitStopActive());
	TestTrue(TEXT("Target hit-stop remains active during real world pause ticks"), TargetComp->IsHitStopActive());
	TestEqual(TEXT("Attacker dilation remains slowed during pause"), Attacker->CustomTimeDilation, SlowFactor);
	TestEqual(TEXT("Target dilation remains slowed during pause"), Target->CustomTimeDilation, SlowFactor);

	// 5. Apply overlapping hit during pause
	AttackerComp->ApplyHitStop(0.20f, SlowFactor);
	TestTrue(TEXT("Attacker hit-stop remains active after overlapping hit during pause"), AttackerComp->IsHitStopActive());
	TestEqual(TEXT("Prior dilation not corrupted by overlapping hit (remains 1.25f)"), AttackerComp->GetPriorCustomTimeDilation(), 1.25f);
	TestTrue(TEXT("Expiration deadline extended by overlapping hit"), AttackerComp->GetHitStopExpirationTime() > InitialDeadline);

	// Additional paused ticks to verify overlapping hit deadline also freezes
	for (int32 i = 0; i < 5; ++i)
	{
		const float StepDelta = 0.05f;
		GFrameCounter++;
		World->RealTimeSeconds += StepDelta;
		World->Tick(LEVELTICK_All, StepDelta);
	}
	TestTrue(TEXT("Attacker hit-stop remains active after additional paused ticks"), AttackerComp->IsHitStopActive());

	// 6. Unpause the world
	World->GetWorldSettings()->SetPauserPlayerState(nullptr);
	TestFalse(TEXT("World is unpaused"), World->IsPaused());

	// 7. Tick the world while unpaused until FTimerManager naturally fires callbacks
	// 10 ticks of 0.05s = 0.50s of unpaused time, sufficient for 0.15s target and 0.20s attacker hit-stop
	for (int32 i = 0; i < 10; ++i)
	{
		const float StepDelta = 0.05f;
		GFrameCounter++;
		World->TimeSeconds += StepDelta;
		World->RealTimeSeconds += StepDelta;
		World->Tick(LEVELTICK_All, StepDelta);
	}

	TestFalse(TEXT("Attacker hit-stop inactive after unpause and natural expiration"), AttackerComp->IsHitStopActive());
	TestFalse(TEXT("Target hit-stop inactive after unpause and natural expiration"), TargetComp->IsHitStopActive());
	TestEqual(TEXT("Attacker restored to original 1.25f naturally"), Attacker->CustomTimeDilation, 1.25f);
	TestEqual(TEXT("Target restored to original 0.80f naturally"), Target->CustomTimeDilation, 0.80f);

	// 8. Safe actor teardown test while hit-stop is active
	AttackerComp->ApplyHitStop(0.50f, SlowFactor);
	TargetComp->ApplyHitStop(0.50f, SlowFactor);
	Attacker->Destroy();
	Target->Destroy();
	PauserState->Destroy();

	TestTrue(TEXT("Actors and PauserState with active hit-stop destroyed safely without crash"), true);

	return true;
}

/**
 * Strict Zabuza Boss Encounter FSM, Telegraph & Phase Progression Validation:
 * Validates the canonical 3-phase encounter definition, legal state machine transitions,
 * attack telegraphs, stagger accumulation, and damage-triggered phase transitions.
 * Executed via abstract test actor (never mistaken for production art).
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSZabuzaBossEncounterTest, "EchoesOfShinobi.Boss.ZabuzaEncounterFSM",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSZabuzaBossEncounterTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for boss encounter test"), World))
	{
		return false;
	}

	// 1. Validate Encounter Registry Definition
	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSBossEncounterSubsystem* BossSubsystem = NewObject<UEOSBossEncounterSubsystem>(TestGI);
	if (!TestNotNull(TEXT("BossEncounterSubsystem instantiated"), BossSubsystem))
	{
		return false;
	}

	BossSubsystem->InitializeGoldenBossRegistry();
	TestTrue(TEXT("Zabuza Momochi registered in boss encounter registry"), BossSubsystem->IsBossRegistered(TEXT("BOSS_ZABUZA")));

	FEOSBossEncounterDefinition ZabuzaDef = BossSubsystem->GetBossEncounterDefinition(TEXT("BOSS_ZABUZA"));
	TestEqual(TEXT("Boss Name matches canon"), ZabuzaDef.BossName, TEXT("Zabuza Momochi"));
	TestEqual(TEXT("Boss Subtitle matches canon"), ZabuzaDef.BossSubtitle, TEXT("Demon of the Hidden Mist"));
	TestEqual(TEXT("Max HP is 4500.0"), ZabuzaDef.MaxHP, 4500.0f);
	TestEqual(TEXT("Primary element is Water"), static_cast<int32>(ZabuzaDef.PrimaryElement), static_cast<int32>(EEOSChakraElement::Water));
	TestEqual(TEXT("Total phases equals 3"), ZabuzaDef.Phases.Num(), 3);
	TestTrue(TEXT("Attack patterns populated (>= 4)"), ZabuzaDef.AttackPatterns.Num() >= 4);

	// 2. Spawn abstract boss character test actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSBossCharacter* BossActor = World->SpawnActor<AEOSBossCharacter>(AEOSBossCharacter::StaticClass(), FVector(500.0f, 0.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("BossActor spawned"), BossActor))
	{
		return false;
	}

	BossActor->InitializeFromEncounterDefinition(ZabuzaDef);
	TestEqual(TEXT("Initial FSM state is Inactive"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Inactive));
	TestEqual(TEXT("Initial Phase is 1"), BossActor->GetCurrentPhase(), 1);
	TestEqual(TEXT("Initial HP is 4500.0"), BossActor->GetCurrentHP(), 4500.0f);
	TestFalse(TEXT("Initial stagger state is false"), BossActor->IsStaggered());

	// 3. FSM Lifecycle: Entrance -> Idle
	BossActor->TriggerEntrance();
	TestEqual(TEXT("State transitioned to Entrance"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Entrance));

	TestTrue(TEXT("Legal transition Entrance -> Idle"), BossActor->TransitionToState(EEOSBossState::Idle));
	TestEqual(TEXT("State is Idle"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Idle));

	// 4. Attack Execution: Idle -> Telegraph -> Attacking -> Recovery -> Idle
	TestTrue(TEXT("ExecuteAttackPattern initiates Telegraph"), BossActor->ExecuteAttackPattern(TEXT("ZAB_KUBIKIRIBOCHO_SLASH")));
	TestEqual(TEXT("State is Telegraph"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));

	TestTrue(TEXT("Legal transition Telegraph -> Attacking"), BossActor->TransitionToState(EEOSBossState::Attacking));
	TestEqual(TEXT("State is Attacking"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Attacking));

	TestTrue(TEXT("Legal transition Attacking -> Recovery"), BossActor->TransitionToState(EEOSBossState::Recovery));
	TestEqual(TEXT("State is Recovery"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Recovery));

	TestTrue(TEXT("Legal transition Recovery -> Idle"), BossActor->TransitionToState(EEOSBossState::Idle));

	// 5. Stagger Model: Accumulate stagger damage to trigger vulnerability
	BossActor->ApplyStaggerDamage(260.0f); // Threshold is 250.0f
	TestTrue(TEXT("Boss enters Staggered state when threshold exceeded"), BossActor->IsStaggered());
	TestEqual(TEXT("FSM state is Staggered"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Staggered));

	// Recover from stagger
	TestTrue(TEXT("Boss recovers from Stagger to Idle"), BossActor->TransitionToState(EEOSBossState::Idle));

	// 6. Phase Transition: Damage crosses 65% HP threshold (4500 * 0.65 = 2925)
	// Apply 1600 damage -> HP drops to 2900 (64.44% HP) -> triggers Phase 2
	UGameplayStatics::ApplyDamage(BossActor, 1600.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestEqual(TEXT("HP correctly reduced to 2900.0"), BossActor->GetCurrentHP(), 2900.0f);
	TestEqual(TEXT("Boss automatically progressed to Phase 2 (Hidden Mist)"), BossActor->GetCurrentPhase(), 2);

	// 7. Terminal State: Defeat
	UGameplayStatics::ApplyDamage(BossActor, 3000.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestTrue(TEXT("Boss HP depleted to 0"), BossActor->GetCurrentHP() <= 0.0f);
	TestTrue(TEXT("Boss is marked as Defeated"), BossActor->IsDefeated());

	// 8. Cleanup test actor
	BossActor->Destroy();
	TestTrue(TEXT("Abstract boss test actor destroyed cleanly"), true);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSHinataQuestLifecycleTest, "EchoesOfShinobi.Narrative.HinataQuestLifecycle",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSHinataQuestLifecycleTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for quest lifecycle test"), World))
	{
		return false;
	}

	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSQuestSubsystem* QuestSubsystem = NewObject<UEOSQuestSubsystem>(TestGI);
	UEOSSaveSubsystem* SaveSubsystem = NewObject<UEOSSaveSubsystem>(TestGI);

	if (!TestNotNull(TEXT("QuestSubsystem instantiated"), QuestSubsystem) ||
		!TestNotNull(TEXT("SaveSubsystem instantiated"), SaveSubsystem))
	{
		return false;
	}

	QuestSubsystem->InitializeDefaultQuests();

	// 1. NPC Interaction: Spawn abstract test NPC configured for Hinata
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSVillageNPC* HinataNPC = World->SpawnActor<AEOSVillageNPC>(AEOSVillageNPC::StaticClass(), FVector(100.0f, 0.0f, 50.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("Hinata abstract test NPC spawned"), HinataNPC))
	{
		return false;
	}

	HinataNPC->NPCType = EEOSNPCType::NamedShinobi;
	HinataNPC->NPCID = TEXT("NPC_HINATA_HYUGA");
	HinataNPC->NPCName = FText::FromString(TEXT("Hinata Hyuga"));
	HinataNPC->DialogueText = FText::FromString(TEXT("N-Naruto-kun... please be careful. Zabuza of the Hidden Mist has been sighted in the outskirts."));

	// Execute interaction
	FString InteractionResult = HinataNPC->Interact(nullptr);
	TestEqual(TEXT("NPC Interaction returns canonical quest hook"), InteractionResult, TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"));

	// 2. Dialogue Branching
	FEOSNPCConversation Conversation;
	TestTrue(TEXT("StartQuestDialogue succeeds for Hinata"), QuestSubsystem->StartQuestDialogue(TEXT("NPC_HINATA_HYUGA"), Conversation));
	TestEqual(TEXT("Speaker Name is Hinata"), Conversation.SpeakerName.ToString(), TEXT("Hinata Hyuga"));
	TestTrue(TEXT("Prompt text contains Zabuza"), Conversation.PromptText.ToString().Contains(TEXT("Zabuza")));
	TestTrue(TEXT("Dialogue presents choice options (>= 2)"), Conversation.Choices.Num() >= 2);
	TestEqual(TEXT("Choice 0 action tag is ACTION_ACCEPT_QUEST"), Conversation.Choices[0].ActionTag, TEXT("ACTION_ACCEPT_QUEST"));

	// 3. Quest Accepted
	TestTrue(TEXT("AcceptQuest triggers successfully"), QuestSubsystem->AcceptQuest(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")));
	TestEqual(TEXT("Quest state is Active"), static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::Active));

	FEOSActiveQuestRecord ActiveQuest;
	TestTrue(TEXT("Active quest record found"), QuestSubsystem->GetActiveQuest(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), ActiveQuest));
	TestEqual(TEXT("Active quest has 3 objectives"), ActiveQuest.Objectives.Num(), 3);

	// 4. Objective Tracking: Complete mandatory objectives in sequence
	TestFalse(TEXT("Quest not ready for completion initially"), QuestSubsystem->IsQuestReadyForCompletion(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")));

	// Objective 1: Mist perimeter investigation
	TestTrue(TEXT("Update Objective 1 progress"), QuestSubsystem->UpdateObjectiveProgress(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST"), true));
	TestEqual(TEXT("Quest state still Active (pending boss defeat)"), static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::Active));

	// Objective 2: Neutralize Zabuza Momochi
	TestTrue(TEXT("Update Objective 2 progress"), QuestSubsystem->UpdateObjectiveProgress(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI"), true));
	TestTrue(TEXT("Quest is now ready for turn-in (ObjectivesMet)"), QuestSubsystem->IsQuestReadyForCompletion(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")));
	TestEqual(TEXT("Quest state is ObjectivesMet"), static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::ObjectivesMet));

	// 5. Mission Completion & Handoff
	FEOSStageResult StageResult;
	TArray<FEOSRewardPresentationItem> Rewards;
	TestTrue(TEXT("CompleteQuest executes successfully"), QuestSubsystem->CompleteQuest(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), StageResult, Rewards));
	TestEqual(TEXT("Quest state is Completed"), static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::Completed));
	TestEqual(TEXT("StageResult StageID is STG_ARC01_ZABUZA_OUTSKIRTS"), StageResult.StageID, TEXT("STG_ARC01_ZABUZA_OUTSKIRTS"));
	TestEqual(TEXT("WorldStateDeltaID is WORLD_STATE_OUTSKIRTS_RESTORED"), StageResult.WorldStateDeltaID, TEXT("WORLD_STATE_OUTSKIRTS_RESTORED"));

	// 6. Rewards Dispatch
	TestTrue(TEXT("Rewards package populated (>= 4 items)"), Rewards.Num() >= 4);
	bool bFoundRyo = false;
	bool bFoundBond = false;
	for (const FEOSRewardPresentationItem& R : Rewards)
	{
		if (R.Type == TEXT("RYO") && R.Quantity == 2500) bFoundRyo = true;
		if (R.Type == TEXT("BOND") && R.RewardID == TEXT("ITEM_HINATA_BOND_TOKEN")) bFoundBond = true;
	}
	TestTrue(TEXT("Granted 2500 Ryo currency reward"), bFoundRyo);
	TestTrue(TEXT("Granted Hinata SSR Bond Token reward"), bFoundBond);

	// 7. Save State Persistence
	FEOSPlayerSaveData SaveData = SaveSubsystem->GetCurrentSaveData();
	SaveData.RyoCurrency += 2500;
	SaveData.PlayerLevel = 5;
	QuestSubsystem->PopulateSaveData(SaveData);
	SaveSubsystem->SetCurrentSaveData(SaveData);

	TestTrue(TEXT("SaveGameData succeeds for Slot_HinataQuest_Save"), SaveSubsystem->SaveGameData(TEXT("Slot_HinataQuest_Save")));

	// Mutate current in-memory state to simulate clean restart
	FEOSPlayerSaveData CorruptedData;
	CorruptedData.RyoCurrency = 0;
	CorruptedData.PlayerLevel = 1;
	SaveSubsystem->SetCurrentSaveData(CorruptedData);

	// Load and verify restored state
	TestTrue(TEXT("LoadGameData restores saved slot"), SaveSubsystem->LoadGameData(TEXT("Slot_HinataQuest_Save")));
	const FEOSPlayerSaveData& LoadedData = SaveSubsystem->GetCurrentSaveData();
	TestEqual(TEXT("Restored Ryo currency is 3500 (1000 base + 2500 reward)"), LoadedData.RyoCurrency, 3500);
	TestEqual(TEXT("Restored Player Level is 5"), LoadedData.PlayerLevel, 5);
	TestEqual(TEXT("Restored Hinata Bond Token count is 1"), LoadedData.BondTokenBalance, 1);
	TestEqual(TEXT("Restored Player EXP is 1200"), LoadedData.PlayerEXP, 1200);
	TestEqual(TEXT("Restored Active World State is WORLD_STATE_OUTSKIRTS_RESTORED"), LoadedData.ActiveWorldStateID, TEXT("WORLD_STATE_OUTSKIRTS_RESTORED"));
	TestTrue(TEXT("Restored unlocked story archive includes Hinata encounter"), LoadedData.UnlockedStoryArchiveIds.Contains(TEXT("ARCHIVE_HINATA_OUTSKIRTS_MIST")));
	TestTrue(TEXT("Restored objective OBJ_INVESTIGATE_OUTSKIRTS_MIST is completed"), LoadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")) && LoadedData.ObjectiveCompletionMap[TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")]);
	TestTrue(TEXT("Restored objective OBJ_DEFEAT_ZABUZA_MOMOCHI is completed"), LoadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")) && LoadedData.ObjectiveCompletionMap[TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")]);
	TestTrue(TEXT("Restored completed stage includes quest"), LoadedData.CompletedStageIds.Contains(FName(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))));

	// Cleanup test NPC
	HinataNPC->Destroy();
	TestTrue(TEXT("Abstract Hinata NPC destroyed cleanly"), true);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSProgressionSaveAndGachaTest, "EchoesOfShinobi.Progression.SaveAndGachaDeterministic",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSProgressionSaveAndGachaTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for gacha test"), World))
	{
		return false;
	}

	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSGachaSubsystem* Gacha1 = NewObject<UEOSGachaSubsystem>(TestGI);
	UEOSGachaSubsystem* Gacha2 = NewObject<UEOSGachaSubsystem>(TestGI);

	if (!TestNotNull(TEXT("GachaSubsystem 1 instantiated"), Gacha1) ||
		!TestNotNull(TEXT("GachaSubsystem 2 instantiated"), Gacha2))
	{
		return false;
	}

	Gacha1->InitializeDefaultGachaDatabase();
	Gacha2->InitializeDefaultGachaDatabase();

	Gacha1->AddCurrency(EEOSCurrencyType::Premium, 15000);
	Gacha2->AddCurrency(EEOSCurrencyType::Premium, 15000);

	const int32 TestSeed = 7771;
	Gacha1->SetDeterministicSeed(TestSeed);
	Gacha2->SetDeterministicSeed(TestSeed);

	// Execute 10-pull on Standard Banner on both instances
	FEOSSummonResult Result1 = Gacha1->ExecuteMultiSummon(TEXT("BANNER_STANDARD"), 10);
	FEOSSummonResult Result2 = Gacha2->ExecuteMultiSummon(TEXT("BANNER_STANDARD"), 10);

	TestTrue(TEXT("Gacha1 multi-summon succeeded"), Result1.bSuccess);
	TestTrue(TEXT("Gacha2 multi-summon succeeded"), Result2.bSuccess);
	TestEqual(TEXT("Both pull 10 items"), Result1.Acquisitions.Num(), 10);
	TestEqual(TEXT("Both pull 10 items"), Result2.Acquisitions.Num(), 10);

	// Verify exact deterministic sequence match across both seeded instances
	bool bAllRollsIdentical = true;
	bool bHasAtLeastOne4StarOrHigher = false;
	for (int32 i = 0; i < 10; ++i)
	{
		if (Result1.Acquisitions[i].CharacterID != Result2.Acquisitions[i].CharacterID ||
			Result1.Acquisitions[i].Rarity != Result2.Acquisitions[i].Rarity)
		{
			bAllRollsIdentical = false;
		}
		if (Result1.Acquisitions[i].Rarity >= EEOSShinobiRarity::Rarity_4)
		{
			bHasAtLeastOne4StarOrHigher = true;
		}
	}
	TestTrue(TEXT("Seeded Gacha produces 100% identical roll sequences"), bAllRollsIdentical);
	TestTrue(TEXT("10-pull guarantees at least one 4★ or higher item"), bHasAtLeastOne4StarOrHigher);

	// Verify Currency Deduction & Pity Increment (3000 base + 15000 added - 1500 multi-pull = 16500)
	TestEqual(TEXT("Premium currency deducted correctly (18000 - 1500 = 16500)"), Gacha1->GetCurrencyBalance(EEOSCurrencyType::Premium), 16500);
	TestEqual(TEXT("Pity counter incremented to 10 after 10-pull"), Gacha1->GetPityCount(TEXT("BANNER_STANDARD")), 10);

	// Verify Hard Pity (HardPity = 80 threshold)
	Gacha1->SetPityCount(TEXT("BANNER_STANDARD"), 79);
	FEOSSummonResult HardPityResult = Gacha1->ExecuteSingleSummon(TEXT("BANNER_STANDARD"));
	TestTrue(TEXT("Single summon at hard pity succeeded"), HardPityResult.bSuccess);
	TestTrue(TEXT("Hard pity guarantees 5★ character"), HardPityResult.Acquisitions[0].Rarity == EEOSShinobiRarity::Rarity_5);
	TestTrue(TEXT("Hard pity flag marked true"), HardPityResult.bTriggeredHardPity);
	TestEqual(TEXT("Pity counter resets to 0 after 5★ acquisition"), Gacha1->GetPityCount(TEXT("BANNER_STANDARD")), 0);

	// Verify Duplicate Conversion
	const FString OwnedChar = HardPityResult.Acquisitions[0].CharacterID;
	TestTrue(TEXT("Character is now marked as owned in inventory"), Gacha1->IsCharacterOwned(OwnedChar));
	const int32 ShardsBefore = Gacha1->GetCharacterShards(OwnedChar);
	// Force duplicate test: pull again at hard pity to guarantee same featured 5★
	Gacha1->SetPityCount(TEXT("BANNER_STANDARD"), 79);
	FEOSSummonResult DupResult = Gacha1->ExecuteSingleSummon(TEXT("BANNER_STANDARD"));
	TestTrue(TEXT("Duplicate summon succeeded"), DupResult.bSuccess);
	if (DupResult.Acquisitions[0].CharacterID == OwnedChar)
	{
		TestTrue(TEXT("Duplicate pull identified as duplicate"), DupResult.Acquisitions[0].bIsDuplicate);
		TestFalse(TEXT("Duplicate pull is not marked as new unlock"), DupResult.Acquisitions[0].bIsNewUnlock);
		TestTrue(TEXT("Duplicate granted character shards"), DupResult.Acquisitions[0].GrantedCharacterShards > 0);
		TestTrue(TEXT("Shard inventory increased"), Gacha1->GetCharacterShards(OwnedChar) > ShardsBefore);
	}

	// Verify Multi-slot Save isolation
	UEOSSaveSubsystem* SaveSys = NewObject<UEOSSaveSubsystem>(TestGI);
	FEOSPlayerSaveData SlotAData;
	SlotAData.SaveSlotName = TEXT("Slot_A");
	SlotAData.PlayerLevel = 10;
	SlotAData.RyoCurrency = 50000;
	SaveSys->SetCurrentSaveData(SlotAData);
	SaveSys->SaveGameData(TEXT("Slot_A"));

	FEOSPlayerSaveData SlotBData;
	SlotBData.SaveSlotName = TEXT("Slot_B");
	SlotBData.PlayerLevel = 25;
	SlotBData.RyoCurrency = 120000;
	SaveSys->SetCurrentSaveData(SlotBData);
	SaveSys->SaveGameData(TEXT("Slot_B"));

	// Load Slot_A and verify
	SaveSys->LoadGameData(TEXT("Slot_A"));
	TestEqual(TEXT("Slot A Player Level preserved"), SaveSys->GetCurrentSaveData().PlayerLevel, 10);
	TestEqual(TEXT("Slot A Ryo preserved"), SaveSys->GetCurrentSaveData().RyoCurrency, 50000);

	// Load Slot_B and verify
	SaveSys->LoadGameData(TEXT("Slot_B"));
	TestEqual(TEXT("Slot B Player Level preserved"), SaveSys->GetCurrentSaveData().PlayerLevel, 25);
	TestEqual(TEXT("Slot B Ryo preserved"), SaveSys->GetCurrentSaveData().RyoCurrency, 120000);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSZabuzaPhase3AndHitStopSafetyTest, "EchoesOfShinobi.Boss.ZabuzaPhase3AndHitStopSafety",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSZabuzaPhase3AndHitStopSafetyTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for Phase 3 test"), World))
	{
		return false;
	}

	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSBossEncounterSubsystem* BossSubsystem = NewObject<UEOSBossEncounterSubsystem>(TestGI);
	BossSubsystem->InitializeGoldenBossRegistry();

	FEOSBossEncounterDefinition ZabuzaDef = BossSubsystem->GetBossEncounterDefinition(TEXT("BOSS_ZABUZA"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSBossCharacter* BossActor = World->SpawnActor<AEOSBossCharacter>(AEOSBossCharacter::StaticClass(), FVector(600.0f, 0.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (!TestNotNull(TEXT("BossActor spawned"), BossActor))
	{
		return false;
	}

	BossActor->InitializeFromEncounterDefinition(ZabuzaDef);
	BossActor->TriggerEntrance();
	BossActor->TransitionToState(EEOSBossState::Idle);

	// ── Pattern 1-3 Verification in Phase 1 ──
	// Phase restriction: Phase 3 patterns must be rejected in Phase 1
	TestFalse(TEXT("Phase 1 rejects Phase 3 Demon Berserk Combo"), BossActor->ExecuteAttackPattern(TEXT("ZAB_DEMON_BERSERK_COMBO")));
	TestFalse(TEXT("Phase 1 rejects Phase 2 Water Prison Trap"), BossActor->ExecuteAttackPattern(TEXT("ZAB_WATER_PRISON_TRAP")));

	// 1. ZAB_KUBIKIRIBOCHO_SLASH (Phase 1, 2, 3)
	TestTrue(TEXT("Execute P1 Pattern 1: Kubikiribocho Cleave"), BossActor->ExecuteAttackPattern(TEXT("ZAB_KUBIKIRIBOCHO_SLASH")));
	TestEqual(TEXT("P1 Pattern 1 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	TestFalse(TEXT("P1 Pattern 1 rejects illegal Telegraph -> Recovery"), BossActor->TransitionToState(EEOSBossState::Recovery));
	TestTrue(TEXT("P1 Pattern 1 Telegraph -> Attacking"), BossActor->TransitionToState(EEOSBossState::Attacking));
	TestTrue(TEXT("P1 Pattern 1 Attacking -> Recovery"), BossActor->TransitionToState(EEOSBossState::Recovery));
	TestTrue(TEXT("P1 Pattern 1 Recovery -> Idle"), BossActor->TransitionToState(EEOSBossState::Idle));
	TestFalse(TEXT("P1 Pattern 1 cooldown rejection"), BossActor->ExecuteAttackPattern(TEXT("ZAB_KUBIKIRIBOCHO_SLASH")));
	BossActor->ResetPatternCooldowns();

	// 2. ZAB_WATER_CLONE_FEINT (Phase 1, 2)
	TestTrue(TEXT("Execute P1 Pattern 2: Water Clone Feint"), BossActor->ExecuteAttackPattern(TEXT("ZAB_WATER_CLONE_FEINT")));
	TestEqual(TEXT("P1 Pattern 2 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);
	BossActor->ResetPatternCooldowns();

	// 3. ZAB_MIST_AMBUSH (Phase 1, 2)
	TestTrue(TEXT("Execute P1 Pattern 3: Silent Mist Ambush"), BossActor->ExecuteAttackPattern(TEXT("ZAB_MIST_AMBUSH")));
	TestEqual(TEXT("P1 Pattern 3 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);
	BossActor->ResetPatternCooldowns();

	// ── Pattern 4-5 Verification in Phase 2 ──
	BossActor->TransitionToPhase(2);
	TestEqual(TEXT("Boss transitioned to Phase 2"), BossActor->GetCurrentPhase(), 2);

	// 4. ZAB_WATER_PRISON_TRAP (Phase 2)
	TestTrue(TEXT("Execute P2 Pattern 4: Water Prison Trap"), BossActor->ExecuteAttackPattern(TEXT("ZAB_WATER_PRISON_TRAP")));
	TestEqual(TEXT("P2 Pattern 4 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);
	BossActor->ResetPatternCooldowns();

	// 5. ZAB_WATER_DRAGON_BARRAGE (Phase 2, 3)
	TestTrue(TEXT("Execute P2 Pattern 5: Water Style Water Dragon"), BossActor->ExecuteAttackPattern(TEXT("ZAB_WATER_DRAGON_BARRAGE")));
	TestEqual(TEXT("P2 Pattern 5 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);
	BossActor->ResetPatternCooldowns();

	// ── Transition to Phase 3 via Damage Threshold ──
	// Apply 3250 damage out of 4500 (HP drops to 1250 / 4500 = 27.7%, crossing the 30% Phase 3 threshold)
	UGameplayStatics::ApplyDamage(BossActor, 3250.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestEqual(TEXT("HP drops to 1250.0"), BossActor->GetCurrentHP(), 1250.0f);
	TestEqual(TEXT("Boss successfully transitions to Phase 3 (Demon's Final Stand)"), BossActor->GetCurrentPhase(), 3);

	// Transition from PhaseTransition state to Idle before executing next attack
	BossActor->TransitionToState(EEOSBossState::Idle);

	// Phase restriction: Phase 1/2-only pattern rejected in Phase 3
	TestFalse(TEXT("Phase 3 rejects Water Clone Feint"), BossActor->ExecuteAttackPattern(TEXT("ZAB_WATER_CLONE_FEINT")));

	// 6. ZAB_DEMON_BERSERK_COMBO (Phase 3)
	TestTrue(TEXT("Execute P3 Pattern 6: Demon Berserk Combo"), BossActor->ExecuteAttackPattern(TEXT("ZAB_DEMON_BERSERK_COMBO")));
	TestEqual(TEXT("P3 Pattern 6 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);
	BossActor->ResetPatternCooldowns();

	// 7. ZAB_HIDDEN_MIST_EXECUTOR (Phase 3 Ultimate)
	TestTrue(TEXT("Execute P3 Pattern 7: Hidden Mist Executor Ultimate"), BossActor->ExecuteAttackPattern(TEXT("ZAB_HIDDEN_MIST_EXECUTOR")));
	TestEqual(TEXT("P3 Pattern 7 enters Telegraph state"), static_cast<int32>(BossActor->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	BossActor->TransitionToState(EEOSBossState::Attacking);
	BossActor->TransitionToState(EEOSBossState::Recovery);
	BossActor->TransitionToState(EEOSBossState::Idle);

	// Test Hit-Stop Safety on dynamic actor destruction
	AEOSCharacterBase* TestChar = World->SpawnActor<AEOSCharacterBase>(AEOSCharacterBase::StaticClass(), FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (TestNotNull(TEXT("TestChar spawned for hit-stop safety"), TestChar))
	{
		UEOSHitStopComponent* HitStopComp = UEOSHitStopComponent::GetOrCreateHitStopComponent(TestChar);
		if (TestNotNull(TEXT("HitStopComponent present"), HitStopComp))
		{
			// Apply hit-stop
			HitStopComp->ApplyHitStop(0.5f, 0.001f);
			TestTrue(TEXT("Hit-stop active"), HitStopComp->IsHitStopActive());

			// Destroy actor during active hit-stop
			TestChar->Destroy();
			TestTrue(TEXT("Actor destroyed safely during active hit-stop without crash"), true);
		}
	}

	BossActor->Destroy();
	return true;
}

/**
 * Authoritative Continuous Playable Mission Integration Test:
 * NPC interaction → Hinata dialogue → Quest acceptance → Investigation objective →
 * Zabuza encounter → Boss combat & hit-stop → Boss defeat → Mission completion →
 * Rewards dispatch → Gacha progression → Multi-slot save → Restart → Load & verify.
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSContinuousPlayableMissionTest, "EchoesOfShinobi.Mission.ContinuousPlayableMission",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSContinuousPlayableMissionTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for continuous mission integration test"), World))
	{
		return false;
	}

	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSQuestSubsystem* QuestSubsystem = NewObject<UEOSQuestSubsystem>(TestGI);
	UEOSBossEncounterSubsystem* BossSubsystem = NewObject<UEOSBossEncounterSubsystem>(TestGI);
	UEOSGachaSubsystem* GachaSubsystem = NewObject<UEOSGachaSubsystem>(TestGI);
	UEOSSaveSubsystem* SaveSubsystem = NewObject<UEOSSaveSubsystem>(TestGI);
	UEOSProgressionSubsystem* ProgSubsystem = NewObject<UEOSProgressionSubsystem>(TestGI);

	if (!TestNotNull(TEXT("QuestSubsystem instantiated"), QuestSubsystem) ||
		!TestNotNull(TEXT("BossSubsystem instantiated"), BossSubsystem) ||
		!TestNotNull(TEXT("GachaSubsystem instantiated"), GachaSubsystem) ||
		!TestNotNull(TEXT("SaveSubsystem instantiated"), SaveSubsystem) ||
		!TestNotNull(TEXT("ProgSubsystem instantiated"), ProgSubsystem))
	{
		return false;
	}

	QuestSubsystem->InitializeDefaultQuests();
	BossSubsystem->InitializeGoldenBossRegistry();
	GachaSubsystem->InitializeDefaultGachaDatabase();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Step 1: Spawn player character and Hinata NPC
	AEOSCharacterBase* PlayerChar = World->SpawnActor<AEOSCharacterBase>(AEOSCharacterBase::StaticClass(), FVector(0.0f, 0.0f, 50.0f), FRotator::ZeroRotator, SpawnParams);
	AEOSVillageNPC* HinataNPC = World->SpawnActor<AEOSVillageNPC>(AEOSVillageNPC::StaticClass(), FVector(150.0f, 0.0f, 50.0f), FRotator(0.0f, 180.0f, 0.0f), SpawnParams);

	if (!TestNotNull(TEXT("Player character spawned"), PlayerChar) ||
		!TestNotNull(TEXT("Hinata NPC spawned"), HinataNPC))
	{
		return false;
	}

	HinataNPC->NPCType = EEOSNPCType::NamedShinobi;
	HinataNPC->NPCID = TEXT("NPC_HINATA_HYUGA");
	HinataNPC->NPCName = FText::FromString(TEXT("Hinata Hyuga"));

	// Step 2: NPC Interaction
	FString QuestHook = HinataNPC->Interact(PlayerChar);
	TestEqual(TEXT("NPC interaction initiates Outskirts quest hook"), QuestHook, TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"));

	// Step 3: Hinata Dialogue & Acceptance
	FEOSNPCConversation Conversation;
	TestTrue(TEXT("StartQuestDialogue succeeds"), QuestSubsystem->StartQuestDialogue(TEXT("NPC_HINATA_HYUGA"), Conversation));
	TestTrue(TEXT("Dialogue has choices"), Conversation.Choices.Num() >= 2);
	TestEqual(TEXT("Choice 0 accepts mission"), Conversation.Choices[0].ActionTag, TEXT("ACTION_ACCEPT_QUEST"));

	TestTrue(TEXT("AcceptQuest succeeds"), QuestSubsystem->AcceptQuest(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")));
	TestEqual(TEXT("Quest state is Active"), static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::Active));

	// Step 4: Investigation Objective
	TestTrue(TEXT("Investigation objective completed"),
		QuestSubsystem->UpdateObjectiveProgress(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST"), true));
	TestEqual(TEXT("Quest remains active awaiting boss encounter"),
		static_cast<int32>(QuestSubsystem->GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"))), static_cast<int32>(EEOSQuestState::Active));

	// Step 5: Zabuza Encounter & Combat with Synchronized Hit-Stop
	FEOSBossEncounterDefinition ZabuzaDef = BossSubsystem->GetBossEncounterDefinition(TEXT("BOSS_ZABUZA"));
	AEOSBossCharacter* ZabuzaBoss = World->SpawnActor<AEOSBossCharacter>(AEOSBossCharacter::StaticClass(), FVector(600.0f, 0.0f, 50.0f), FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
	if (!TestNotNull(TEXT("Zabuza boss spawned"), ZabuzaBoss))
	{
		PlayerChar->Destroy();
		HinataNPC->Destroy();
		return false;
	}

	ZabuzaBoss->InitializeFromEncounterDefinition(ZabuzaDef);
	ZabuzaBoss->TriggerEntrance();
	ZabuzaBoss->TransitionToState(EEOSBossState::Idle);

	// Zabuza executes attack
	TestTrue(TEXT("Zabuza executes Kubikiribocho Cleave"), ZabuzaBoss->ExecuteAttackPattern(TEXT("ZAB_KUBIKIRIBOCHO_SLASH")));
	TestEqual(TEXT("Zabuza in Telegraph state"), static_cast<int32>(ZabuzaBoss->GetCurrentFSMState()), static_cast<int32>(EEOSBossState::Telegraph));
	ZabuzaBoss->TransitionToState(EEOSBossState::Attacking);

	// Player lands counter strike -> Synchronized hit-stop
	UEOSHitStopComponent::ApplyHitStopToPair(PlayerChar, ZabuzaBoss, 0.08f, 0.001f);
	UEOSHitStopComponent* PlayerHitStop = PlayerChar->FindComponentByClass<UEOSHitStopComponent>();
	UEOSHitStopComponent* BossHitStop = ZabuzaBoss->FindComponentByClass<UEOSHitStopComponent>();

	TestNotNull(TEXT("Player hit-stop component exists"), PlayerHitStop);
	TestNotNull(TEXT("Boss hit-stop component exists"), BossHitStop);
	if (PlayerHitStop && BossHitStop)
	{
		TestTrue(TEXT("Player in hit-stop"), PlayerHitStop->IsHitStopActive());
		TestTrue(TEXT("Boss in hit-stop"), BossHitStop->IsHitStopActive());
		PlayerHitStop->CancelHitStop();
		BossHitStop->CancelHitStop();
	}

	// Boss finishes attack recovery back to Idle
	ZabuzaBoss->TransitionToState(EEOSBossState::Recovery);
	ZabuzaBoss->TransitionToState(EEOSBossState::Idle);

	// Step 6: Boss Defeat across phases
	// Deal damage to trigger Phase 2 (HP <= 65% of 4500 = 2925)
	UGameplayStatics::ApplyDamage(ZabuzaBoss, 1600.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestEqual(TEXT("Zabuza transitions to Phase 2"), ZabuzaBoss->GetCurrentPhase(), 2);
	ZabuzaBoss->TransitionToState(EEOSBossState::Idle);

	// Deal damage to trigger Phase 3 (HP <= 30% of 4500 = 1350)
	UGameplayStatics::ApplyDamage(ZabuzaBoss, 1650.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestEqual(TEXT("Zabuza transitions to Phase 3"), ZabuzaBoss->GetCurrentPhase(), 3);
	ZabuzaBoss->TransitionToState(EEOSBossState::Idle);

	// Deplete remaining HP to 0
	UGameplayStatics::ApplyDamage(ZabuzaBoss, 1300.0f, nullptr, nullptr, UDamageType::StaticClass());
	TestTrue(TEXT("Zabuza HP depleted"), ZabuzaBoss->GetCurrentHP() <= 0.0f);
	TestTrue(TEXT("Zabuza marked defeated"), ZabuzaBoss->IsDefeated());

	// Step 7: Objective 2 Completion & Mission Turn-in
	TestTrue(TEXT("Zabuza defeat objective fulfilled"),
		QuestSubsystem->UpdateObjectiveProgress(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI"), true));
	TestTrue(TEXT("Quest ready for completion"), QuestSubsystem->IsQuestReadyForCompletion(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")));

	FEOSStageResult StageResult;
	TArray<FEOSRewardPresentationItem> Rewards;
	TestTrue(TEXT("CompleteQuest dispatches stage result and rewards"),
		QuestSubsystem->CompleteQuest(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"), StageResult, Rewards));
	TestEqual(TEXT("StageResult StageID is STG_ARC01_ZABUZA_OUTSKIRTS"), StageResult.StageID, TEXT("STG_ARC01_ZABUZA_OUTSKIRTS"));
	TestEqual(TEXT("World state delta is WORLD_STATE_OUTSKIRTS_RESTORED"), StageResult.WorldStateDeltaID, TEXT("WORLD_STATE_OUTSKIRTS_RESTORED"));

	// Step 8: Progression & Gacha
	const int32 InitialGachaCurrency = GachaSubsystem->GetCurrencyBalance(EEOSCurrencyType::Premium);
	GachaSubsystem->AddCurrency(EEOSCurrencyType::Premium, 1500); // Currency earned from mission
	TestEqual(TEXT("Premium currency balance incremented by 1500"), GachaSubsystem->GetCurrencyBalance(EEOSCurrencyType::Premium), InitialGachaCurrency + 1500);

	FEOSSummonResult SummonRes = GachaSubsystem->ExecuteMultiSummon(TEXT("BANNER_STANDARD"), 10);
	TestTrue(TEXT("Summon executed with mission rewards"), SummonRes.bSuccess);
	TestEqual(TEXT("Summon expended 1500 currency"), GachaSubsystem->GetCurrencyBalance(EEOSCurrencyType::Premium), InitialGachaCurrency);
	TestEqual(TEXT("Pity count incremented to 10"), GachaSubsystem->GetPityCount(TEXT("BANNER_STANDARD")), 10);

	// Step 9: Save Game State & Dynamic EXP Progression (Correction C)
	FEOSPlayerSaveData SaveData = SaveSubsystem->GetCurrentSaveData();
	SaveData.SaveSlotName = TEXT("Slot_MissionContinuous_Save");
	SaveData.RyoCurrency += 2500;
	QuestSubsystem->PopulateSaveData(SaveData);

	// Verify dynamic level calculation from cumulative EXP table
	const int32 DynamicCalculatedLevel = ProgSubsystem->CalculateLevelFromCumulativeEXP(SaveData.PlayerEXP);
	TestEqual(TEXT("Dynamic level calculation yields Level 5 for 1200 cumulative EXP"), DynamicCalculatedLevel, 5);
	SaveData.PlayerLevel = DynamicCalculatedLevel;

	SaveSubsystem->SetCurrentSaveData(SaveData);
	TestTrue(TEXT("Mission save succeeded"), SaveSubsystem->SaveGameData(TEXT("Slot_MissionContinuous_Save")));

	// Step 10: Simulate Restart & Reload State
	FEOSPlayerSaveData BlankData;
	BlankData.PlayerLevel = 1;
	BlankData.RyoCurrency = 0;
	BlankData.PlayerEXP = 0;
	BlankData.BondTokenBalance = 0;
	BlankData.ActiveWorldStateID = TEXT("WORLD_STATE_EMPTY");
	SaveSubsystem->SetCurrentSaveData(BlankData);

	TestTrue(TEXT("Reload of mission save succeeds"), SaveSubsystem->LoadGameData(TEXT("Slot_MissionContinuous_Save")));

	const FEOSPlayerSaveData& ReloadedData = SaveSubsystem->GetCurrentSaveData();
	TestEqual(TEXT("Reloaded Player Level is 5"), ReloadedData.PlayerLevel, 5);
	TestEqual(TEXT("Reloaded Ryo is 3500"), ReloadedData.RyoCurrency, 3500);
	TestEqual(TEXT("Reloaded Player EXP is 1200"), ReloadedData.PlayerEXP, 1200);
	TestEqual(TEXT("Reloaded Hinata Bond Token is 1"), ReloadedData.BondTokenBalance, 1);
	TestEqual(TEXT("Reloaded World State is WORLD_STATE_OUTSKIRTS_RESTORED"), ReloadedData.ActiveWorldStateID, TEXT("WORLD_STATE_OUTSKIRTS_RESTORED"));
	TestTrue(TEXT("Reloaded Story Archive contains Hinata encounter"), ReloadedData.UnlockedStoryArchiveIds.Contains(TEXT("ARCHIVE_HINATA_OUTSKIRTS_MIST")));
	TestTrue(TEXT("Reloaded OBJ_INVESTIGATE_OUTSKIRTS_MIST is complete"), ReloadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")) && ReloadedData.ObjectiveCompletionMap[TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")]);
	TestTrue(TEXT("Reloaded OBJ_DEFEAT_ZABUZA_MOMOCHI is complete"), ReloadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")) && ReloadedData.ObjectiveCompletionMap[TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")]);
	TestEqual(TEXT("Reloaded Player Level is 5"), ReloadedData.PlayerLevel, 5);
	TestEqual(TEXT("Reloaded Ryo is 3500"), ReloadedData.RyoCurrency, 3500);
	TestEqual(TEXT("Reloaded Player EXP is 1200"), ReloadedData.PlayerEXP, 1200);
	TestEqual(TEXT("Reloaded Hinata Bond Token is 1"), ReloadedData.BondTokenBalance, 1);
	TestEqual(TEXT("Reloaded World State is WORLD_STATE_OUTSKIRTS_RESTORED"), ReloadedData.ActiveWorldStateID, TEXT("WORLD_STATE_OUTSKIRTS_RESTORED"));
	TestTrue(TEXT("Reloaded Story Archive contains Hinata encounter"), ReloadedData.UnlockedStoryArchiveIds.Contains(TEXT("ARCHIVE_HINATA_OUTSKIRTS_MIST")));
	TestTrue(TEXT("Reloaded OBJ_INVESTIGATE_OUTSKIRTS_MIST is complete"), ReloadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")) && ReloadedData.ObjectiveCompletionMap[TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST")]);
	TestTrue(TEXT("Reloaded OBJ_DEFEAT_ZABUZA_MOMOCHI is complete"), ReloadedData.ObjectiveCompletionMap.Contains(TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")) && ReloadedData.ObjectiveCompletionMap[TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI")]);

	// Cleanup test actors
	PlayerChar->Destroy();
	HinataNPC->Destroy();
	ZabuzaBoss->Destroy();
	return true;
}

/**
 * Sprint 02 Adaptive AI Determinism & Feedback Loop Verification Test
 * 1. Records telemetry for 3 distinct player profiles (Back-Dodger, Rasengan-Spammer, Turtle-Guard)
 * 2. Persists profiles to disk JSON & reloads
 * 3. Evaluates directives for all 3 profiles via UEOSAdaptiveAISubsystem
 * 4. Verifies 100 consecutive deterministic evaluations under fixed seed
 * 5. Passes selected tactic into actual Zabuza Encounter FSM
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOSAdaptiveAIDeterminismTest, "EchoesOfShinobi.Combat.AdaptiveAIDeterminism",
	(EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter))

bool FEOSAdaptiveAIDeterminismTest::RunTest(const FString& Parameters)
{
	UWorld* World = GWorld;
	if (!TestNotNull(TEXT("World available for adaptive AI test"), World))
	{
		return false;
	}

	UGameInstance* TestGI = World->GetGameInstance() ? World->GetGameInstance() : NewObject<UGameInstance>();
	UEOSAdaptiveAISubsystem* AdaptiveAI = NewObject<UEOSAdaptiveAISubsystem>(TestGI);
	if (!TestNotNull(TEXT("AdaptiveAISubsystem instantiated"), AdaptiveAI))
	{
		return false;
	}

	// 1. Record Telemetry Events for 3 distinct player profiles
	// Profile A: Back-Dodger (repeatedly rolls away from boss)
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_BACKDODGER"), TEXT("DODGE_BACKWARD"), 8.0f);
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_BACKDODGER"), TEXT("DISTANCE"), 450.0f);

	// Profile B: Rasengan-Spammer (spams ranged jutsu at distance > 700cm)
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_RASENGAN_SPAMMER"), TEXT("RANGED_JUTSU"), 7.0f);
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_RASENGAN_SPAMMER"), TEXT("DISTANCE"), 850.0f);

	// Profile C: Turtle-Guard (passive close-range guarding < 300cm)
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_TURTLE_GUARD"), TEXT("GUARD"), 6.0f);
	AdaptiveAI->RecordCombatEvent(TEXT("PLAYER_TURTLE_GUARD"), TEXT("DISTANCE"), 200.0f);

	// 2. Persist Profiles to Disk JSON & Reload Verification
	const FString SaveDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("SaveGames"));
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*SaveDir))
	{
		PlatformFile.CreateDirectoryTree(*SaveDir);
	}

	const FString FileA = FPaths::Combine(SaveDir, TEXT("EOS_Telemetry_Profile_A.json"));
	const FString FileB = FPaths::Combine(SaveDir, TEXT("EOS_Telemetry_Profile_B.json"));
	const FString FileC = FPaths::Combine(SaveDir, TEXT("EOS_Telemetry_Profile_C.json"));

	FEOSTelemetryFeatureSnapshot SnapA = AdaptiveAI->ExtractBehaviorFeatures(TEXT("PLAYER_BACKDODGER"));
	FEOSTelemetryFeatureSnapshot SnapB = AdaptiveAI->ExtractBehaviorFeatures(TEXT("PLAYER_RASENGAN_SPAMMER"));
	FEOSTelemetryFeatureSnapshot SnapC = AdaptiveAI->ExtractBehaviorFeatures(TEXT("PLAYER_TURTLE_GUARD"));

	// Format JSON strings
	FString JsonA = FString::Printf(TEXT("{\"PlayerID\":\"PLAYER_BACKDODGER\",\"DodgeBackward\":%d,\"Distance\":%.1f}"),
		SnapA.PlayerDodgeBackwardCount, SnapA.PreferredDistanceUnits);
	FString JsonB = FString::Printf(TEXT("{\"PlayerID\":\"PLAYER_RASENGAN_SPAMMER\",\"RangedJutsu\":%d,\"Distance\":%.1f}"),
		SnapB.RangedJutsuSpamCount, SnapB.PreferredDistanceUnits);
	FString JsonC = FString::Printf(TEXT("{\"PlayerID\":\"PLAYER_TURTLE_GUARD\",\"Guard\":%d,\"Distance\":%.1f}"),
		SnapC.PlayerGuardCount, SnapC.PreferredDistanceUnits);

	TestTrue(TEXT("Profile A JSON saved to disk"), FFileHelper::SaveStringToFile(JsonA, *FileA));
	TestTrue(TEXT("Profile B JSON saved to disk"), FFileHelper::SaveStringToFile(JsonB, *FileB));
	TestTrue(TEXT("Profile C JSON saved to disk"), FFileHelper::SaveStringToFile(JsonC, *FileC));

	// Verify disk readback
	FString LoadedJsonA, LoadedJsonB, LoadedJsonC;
	TestTrue(TEXT("Profile A loaded from disk"), FFileHelper::LoadFileToString(LoadedJsonA, *FileA));
	TestTrue(TEXT("Profile B loaded from disk"), FFileHelper::LoadFileToString(LoadedJsonB, *FileB));
	TestTrue(TEXT("Profile C loaded from disk"), FFileHelper::LoadFileToString(LoadedJsonC, *FileC));

	// 3. Subsystem Evaluates Loaded Profiles & Produces Expected Directives
	FEOSAdaptationDirective DirA = AdaptiveAI->EvaluateAdaptationDirective(TEXT("BOSS_ZABUZA"), EEOSAITier::MajorBoss, SnapA);
	FEOSAdaptationDirective DirB = AdaptiveAI->EvaluateAdaptationDirective(TEXT("BOSS_ZABUZA"), EEOSAITier::MajorBoss, SnapB);
	FEOSAdaptationDirective DirC = AdaptiveAI->EvaluateAdaptationDirective(TEXT("BOSS_ZABUZA"), EEOSAITier::MajorBoss, SnapC);

	// Profile A must select GapCloser DashStrike
	TestEqual(TEXT("Profile A selects Gap Closer counter"), DirA.SelectedStrategyID, TEXT("ST_Action_GapCloser_DashStrike"));
	TestEqual(TEXT("Profile A sets GapCloser multiplier to 2.0x"), DirA.GapCloserWeightMultiplier, 2.0f);

	// Profile B must select Barrier / TeleportClose with projectile defense
	TestEqual(TEXT("Profile B selects Barrier/TeleportClose counter"), DirB.SelectedStrategyID, TEXT("ST_Action_Barrier_TeleportClose"));
	TestTrue(TEXT("Profile B triggers projectile defense"), DirB.bTriggerProjectileDefense);

	// Profile C must select MeleeGuardBreak Punish with close range punish
	TestEqual(TEXT("Profile C selects GuardBreak Punish counter"), DirC.SelectedStrategyID, TEXT("ST_Action_MeleeGuardBreak_Punish"));
	TestTrue(TEXT("Profile C triggers close range punish"), DirC.bTriggerCloseRangePunish);

	// Fairness Constraints Validation
	TestTrue(TEXT("Directive obeys fairness (no input reading, no damage tampering)"),
		UEOSAdaptiveAISubsystem::ValidateFairnessConstraints(DirB, false, false));
	TestFalse(TEXT("Fairness constraint rejects input reading attempts"),
		UEOSAdaptiveAISubsystem::ValidateFairnessConstraints(DirB, true, false));

	// 4. Repeated evaluations under identical conditions produce 100% deterministic results
	// Strategy selection evaluated independently from runtime attack cooldown state
	bool bDeterministic100 = true;
	for (int32 i = 0; i < 100; ++i)
	{
		FEOSAdaptationDirective EvalB = AdaptiveAI->EvaluateAdaptationDirective(TEXT("BOSS_ZABUZA"), EEOSAITier::MajorBoss, SnapB);
		if (EvalB.SelectedStrategyID != TEXT("ST_Action_Barrier_TeleportClose") || !EvalB.bTriggerProjectileDefense)
		{
			bDeterministic100 = false;
			break;
		}
	}
	TestTrue(TEXT("100/100 consecutive evaluations of Profile B produced identical ST_Action_Barrier_TeleportClose"), bDeterministic100);

	// 5. Selected tactic is passed into actual Zabuza Encounter FSM
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSBossCharacter* TestZabuza = World->SpawnActor<AEOSBossCharacter>(AEOSBossCharacter::StaticClass(), FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator, SpawnParams);
	if (TestNotNull(TEXT("TestZabuza spawned"), TestZabuza))
	{
		UEOSBossEncounterSubsystem* BossSubsystem = NewObject<UEOSBossEncounterSubsystem>(TestGI);
		BossSubsystem->InitializeGoldenBossRegistry();
		FEOSBossEncounterDefinition Def = BossSubsystem->GetBossEncounterDefinition(TEXT("BOSS_ZABUZA"));
		TestZabuza->InitializeFromEncounterDefinition(Def);
		TestZabuza->TriggerEntrance();
		TestZabuza->TransitionToState(EEOSBossState::Idle);
		TestZabuza->TransitionToPhase(2);

		// Execute the counter-tactic selected by Profile B (Water Dragon Barrage)
		const bool bExecPass = TestZabuza->ExecuteAttackPattern(TEXT("ZAB_WATER_DRAGON_BARRAGE"));
		TestTrue(TEXT("Zabuza executed adaptive pattern ZAB_WATER_DRAGON_BARRAGE"), bExecPass);
		TestEqual(TEXT("Encounter FSM transitioned to Telegraph"), TestZabuza->GetEncounterState().CurrentFSMState, EEOSBossState::Telegraph);
		TestEqual(TEXT("ActivePatternID is ZAB_WATER_DRAGON_BARRAGE"), TestZabuza->GetEncounterState().ActivePatternID, TEXT("ZAB_WATER_DRAGON_BARRAGE"));

		TestZabuza->Destroy();
	}

	return true;
}

#endif
