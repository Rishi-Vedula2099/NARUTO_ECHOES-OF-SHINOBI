#pragma once

#include "CoreMinimal.h"

/**
 * Automated Test Suite for Phase 6: Adaptive Boss, Mini-Boss & Minion AI Architecture
 */
class ECHOESOFSHINOBI_API FEOSPhase6Tests
{
public:
	static bool RunAllPhase6ValidationTests();

	// Phase 6 Test Suites (6 suites)
	static bool TestBehaviorFeatureExtractorTelemetry();
	static bool TestBossMemoryPersistence();
	static bool TestAdaptationDirectorBoundedStrategySelection();
	static bool TestThreeTierAIArchetypeGating();
	static bool TestFairnessConstraintsEnforcement();
	static bool TestAdaptiveAIRematchNemesisExperience();
};
