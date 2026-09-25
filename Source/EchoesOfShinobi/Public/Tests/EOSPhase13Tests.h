#pragma once

#include "CoreMinimal.h"

/**
 * Complete Phase 13 Test Suite:
 * Validates UI Subsystem, Navigation Stack, 10-Layer Compositor,
 * ViewModels, Modals, Notifications, and Complete UI Vertical Slice.
 */
class ECHOESOFSHINOBI_API FEOSPhase13Tests
{
public:
	static bool RunAllPhase13ValidationTests();

	// 12 Phase 13 Test Suites
	static bool TestScreenRegistrationAndLayerOrdering();
	static bool TestNavigationStackPushPopAndBackflow();
	static bool TestModalConfirmationAndBackgroundBlocking();
	static bool TestCombatHUDAndBossHUDViewModel();
	static bool TestCharacterDetailPowerBreakdown();
	static bool TestJutsuLearningAndTrialEligibilityUI();
	static bool TestGearComparisonAndSetBonusDisplay();
	static bool TestShinobiIntelDiscoveryFilters();
	static bool TestCollectionAndStoryArchiveNavigation();
	static bool TestNotificationDeepLinkRouting();
	static bool TestSettingsAndScalableGraphicsPresets();
	static bool TestCompleteUIVerticalSlice();

	// Phase 13 Playable Vertical Slice & Visual Reconstruction Suites
	static bool TestCameraAndMovementProfiles();
	static bool TestAssetProvenanceAndOriginalityGate();
	static bool TestKonohaVillageBuilderAndNPCLODSimulation();
	static bool TestJutsuExecutionAndSubstitutionAction();
	static bool TestCompletePlayableVerticalSliceLoop();

	// Master Cross-Phase Art & Visual Style Suite (Phases 1 - 13)
	static bool TestCompleteCrossPhaseArtAndVisualStyling();
};
