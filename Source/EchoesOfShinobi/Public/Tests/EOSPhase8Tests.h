#pragma once

#include "CoreMinimal.h"

/**
 * Automated Test Suite for Phase 8: World Exploration, Konoha Hub & Interconnected Lands
 */
class ECHOESOFSHINOBI_API FEOSPhase8Tests
{
public:
	static bool RunAllPhase8ValidationTests();

	// Phase 8 Test Suites (7 suites)
	static bool TestZoneRegistrationAndRetrieval();
	static bool TestZoneInterconnectionAndTraversal();
	static bool TestFastTravelWaypointNetwork();
	static bool TestLoreDiscoveryAndArchiveIntegration();
	static bool TestWeatherAndTimeVariants();
	static bool TestWorldStateDamageReflection();
	static bool TestAllLandsCoverageAudit();
};
