#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSMetaProgressionDataTypes.generated.h"

/**
 * Top-Level Meta Gear Classification
 */
UENUM(BlueprintType)
enum class EEOSMetaGearType : uint8
{
	Weapon        UMETA(DisplayName = "Shinobi Weapon"),
	Artifact      UMETA(DisplayName = "Chakra / Relic Artifact"),
	Equipment     UMETA(DisplayName = "Armor / Headband Equipment"),
	SpecialRelic  UMETA(DisplayName = "Special Clan / Kage Relic")
};

/**
 * Weapon Combat Classes
 */
UENUM(BlueprintType)
enum class EEOSWeaponClass : uint8
{
	Kunai           UMETA(DisplayName = "Throwing Kunai"),
	Shuriken        UMETA(DisplayName = "Giant / Fuma Shuriken"),
	Sword           UMETA(DisplayName = "Katana / Ninjato"),
	SpecialBlade    UMETA(DisplayName = "Seven Mist Blade / Kusanagi"),
	Staff           UMETA(DisplayName = "Adamantine / Chakra Staff"),
	NinjaTool       UMETA(DisplayName = "Scientific Ninja Tool"),
	SignatureWeapon UMETA(DisplayName = "Signature Canon Weapon")
};

/**
 * Artifact Role
 */
UENUM(BlueprintType)
enum class EEOSArtifactType : uint8
{
	Chakra       UMETA(DisplayName = "Chakra Regeneration Catalyst"),
	Defensive    UMETA(DisplayName = "Iron Wall Protective Relic"),
	Offensive    UMETA(DisplayName = "Lethal Strike Relic"),
	Sage         UMETA(DisplayName = "Nature Energy Harmonizer"),
	TailedBeast  UMETA(DisplayName = "Beast Vessel Seal Stone"),
	BondRelic    UMETA(DisplayName = "Comrade Resonance Charm")
};

/**
 * Gear Tier Breakthrough Progression
 */
UENUM(BlueprintType)
enum class EEOSGearTier : uint8
{
	Tier1_Basic         UMETA(DisplayName = "Tier 1: Basic Shinobi Gear"),
	Tier2_Reinforced    UMETA(DisplayName = "Tier 2: Reinforced Iron"),
	Tier3_Forged        UMETA(DisplayName = "Tier 3: Chakra Forged"),
	Tier4_Masterwork    UMETA(DisplayName = "Tier 4: Masterwork Heritage"),
	Tier5_Transcendent  UMETA(DisplayName = "Tier 5: Transcendent Legendary")
};

/**
 * Relationship Bond Type
 */
UENUM(BlueprintType)
enum class EEOSBondType : uint8
{
	Teammate             UMETA(DisplayName = "Squad Teammate"),
	MasterStudent        UMETA(DisplayName = "Master & Apprentice"),
	Family               UMETA(DisplayName = "Immediate Family"),
	Clan                 UMETA(DisplayName = "Clan Bloodline"),
	Friend               UMETA(DisplayName = "Trusted Friend"),
	Rival                UMETA(DisplayName = "Eternal Rival"),
	Ally                 UMETA(DisplayName = "Allied Shinobi"),
	Mentor               UMETA(DisplayName = "Senior Mentor"),
	HistoricalConnection UMETA(DisplayName = "Fated / Reincarnated Bond")
};

/**
 * Contextual Bond Dialogue Trigger in Combat/World
 */
UENUM(BlueprintType)
enum class EEOSBondDialogueTrigger : uint8
{
	FirstMeeting     UMETA(DisplayName = "First Deployment Meeting"),
	MissionStart     UMETA(DisplayName = "Stage Departure Banter"),
	Victory          UMETA(DisplayName = "Joint Victory Scene"),
	LowHP            UMETA(DisplayName = "Partner Critical HP Warning"),
	CharacterSwap    UMETA(DisplayName = "Tag-Team Swap Reaction"),
	UltimateCombo    UMETA(DisplayName = "Dual Linked Ultimate Jutsu")
};

/**
 * Shinobi Intel Knowledge Category
 */
UENUM(BlueprintType)
enum class EEOSIntelCategory : uint8
{
	Character        UMETA(DisplayName = "Shinobi Profile"),
	Enemy            UMETA(DisplayName = "Hostile Enemy Unit"),
	Boss             UMETA(DisplayName = "Boss & Rematch Intel"),
	Jutsu            UMETA(DisplayName = "Jutsu Technique Encyclopedia"),
	Form             UMETA(DisplayName = "Form & Transformation"),
	Location         UMETA(DisplayName = "World Landmark / District"),
	Lore             UMETA(DisplayName = "Historical Ninja Annals"),
	Event            UMETA(DisplayName = "Special Live Event Record")
};

