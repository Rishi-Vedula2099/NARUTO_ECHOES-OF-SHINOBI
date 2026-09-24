#pragma once

#include "CoreMinimal.h"

/**
 * Automated Test Suite for Phase 9: Game Modes & Player Experiences
 */
class ECHOESOFSHINOBI_API FEOSPhase9Tests
{
public:
	static bool RunAllPhase9ValidationTests();

	// Phase 9 Test Suites (8 suites)
	static bool TestModeRegistrationAndRetrieval();
	static bool TestModeLifecycleStateMachine();
	static bool TestModeObjectiveAndModifierEvaluation();
	static bool TestModeScoringEngineAndFormulas();
	static bool TestRewardResolutionAndIdempotency();
	static bool TestWeeklyBountyRotationSystem();
	static bool TestAdaptiveBossRematchIntegration();
	static bool TestVerticalSliceModesExecution();
};
