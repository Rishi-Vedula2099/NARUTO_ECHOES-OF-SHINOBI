#pragma once

#include "CoreMinimal.h"

/**
 * Automated Test Suite for Phase 7: Data-Driven Story & Stage Content Pipeline
 */
class ECHOESOFSHINOBI_API FEOSPhase7Tests
{
public:
	static bool RunAllPhase7ValidationTests();

	// Phase 7 Test Suites (7 suites)
	static bool TestDataDrivenArcChapterStageLoading();
	static bool TestStageObjectiveSystemEvaluation();
	static bool TestWorldStateDeltaTransitions();
	static bool TestMotionMangaPlaybackAndArchiveUnlock();
	static bool TestStageRewardAndPowerFloorValidation();
	static bool TestCanonStatusAndProvenanceTagging();
	static bool TestFullCampaign500StagesCoverage();
};
