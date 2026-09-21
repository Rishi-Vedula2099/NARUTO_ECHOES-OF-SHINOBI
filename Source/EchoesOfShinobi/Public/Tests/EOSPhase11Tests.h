#pragma once

#include "../CoreMinimal.h"

/**
 * Automated Verification Suites for Phase 11:
 * Gear/Equipment, Enhancement/Advancement, Set Bonuses, Character Builds,
 * Bonds/Relationships, Contextual Dialogue, Team Effects, Shinobi Intel,
 * Knowledge Graph, and Collection Archive.
 */
class ECHOESOFSHINOBI_API FEOSPhase11Tests
{
public:
	static bool RunAllPhase11ValidationTests();

	// 12 Dedicated Unit / Integration Tests
	static bool TestGearItemRegistrationAndSlotCompatibility();
	static bool TestGearEnhancementAndStatGrowth();
	static bool TestGearTierAdvancement();
	static bool TestGearSetBonusResolution();
	static bool TestCharacterBuildLoadoutManagement();
	static bool TestBondRelationshipRegistration();
	static bool TestBondProgressionAndLevelUp();
	static bool TestBondContextualDialogueAndTeamEffects();
	static bool TestShinobiIntelDiscoveryStateAndLogging();
	static bool TestKnowledgeGraphCrossLinking();
	static bool TestCollectionCategoryCompletionAndMilestones();
	static bool TestPhase11CompleteMetaLoopVerticalSlice();
};
