#pragma once

#include "CoreMinimal.h"

/**
 * Automated Test Suite for Phase 4: Rank Progression, Mentor Training, Nature Energy & Sage Pathways
 */
class ECHOESOFSHINOBI_API FEOSPhase4Tests
{
public:
	static bool RunAllPhase4ValidationTests();

	// Phase 4 Test Suites (6 suites)
	static bool TestRankAdvancementGating();
	static bool TestMentorTrainingSessionExecution();
	static bool TestSageCompatibilityRules();
	static bool TestNatureEnergyGaugeStabilityMath();
	static bool TestSageFormActivationAndJutsuIntegration();
	static bool TestRankAndSageTrialRetryPersistence();
};
