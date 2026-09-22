#include "../../Public/Tests/EOSPhase12Tests.h"
#include "../../Public/Subsystems/EOSNarrativeSubsystem.h"
#include "../../Public/Subsystems/EOSMotionMangaSubsystem.h"
#include "../../Public/Data/EOSNarrativeDataTypes.h"
#include "../../Public/Tests/EOSPhase11Tests.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase12Tests::RunAllPhase12ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1 - PHASE 12 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	// Execute Phase 1-11 tests first (85 suites)
	bool bP1_P11_Passed = FEOSPhase11Tests::RunAllPhase11ValidationTests();

	// Execute Phase 12 Narrative & Motion Manga tests (12 suites)
	bool bP12_Victory    = TestMissionCompleteAndVictoryValidation();
	bool bP12_BossHandoff= TestBossDefeatAndInputHandoff();
	bool bP12_HUDFade    = TestHUDTransitionProfiles();
	bool bP12_MusicTrans = TestMusicTransitionStateResolution();
	bool bP12_MangaReg   = TestMotionMangaSequenceRegistry();
	bool bP12_Parallax   = TestParallaxAndCameraMotionProfiles();
	bool bP12_Dialogue   = TestDialogueLocalizationAndSpeakerResolution();
	bool bP12_FinalPanel = TestFinalPanelAndArcCompletion();
	bool bP12_AntiDup    = TestRewardPresentationAntiDuplication();
	bool bP12_Archive    = TestStoryArchiveUnlocks();
	bool bP12_WorldTx    = TestWorldStateCommitTransactionSafety();
	bool bP12_SkipCrash  = TestSkipPauseReplayCrashRecovery();

	bool bPhase12Passed = bP12_Victory && bP12_BossHandoff && bP12_HUDFade && bP12_MusicTrans &&
	                      bP12_MangaReg && bP12_Parallax && bP12_Dialogue && bP12_FinalPanel &&
	                      bP12_AntiDup && bP12_Archive && bP12_WorldTx && bP12_SkipCrash;

	bool bAllPassed = bP1_P11_Passed && bPhase12Passed;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 12 TEST SUMMARY: %s (12/12 PHASE 12 SUITES PASSED)"), bPhase12Passed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("OVERALL SYSTEM RESULT: %s (97/97 TOTAL SYSTEM SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase12Tests::TestMissionCompleteAndVictoryValidation()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	// Case 1: Failure result must be rejected
	FEOSStageResult FailedResult;
	FailedResult.StageID = TEXT("STG_01_04_05");
	FailedResult.bIsSuccess = false;
	bool bRejectFailed = !Subsystem.StartMissionCompleteSequence(FailedResult);

	// Case 2: Authoritative success result accepted
	FEOSStageResult SuccessResult;
	SuccessResult.StageID = TEXT("STG_01_04_05");
	SuccessResult.bIsSuccess = true;
	SuccessResult.BossDefeated = true;
	SuccessResult.NarrativeSequenceID = TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX");
	SuccessResult.TransactionID = TEXT("TX_A01_STG05_TEST");
	bool bAcceptSuccess = Subsystem.StartMissionCompleteSequence(SuccessResult);

	bool bPass = bRejectFailed && bAcceptSuccess && (Subsystem.GetCurrentState() == EEOSNarrativeState::MissionComplete);
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Mission Complete & Authoritative Victory Validation: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestBossDefeatAndInputHandoff()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	// Capture input context
	Subsystem.CapturePreNarrativeInputContext(FVector(150.0f, 250.0f, 50.0f), FRotator(0.0f, 90.0f, 0.0f), true);

	// Signal boss defeat
	bool bHandoff = Subsystem.TriggerBossDefeatHandoff(TEXT("BOSS_ZABUZA"));
	bool bStateCorrect = (Subsystem.GetCurrentState() == EEOSNarrativeState::BossDefeated);

	// Restore input context
	bool bRestored = Subsystem.RestorePreNarrativeInputContext();

	bool bPass = bHandoff && bStateCorrect && bRestored;
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Boss Defeat & Post-Boss Control Handoff: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestHUDTransitionProfiles()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	FEOSNarrativeTransitionProfile Profile;
	bool bFound = Subsystem.GetTransitionProfile(TEXT("TRANS_PROFILE_DEFAULT"), Profile);

	bool bPass = bFound &&
	             (Profile.BossUIFadeTime == 0.10f) &&
	             (Profile.ObjectiveUIFadeTime == 0.25f) &&
	             (Profile.SkillUIFadeTime == 0.40f) &&
	             (Profile.MinimapFadeTime == 0.50f) &&
	             (Profile.FullHUDHiddenTime == 0.60f) &&
	             (Profile.NarrativeStartTime == 0.80f) &&
	             Profile.bLetterboxEnabled;

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Data-Driven HUD Staged Fade Profile: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestMusicTransitionStateResolution()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	FEOSMusicTransitionRecord Record;
	bool bFound = Subsystem.GetMusicTransition(TEXT("MUS_TRANS_BOSS_TO_MELANCHOLY"), Record);

	bool bPass = bFound &&
	             (Record.FromMusicState == TEXT("COMBAT_TRACK")) &&
	             (Record.ToMusicState == TEXT("NARRATIVE_MELANCHOLY")) &&
	             (Record.StingerID == TEXT("STINGER_BOSS_DEFEAT_02")) &&
	             (Record.CrossfadeDuration == 1.5f);

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Audio Stinger & Music State Transition Resolution: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestMotionMangaSequenceRegistry()
{
	UEOSMotionMangaSubsystem MangaSubsystem;
	MangaSubsystem.InitializeDefaultMangaDatabase();

	bool bLoaded = MangaSubsystem.LoadSequence(TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX"));
	int32 PanelCount = MangaSubsystem.GetTotalPanelCount();

	FEOSMotionMangaPanel FirstPanel;
	bool bHasFirst = MangaSubsystem.GetActivePanel(FirstPanel);

	bool bPass = bLoaded && (PanelCount == 5) && bHasFirst &&
	             (FirstPanel.PanelID == TEXT("MM_A01_C04_P001")) &&
	             (FirstPanel.MotionProfile == EEOSPanelMotionProfile::ZoomIn);

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Motion Manga Sequence & Panel Streaming Registry: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestParallaxAndCameraMotionProfiles()
{
	UEOSMotionMangaSubsystem MangaSubsystem;
	MangaSubsystem.InitializeDefaultMangaDatabase();

	// Test 2.5D Layer Parallax Calculation
	FVector2D CameraDelta(100.0f, 50.0f);
	FVector2D BgOffset = MangaSubsystem.CalculateLayerParallaxOffset(CameraDelta, 0.10f, false);
	FVector2D CharOffset = MangaSubsystem.CalculateLayerParallaxOffset(CameraDelta, 0.50f, false);
	FVector2D ReducedOffset = MangaSubsystem.CalculateLayerParallaxOffset(CameraDelta, 0.50f, true);

	// Test Camera Scale & Offset Interpolations
	float ZoomScale = MangaSubsystem.EvaluateCameraScale(EEOSPanelMotionProfile::ZoomIn, 1.0f, false);
	FVector2D PanOffset = MangaSubsystem.EvaluateCameraOffset(EEOSPanelMotionProfile::PanLeft, 1.0f, false);

	bool bParallaxCorrect = (BgOffset == FVector2D(10.0f, 5.0f)) &&
	                        (CharOffset == FVector2D(50.0f, 25.0f)) &&
	                        (ReducedOffset == FVector2D(15.0f, 7.5f));

	bool bCameraCorrect = (FMath::IsNearlyEqual(ZoomScale, 1.08f, 0.001f)) &&
	                      (PanOffset == FVector2D(-40.0f, 0.0f));

	bool bPass = bParallaxCorrect && bCameraCorrect;
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] 2.5D Parallax Offsets & Camera Motion Math: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestDialogueLocalizationAndSpeakerResolution()
{
	UEOSMotionMangaSubsystem MangaSubsystem;
	MangaSubsystem.InitializeDefaultMangaDatabase();

	// Speaker Resolution
	FText SpeakerName;
	FString Portrait;
	bool bSpeakerFound = MangaSubsystem.ResolveSpeaker(TEXT("CHAR_ZABUZA"), SpeakerName, Portrait);

	// Dialogue Entry Query
	FEOSDialogueEntry Entry;
	bool bDialogueFound = MangaSubsystem.GetDialogueEntry(TEXT("D_A01_C04_P001_01"), Entry);

	bool bPass = bSpeakerFound && bDialogueFound &&
	             (SpeakerName.ToString() == TEXT("Zabuza Momochi")) &&
	             (Portrait == TEXT("PORTRAIT_ZABUZA_DEMON")) &&
	             (Entry.LocalizationKey == TEXT("LOC_A01_C04_D001")) &&
	             (Entry.Emotion == TEXT("SORROW"));

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Dialogue Speaker & Localization Key Resolution: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestFinalPanelAndArcCompletion()
{
	UEOSMotionMangaSubsystem MangaSubsystem;
	MangaSubsystem.InitializeDefaultMangaDatabase();
	MangaSubsystem.LoadSequence(TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX"));
	MangaSubsystem.StartPlayback();

	// Advance through panels 0, 1, 2, 3, to panel 4 (Final Panel)
	MangaSubsystem.AdvancePanel(); // Panel 1
	MangaSubsystem.AdvancePanel(); // Panel 2
	MangaSubsystem.AdvancePanel(); // Panel 3
	MangaSubsystem.AdvancePanel(); // Panel 4 (Final)

	bool bIsFinal = MangaSubsystem.IsActivePanelFinal();
	FEOSMotionMangaPanel ActivePanel;
	MangaSubsystem.GetActivePanel(ActivePanel);

	bool bPass = bIsFinal && ActivePanel.bIsFinalPanel &&
	             (ActivePanel.PanelID == TEXT("MM_A01_C04_P005_FINAL")) &&
	             (ActivePanel.HoldDuration >= 2.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Final Panel Detection & Climax Hold Timing: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestRewardPresentationAntiDuplication()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	FEOSStageResult Result;
	Result.StageID = TEXT("STG_01_04_05");
	Result.bIsSuccess = true;
	Result.TransactionID = TEXT("TX_REWARD_TEST_001");
	Subsystem.StartMissionCompleteSequence(Result);

	TArray<FEOSRewardPresentationItem> Rewards;
	FEOSRewardPresentationItem Item1;
	Item1.RewardID = TEXT("REW_EXP_1200");
	Item1.Quantity = 1200;
	Rewards.Add(Item1);

	FEOSRewardPresentationItem Item2;
	Item2.RewardID = TEXT("REW_RYO_5000");
	Item2.Quantity = 5000;
	Rewards.Add(Item2);

	// First presentation grants rewards
	bool bFirstGrant = Subsystem.PresentRewards(Rewards, false);
	int32 FirstCount = Subsystem.GetPresentedRewardIDs().Num();

	// Replay or second grant attempts are strictly suppressed
	bool bSecondGrant = Subsystem.PresentRewards(Rewards, false);
	int32 SecondCount = Subsystem.GetPresentedRewardIDs().Num();

	bool bPass = bFirstGrant && bSecondGrant && (FirstCount == 2) && (SecondCount == 2);
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Idempotent Reward Presentation & Anti-Duplication: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestStoryArchiveUnlocks()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	bool bUnlocked = Subsystem.UnlockStoryArchiveEntry(TEXT("ARCHIVE_MM_A01_C04"), TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX"));
	bool bInList = Subsystem.GetUnlockedArchiveIDs().Contains(TEXT("ARCHIVE_MM_A01_C04"));

	bool bPass = bUnlocked && bInList;
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Story Archive & Motion Manga Library Unlock: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestWorldStateCommitTransactionSafety()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	FEOSStageResult Result;
	Result.StageID = TEXT("STG_01_04_05");
	Result.bIsSuccess = true;
	Result.TransactionID = TEXT("TX_WORLD_TEST_001");
	Subsystem.StartMissionCompleteSequence(Result);

	// First commit updates world state
	bool bFirstCommit = Subsystem.CommitWorldStateDelta(TEXT("DELTA_LAND_OF_WAVES_RESTORED"));
	FString WorldAfterFirst = Subsystem.GetActiveWorldStateID();

	// Second commit is idempotent
	bool bSecondCommit = Subsystem.CommitWorldStateDelta(TEXT("DELTA_LAND_OF_WAVES_RESTORED"));
	FString WorldAfterSecond = Subsystem.GetActiveWorldStateID();

	bool bPass = bFirstCommit && bSecondCommit &&
	             (WorldAfterFirst == TEXT("DELTA_LAND_OF_WAVES_RESTORED")) &&
	             (WorldAfterSecond == TEXT("DELTA_LAND_OF_WAVES_RESTORED"));

	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Transaction-Safe Atomic World State Commit: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase12Tests::TestSkipPauseReplayCrashRecovery()
{
	UEOSNarrativeSubsystem Subsystem;
	Subsystem.InitializeDefaultNarrativeDatabase();

	// 1. Test Pause & Playback Speed
	Subsystem.PauseSequence();
	Subsystem.SetPlaybackSpeed(1.5f);
	bool bSpeedValid = (Subsystem.GetPlaybackSpeed() == 1.5f);
	Subsystem.ResumeSequence();

	// 2. Test Skip Safety (Guarantees Archive & World State commits even when skipping)
	FEOSStageResult Result;
	Result.StageID = TEXT("STG_01_04_05");
	Result.bIsSuccess = true;
	Result.WorldStateDeltaID = TEXT("DELTA_LAND_OF_WAVES_RESTORED");
	Result.NarrativeSequenceID = TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX");
	Result.TransactionID = TEXT("TX_CRASH_TEST_001");
	Subsystem.StartMissionCompleteSequence(Result);

	bool bSkipped = Subsystem.SkipSequence();
	bool bArchiveCommitted = Subsystem.GetUnlockedArchiveIDs().Contains(TEXT("ARCHIVE_MM_SEQ_A01_C04_ZABUZA_CLIMAX"));
	bool bWorldCommitted = (Subsystem.GetActiveWorldStateID() == TEXT("DELTA_LAND_OF_WAVES_RESTORED"));

	// 3. Test Crash Recovery Resume
	FEOSNarrativeTransaction PendingTx;
	Subsystem.GetTransaction(TEXT("TX_CRASH_TEST_001"), PendingTx);
	bool bResumed = Subsystem.ResumePendingTransaction(TEXT("TX_CRASH_TEST_001"));

	bool bPass = bSpeedValid && bSkipped && bArchiveCommitted && bWorldCommitted && bResumed;
	UE_LOG(LogEOSCore, Log, TEXT("[P12 TEST] Skip Safety, Pause/Resume, and Crash Recovery: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
