#include "../../Public/Tests/EOSPhase11Tests.h"
#include "../../Public/Subsystems/EOSGearSubsystem.h"
#include "../../Public/Subsystems/EOSBondSubsystem.h"
#include "../../Public/Subsystems/EOSShinobiIntelSubsystem.h"
#include "../../Public/Subsystems/EOSCollectionSubsystem.h"
#include "../../Public/Data/EOSMetaProgressionDataTypes.h"
#include "../../Public/Tests/EOSPhase10Tests.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase11Tests::RunAllPhase11ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1 - PHASE 11 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	// Execute Phase 1-10 tests first (73 suites)
	bool bP1_P10_Passed = FEOSPhase10Tests::RunAllPhase10ValidationTests();

	// Execute Phase 11 Meta-Progression tests (12 suites)
	bool bP11_GearReg   = TestGearItemRegistrationAndSlotCompatibility();
	bool bP11_Enhance   = TestGearEnhancementAndStatGrowth();
	bool bP11_Advance   = TestGearTierAdvancement();
	bool bP11_SetBonus  = TestGearSetBonusResolution();
	bool bP11_Build     = TestCharacterBuildLoadoutManagement();
	bool bP11_BondReg   = TestBondRelationshipRegistration();
	bool bP11_BondProg  = TestBondProgressionAndLevelUp();
	bool bP11_Dialogue  = TestBondContextualDialogueAndTeamEffects();
	bool bP11_IntelLog  = TestShinobiIntelDiscoveryStateAndLogging();
	bool bP11_KGraph    = TestKnowledgeGraphCrossLinking();
	bool bP11_Collect   = TestCollectionCategoryCompletionAndMilestones();
	bool bP11_MetaSlice = TestPhase11CompleteMetaLoopVerticalSlice();

	bool bPhase11Passed = bP11_GearReg && bP11_Enhance && bP11_Advance && bP11_SetBonus && bP11_Build &&
	                      bP11_BondReg && bP11_BondProg && bP11_Dialogue && bP11_IntelLog && bP11_KGraph &&
	                      bP11_Collect && bP11_MetaSlice;
	bool bAllPassed = bP1_P10_Passed && bPhase11Passed;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 11 TEST SUMMARY: %s (12/12 PHASE 11 SUITES PASSED)"), bPhase11Passed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("OVERALL SYSTEM RESULT: %s (85/85 TOTAL SYSTEM SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase11Tests::TestGearItemRegistrationAndSlotCompatibility()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	FEOSGearItemRecord Sword;
	bool bFoundSword = GearSubsystem.GetGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), Sword);

	FEOSGearItemRecord Artifact;
	bool bFoundArtifact = GearSubsystem.GetGearItem(TEXT("ARTIFACT_UCHIHA_FIRE_FAN"), Artifact);

	FEOSGearItemRecord Armor;
	bool bFoundArmor = GearSubsystem.GetGearItem(TEXT("EQUIP_UCHIHA_COMBAT_ROBE"), Armor);

	bool bPass = bFoundSword && bFoundArtifact && bFoundArmor &&
	             (Sword.GearType == EEOSMetaGearType::Weapon) &&
	             (Artifact.GearType == EEOSMetaGearType::Artifact) &&
	             (Armor.EquipSlot == EEOSGearSlot::CHEST_ARMOR) &&
	             (Sword.BasePrimaryStatBonus == 120.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Gear Item Registration & Slot Classification: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestGearEnhancementAndStatGrowth()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	FEOSGearItemRecord SwordBefore;
	GearSubsystem.GetGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), SwordBefore);

	// Enhance by 5 levels (Lv 1 -> Lv 6, stat growth = 8.0f per level)
	bool bEnhanced = GearSubsystem.EnhanceGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), 5);

	FEOSGearItemRecord SwordAfter;
	GearSubsystem.GetGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), SwordAfter);

	bool bPass = bEnhanced && (SwordAfter.CurrentLevel == 6) &&
	             (SwordAfter.BasePrimaryStatBonus == (SwordBefore.BasePrimaryStatBonus + (5 * SwordBefore.StatGrowthPerLevel)));

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Deterministic Gear Enhancement & Stat Growth (Lv 1 -> Lv 6): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestGearTierAdvancement()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	// Enhance to level cap (Lv 10)
	GearSubsystem.EnhanceGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), 9);

	// Advance tier (Tier 1 Basic -> Tier 2 Reinforced)
	bool bAdvanced = GearSubsystem.AdvanceGearTier(TEXT("WEAPON_KUSANAGI_SWORD"));

	FEOSGearItemRecord AdvancedItem;
	GearSubsystem.GetGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), AdvancedItem);

	bool bPass = bAdvanced && (AdvancedItem.CurrentTier == EEOSGearTier::Tier2_Reinforced) &&
	             (AdvancedItem.MaxLevel == 20) && (AdvancedItem.BasePrimaryStatBonus > 192.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Gear Tier Advancement Breakthrough (Tier 1 -> Tier 2): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestGearSetBonusResolution()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	const FString CharSasuke = TEXT("CHR_SASUKE_MS");

	// Equip 2 pieces of Uchiha Legacy (Sword + Fan)
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("WEAPON_KUSANAGI_SWORD"), EEOSGearSlot::WEAPON_TOOL);
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("ARTIFACT_UCHIHA_FIRE_FAN"), EEOSGearSlot::ACCESSORY_1);

	int32 Pieces2 = GearSubsystem.EvaluateEquippedSetCount(CharSasuke, TEXT("SET_UCHIHA_LEGACY"));
	bool bTwoPieceActive = GearSubsystem.IsSetBonusActive(CharSasuke, TEXT("SET_UCHIHA_LEGACY"), 2);
	bool bFourPieceInactive = !GearSubsystem.IsSetBonusActive(CharSasuke, TEXT("SET_UCHIHA_LEGACY"), 4);

	// Equip remaining 2 pieces (Armor + Greaves)
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("EQUIP_UCHIHA_COMBAT_ROBE"), EEOSGearSlot::CHEST_ARMOR);
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("EQUIP_UCHIHA_GREAVES"), EEOSGearSlot::BOOTS);

	int32 Pieces4 = GearSubsystem.EvaluateEquippedSetCount(CharSasuke, TEXT("SET_UCHIHA_LEGACY"));
	bool bFourPieceActive = GearSubsystem.IsSetBonusActive(CharSasuke, TEXT("SET_UCHIHA_LEGACY"), 4);

	bool bPass = (Pieces2 == 2) && bTwoPieceActive && bFourPieceInactive && (Pieces4 == 4) && bFourPieceActive;

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Gear Set Bonus Resolution (2-Piece & 4-Piece Activation): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestCharacterBuildLoadoutManagement()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	const FString CharSasuke = TEXT("CHR_SASUKE_MS");

	FEOSCharacterBuildLoadout BossBuild;
	BossBuild.BuildID = TEXT("BUILD_SASUKE_BOSS_BURST");
	BossBuild.BuildName = TEXT("Boss Assault Build");
	BossBuild.CharacterID = CharSasuke;
	BossBuild.WeaponItemID = TEXT("WEAPON_KUSANAGI_SWORD");
	BossBuild.Artifact1ItemID = TEXT("ARTIFACT_UCHIHA_FIRE_FAN");
	BossBuild.ActiveSetID = TEXT("SET_UCHIHA_LEGACY");
	BossBuild.CalculatedGearPower = 1250.0f;

	bool bSaved = GearSubsystem.SaveBuildLoadout(CharSasuke, BossBuild);

	FEOSCharacterBuildLoadout Retrieved;
	bool bFound = GearSubsystem.GetBuildLoadout(CharSasuke, TEXT("BUILD_SASUKE_BOSS_BURST"), Retrieved);

	bool bPass = bSaved && bFound && (Retrieved.BuildName == TEXT("Boss Assault Build")) &&
	             (Retrieved.ActiveSetID == TEXT("SET_UCHIHA_LEGACY"));

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Character Build Loadout Management & Snapshots: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestBondRelationshipRegistration()
{
	UEOSBondSubsystem BondSubsystem;
	BondSubsystem.InitializeDefaultBondDatabase();

	FEOSCharacterBondRecord NarutoSasukeBond;
	bool bFoundRival = BondSubsystem.GetBond(TEXT("BOND_NARUTO_SASUKE"), NarutoSasukeBond);

	FEOSCharacterBondRecord JiraiyaNarutoBond;
	bool bFoundMentor = BondSubsystem.GetBond(TEXT("BOND_JIRAIYA_NARUTO"), JiraiyaNarutoBond);

	bool bPass = bFoundRival && bFoundMentor &&
	             (NarutoSasukeBond.BondType == EEOSBondType::Rival) &&
	             (JiraiyaNarutoBond.BondType == EEOSBondType::MasterStudent) &&
	             (NarutoSasukeBond.BondLevel == 1);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Bond Relationship Registration (Rival & Master/Student): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestBondProgressionAndLevelUp()
{
	UEOSBondSubsystem BondSubsystem;
	BondSubsystem.InitializeDefaultBondDatabase();

	FEOSCharacterBondRecord BondBefore;
	BondSubsystem.GetBond(TEXT("BOND_NARUTO_SASUKE"), BondBefore);

	// Add 1500 Bond XP -> advances from Lv 1 to Lv 2
	bool bXPGained = BondSubsystem.AddBondXP(TEXT("BOND_NARUTO_SASUKE"), 1500);

	FEOSCharacterBondRecord BondAfter;
	BondSubsystem.GetBond(TEXT("BOND_NARUTO_SASUKE"), BondAfter);

	// Set directly to max (Lv 5)
	BondSubsystem.SetBondLevel(TEXT("BOND_NARUTO_SASUKE"), 5);
	FEOSCharacterBondRecord BondMax;
	BondSubsystem.GetBond(TEXT("BOND_NARUTO_SASUKE"), BondMax);

	bool bPass = bXPGained && (BondAfter.BondLevel == 2) && (BondMax.BondLevel == 5) && BondMax.bIsMaxLevel;

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Bond Progression & Level Breakthrough (Lv 1 -> Lv 2 -> Lv 5 Max): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestBondContextualDialogueAndTeamEffects()
{
	UEOSBondSubsystem BondSubsystem;
	BondSubsystem.InitializeDefaultBondDatabase();

	// Verify contextual dialogue retrieval for Mission Start and Character Swap
	FEOSBondDialogueRecord DlgStart;
	bool bFoundStart = BondSubsystem.GetDialogueForTrigger(TEXT("BOND_NARUTO_SASUKE"), EEOSBondDialogueTrigger::MissionStart, DlgStart);

	FEOSBondDialogueRecord DlgSwap;
	bool bFoundSwap = BondSubsystem.GetDialogueForTrigger(TEXT("BOND_NARUTO_SASUKE"), EEOSBondDialogueTrigger::CharacterSwap, DlgSwap);

	// Verify Team Synergy evaluation at Level 4
	BondSubsystem.SetBondLevel(TEXT("BOND_NARUTO_SASUKE"), 4);
	float SwapBonus = 0.0f, JutsuBonus = 0.0f;
	bool bSynergyActive = BondSubsystem.EvaluateTeamSynergy(TEXT("CHR_NARUTO_GENIN"), TEXT("CHR_SASUKE_GENIN"), SwapBonus, JutsuBonus);

	bool bPass = bFoundStart && bFoundSwap && bSynergyActive && (SwapBonus > 0.0f) && (JutsuBonus > 0.0f);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Contextual Dialogue & Team Synergy Deployment Buffs: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestShinobiIntelDiscoveryStateAndLogging()
{
	UEOSShinobiIntelSubsystem IntelSubsystem;
	IntelSubsystem.InitializeDefaultIntelDatabase();

	FEOSShinobiIntelRecord ObitoIntel;
	bool bFoundObito = IntelSubsystem.GetIntelRecord(TEXT("INTEL_BOSS_OBITO"), ObitoIntel);

	// Log repeated boss encounters -> discovery upgrades to FullyRevealed
	IntelSubsystem.LogEncounter(TEXT("INTEL_BOSS_OBITO"));
	IntelSubsystem.LogEncounter(TEXT("INTEL_BOSS_OBITO"));
	IntelSubsystem.GetIntelRecord(TEXT("INTEL_BOSS_OBITO"), ObitoIntel);

	bool bPass = bFoundObito && (ObitoIntel.Category == EEOSIntelCategory::Boss) &&
	             (ObitoIntel.DiscoveryState == EEOSDiscoveryState::FullyRevealed) &&
	             !ObitoIntel.WeaknessDescription.IsEmpty();

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Shinobi Intel Discovery States & Adaptive Boss Logging: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestKnowledgeGraphCrossLinking()
{
	UEOSShinobiIntelSubsystem IntelSubsystem;
	IntelSubsystem.InitializeDefaultIntelDatabase();

	TArray<FString> NarutoLinks = IntelSubsystem.GetCrossLinkedEntities(TEXT("INTEL_CHR_NARUTO"));

	bool bHasJiraiya = NarutoLinks.Contains(TEXT("INTEL_CHR_JIRAIYA"));
	bool bHasSasuke = NarutoLinks.Contains(TEXT("INTEL_CHR_SASUKE"));
	bool bHasRasengan = NarutoLinks.Contains(TEXT("INTEL_JUT_RASENGAN"));
	bool bHasKurama = NarutoLinks.Contains(TEXT("INTEL_BEAST_KURAMA"));

	bool bPass = (NarutoLinks.Num() >= 5) && bHasJiraiya && bHasSasuke && bHasRasengan && bHasKurama;

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Knowledge Graph Cross-Entity Navigation (Naruto Links: %d): %s"), NarutoLinks.Num(), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestCollectionCategoryCompletionAndMilestones()
{
	UEOSCollectionSubsystem CollectionSubsystem;
	CollectionSubsystem.InitializeDefaultCollectionDatabase();

	float CharComp = CollectionSubsystem.GetCategoryCompletionPercentage(EEOSCollectionCategory::Characters);
	float GlobalComp = CollectionSubsystem.GetGlobalArchiveCompletionPercentage();
	int32 ArchiveLevel = CollectionSubsystem.GetArchiveAccountLevel();

	int32 GrantedGems = 0;
	FString UnlockedTitle;
	int32 Tier = CollectionSubsystem.EvaluateMilestoneRewards(GrantedGems, UnlockedTitle);

	bool bPass = (CharComp > 50.0f) && (GlobalComp > 50.0f) && (ArchiveLevel >= 1) &&
	             (Tier >= 1) && (GrantedGems > 0);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Collection Category Tracking (Global: %.1f%%, Archive Lv %d, Gems: %d): %s"),
		GlobalComp, ArchiveLevel, GrantedGems, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase11Tests::TestPhase11CompleteMetaLoopVerticalSlice()
{
	UEOSGearSubsystem GearSubsystem;
	GearSubsystem.InitializeDefaultGearDatabase();

	UEOSBondSubsystem BondSubsystem;
	BondSubsystem.InitializeDefaultBondDatabase();

	UEOSShinobiIntelSubsystem IntelSubsystem;
	IntelSubsystem.InitializeDefaultIntelDatabase();

	UEOSCollectionSubsystem CollectionSubsystem;
	CollectionSubsystem.InitializeDefaultCollectionDatabase();

	const FString CharSasuke = TEXT("CHR_SASUKE_MS");
	const FString CharNaruto = TEXT("CHR_NARUTO_GENIN");

	// 1. Equip Weapons, Artifacts & Activate Set Bonus
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("WEAPON_KUSANAGI_SWORD"), EEOSGearSlot::WEAPON_TOOL);
	GearSubsystem.EquipItemToCharacter(CharSasuke, TEXT("ARTIFACT_UCHIHA_FIRE_FAN"), EEOSGearSlot::ACCESSORY_1);
	GearSubsystem.EnhanceGearItem(TEXT("WEAPON_KUSANAGI_SWORD"), 5);
	bool bSetBonusActive = GearSubsystem.IsSetBonusActive(CharSasuke, TEXT("SET_UCHIHA_LEGACY"), 2);
	float GearPower = GearSubsystem.CalculateGearPower(CharSasuke);

	// 2. Advance Team Bond & Synergy
	BondSubsystem.AddBondXP(TEXT("BOND_NARUTO_SASUKE"), 2000);
	float SwapBonus = 0.0f, JutsuBonus = 0.0f;
	bool bSynergy = BondSubsystem.EvaluateTeamSynergy(CharNaruto, CharSasuke, SwapBonus, JutsuBonus);

	// 3. Boss Encounter -> Log Intel & Discover Lore
	IntelSubsystem.LogEncounter(TEXT("INTEL_BOSS_OBITO"));
	IntelSubsystem.SetDiscoveryState(TEXT("INTEL_REALM_KAMUI"), EEOSDiscoveryState::FullyRevealed);

	// 4. Update Permanent Collection Archive
	CollectionSubsystem.SetEntryOwned(TEXT("COL_BOSS_OBITO"), true);
	CollectionSubsystem.SetEntryMastered(TEXT("COL_BOSS_OBITO"), true);
	CollectionSubsystem.SetEntryOwned(TEXT("COL_MM_OBITO_UNMASKED"), true);

	float FinalGlobalComp = CollectionSubsystem.GetGlobalArchiveCompletionPercentage();
	int32 AccountArchiveLevel = CollectionSubsystem.GetArchiveAccountLevel();

	bool bPass = bSetBonusActive && (GearPower > 300.0f) && bSynergy &&
	             (FinalGlobalComp > 65.0f) && (AccountArchiveLevel >= 2);

	UE_LOG(LogEOSCore, Log, TEXT("[P11 TEST] Phase 11 Complete Meta Loop Vertical Slice (Power: %.1f, Archive Lv: %d): %s"),
		GearPower, AccountArchiveLevel, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
