#pragma once

#include "CoreMinimal.h"

/**
 * Complete Phase 12 Test Suite:
 * Validates Narrative Presentation, Dialogue System, Opening UI, and 2.5D Motion Manga Pipeline.
 */
class ECHOESOFSHINOBI_API FEOSPhase12Tests
{
public:
	static bool RunAllPhase12ValidationTests();

	// 12 Phase 12 Test Suites
	static bool TestMissionCompleteAndVictoryValidation();
	static bool TestBossDefeatAndInputHandoff();
	static bool TestHUDTransitionProfiles();
	static bool TestMusicTransitionStateResolution();
	static bool TestMotionMangaSequenceRegistry();
	static bool TestParallaxAndCameraMotionProfiles();
	static bool TestDialogueLocalizationAndSpeakerResolution();
	static bool TestFinalPanelAndArcCompletion();
	static bool TestRewardPresentationAntiDuplication();
	static bool TestStoryArchiveUnlocks();
	static bool TestWorldStateCommitTransactionSafety();
	static bool TestSkipPauseReplayCrashRecovery();
};