/**
 * Player Research & Discovery State
 */
UENUM(BlueprintType)
enum class EEOSDiscoveryState : uint8
{
	Locked             UMETA(DisplayName = "Locked / Unknown"),
	Discovered         UMETA(DisplayName = "Initial Discovery"),
	PartiallyRevealed  UMETA(DisplayName = "Field Encounter Recorded"),
	FullyRevealed      UMETA(DisplayName = "Fully Researched"),
	Mastered           UMETA(DisplayName = "Tactical Mastery Archive")
};

/**
 * Permanent Shinobi Collection Categories
 */
UENUM(BlueprintType)
enum class EEOSCollectionCategory : uint8
{
	Characters       UMETA(DisplayName = "Playable Shinobi Roster"),
	Forms            UMETA(DisplayName = "Shinobi Forms & Avatars"),
	Jutsu            UMETA(DisplayName = "Mastered Jutsu Arsenal"),
	Weapons          UMETA(DisplayName = "Ninja Weapons"),
	Artifacts        UMETA(DisplayName = "Sacred Artifacts"),
	GearSets         UMETA(DisplayName = "Equipment Sets"),
	Lore             UMETA(DisplayName = "Historical Lore Records"),
	Enemies          UMETA(DisplayName = "Enemy Bestiary"),
	Bosses           UMETA(DisplayName = "Boss Archive"),
	WorldLocations   UMETA(DisplayName = "World Regions & Waypoints"),
	MotionManga      UMETA(DisplayName = "Motion Manga Sequences")
};

/**
 * Detailed Gear Item Record
 */
USTRUCT(BlueprintType)
struct FEOSGearItemRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSMetaGearType GearType = EEOSMetaGearType::Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSWeaponClass WeaponClass = EEOSWeaponClass::Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSGearSlot EquipSlot = EEOSGearSlot::WEAPON_TOOL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSGearTier CurrentTier = EEOSGearTier::Tier1_Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasePrimaryStatBonus = 50.0f; // e.g. +50 Attack or +50 Defense

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatGrowthPerLevel = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PassiveSkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SetID; // e.g. "SET_UCHIHA_LEGACY"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFavorite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquippedCharacterID; // Empty if in inventory
};

/**
 * Gear Set Bonus Definition
 */
USTRUCT(BlueprintType)
struct FEOSGearSetBonus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TwoPieceBonusDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FourPieceBonusDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TwoPieceAttackBonusPct = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TwoPieceCritRateBonusPct = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FourPieceElementalDamagePct = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FourPieceChakraRegenPct = 12.0f;
};

/**
 * Saved Character Build Loadout Snapshot
 */
USTRUCT(BlueprintType)
struct FEOSCharacterBuildLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildName; // e.g. "Boss Assault Build", "Survival CC Build"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FormID = TEXT("FORM_BASE");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Artifact1ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Artifact2ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Equipment1ItemID; // Armor / Vest

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Equipment2ItemID; // Boots / Headband

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpecialRelicItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActiveSetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CalculatedGearPower = 0.0f;
};

/**
 * Character Relationship Bond Record
 */
USTRUCT(BlueprintType)
struct FEOSCharacterBondRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BondID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSBondType BondType = EEOSBondType::Teammate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BondLevel = 1; // 1 to 5

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredXP = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TeamSynergyEffectDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SynergySwapSpeedBonusPct = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SynergyJutsuPowerBonusPct = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMaxLevel = false;
};

/**
 * Contextual Dialogue Line
 */
USTRUCT(BlueprintType)
struct FEOSBondDialogueRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BondID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSBondDialogueTrigger Trigger = EEOSBondDialogueTrigger::MissionStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpeakerCharID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueText;
};

/**
 * Shinobi Intel Knowledge Record
 */
USTRUCT(BlueprintType)
struct FEOSShinobiIntelRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IntelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSIntelCategory Category = EEOSIntelCategory::Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSDiscoveryState DiscoveryState = EEOSDiscoveryState::Discovered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EncounterCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ElementAffinity; // e.g. "Fire / Lightning"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaknessDescription; // Tactical counterplay

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObservedBehaviorNotes; // Player-facing observed patterns

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoreSummary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> CrossLinkedEntityIDs; // Knowledge Graph connections
};

/**
 * Collection Archive Item Entry
 */
USTRUCT(BlueprintType)
struct FEOSCollectionEntryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EntryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSCollectionCategory Category = EEOSCollectionCategory::Characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOwned = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDiscovered = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMastered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstAcquiredDate = TEXT("2026-09-01");
};
