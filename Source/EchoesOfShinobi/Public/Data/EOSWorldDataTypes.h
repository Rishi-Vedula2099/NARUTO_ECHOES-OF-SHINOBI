#pragma once

#include "../CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSWorldDataTypes.generated.h"

/**
 * World Region Identifier
 */
UENUM(BlueprintType)
enum class EEOSWorldRegion : uint8
{
	Konoha             UMETA(DisplayName = "Hidden Leaf Village & Environs"),
	LandOfFire         UMETA(DisplayName = "Land of Fire Perimeter"),
	LandOfWind         UMETA(DisplayName = "Land of Wind & Sunagakure"),
	LandOfWater        UMETA(DisplayName = "Land of Water & Kirigakure"),
	LandOfLightning    UMETA(DisplayName = "Land of Lightning & Kumogakure"),
	LandOfEarth        UMETA(DisplayName = "Land of Earth & Iwagakure"),
	Amegakure          UMETA(DisplayName = "Hidden Rain Village"),
	MountMyoboku       UMETA(DisplayName = "Mount Myoboku Sage Realm"),
	ValleyOfTheEnd     UMETA(DisplayName = "Valley of the End"),
	WarFronts          UMETA(DisplayName = "Fourth Shinobi War Fronts"),
	KamuiDimension     UMETA(DisplayName = "Kamui Dimension"),
	DivineTreeRealms   UMETA(DisplayName = "Divine Tree Core Realms"),
	AstralEclipseRealm UMETA(DisplayName = "Astral Eclipse Endgame Realm")
};

/**
 * Zone Spatial / Functional Classification
 */
UENUM(BlueprintType)
enum class EEOSZoneType : uint8
{
	HubVillage         UMETA(DisplayName = "Main Hub Village"),
	District           UMETA(DisplayName = "City District"),
	TrainingGrounds    UMETA(DisplayName = "Shinobi Training Grounds"),
	RooftopNetwork     UMETA(DisplayName = "Vertical Rooftop Highway"),
	ForestWilderness   UMETA(DisplayName = "Forest / Wilderness Zone"),
	InteriorFacility   UMETA(DisplayName = "Interior Facility / Hall"),
	MemorialMonument   UMETA(DisplayName = "Memorial / Historical Monument"),
	UndergroundNetwork UMETA(DisplayName = "Subterranean Tunnel Network"),
	Battlefield        UMETA(DisplayName = "Open Battlefield"),
	DimensionRealm     UMETA(DisplayName = "Alternate Dimension")
};

/**
 * Weather & Time of Day Dynamic Variants
 */
UENUM(BlueprintType)
enum class EEOSWeatherTimeVariant : uint8
{
	Day                UMETA(DisplayName = "Clear Daylight"),
	Sunset             UMETA(DisplayName = "Golden Sunset"),
	Night              UMETA(DisplayName = "Moonlit Night"),
	Rain               UMETA(DisplayName = "Heavy Rain Downpour"),
	Storm              UMETA(DisplayName = "Thunderstorm / Sandstorm"),
	DamagedPostInvasion UMETA(DisplayName = "Invasion Ruins & Smoldering Debris")
};

/**
 * Lore Prop Category
 */
UENUM(BlueprintType)
enum class EEOSLoreCategory : uint8
{
	Memorial           UMETA(DisplayName = "Memorial Inscription"),
	Scroll             UMETA(DisplayName = "Ancient Ninjutsu Scroll"),
	ClanRelic          UMETA(DisplayName = "Clan Crest / Relic"),
	TrainingMark       UMETA(DisplayName = "Combat Training Mark"),
	BrokenWeapon       UMETA(DisplayName = "Broken Shinobi Weapon"),
	EnvironmentalClue  UMETA(DisplayName = "Environmental Tracking Clue"),
	FactionSymbol      UMETA(DisplayName = "Faction Sigil"),
	ArchiveTrigger     UMETA(DisplayName = "Story Archive Trigger")
};

/**
 * NPC Role
 */
UENUM(BlueprintType)
enum class EEOSNPCRole : uint8
{
	MissionGiver       UMETA(DisplayName = "Mission / Quest Board Master"),
	BountyMaster       UMETA(DisplayName = "Weekly Bounty Master"),
	ShopKeeper         UMETA(DisplayName = "Merchant / Ninja Tool Vendor"),
	Trainer            UMETA(DisplayName = "Jutsu / Sparring Trainer"),
	LoreKeeper         UMETA(DisplayName = "Historical Lore Keeper"),
	Civilian           UMETA(DisplayName = "Village Civilian / Observer")
};

/**
 * Traversal Point / Route Node
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSTraversalNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
	FString NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
	FString TraversalType = "RooftopShingle"; // WallRun, Wireline, HiddenTunnel, LedgeGrip

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
	FString TargetNodeID;
};

/**
 * Interactive Lore Prop
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSLoreProp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	FString PropID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	FString ZoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	EEOSLoreCategory Category = EEOSLoreCategory::Memorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	FString ArchiveUnlockID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	bool bIsDiscovered = false;
};

/**
 * Fast-Travel Waypoint
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSFastTravelPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	FString PointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	FString ZoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	FVector WorldPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	bool bIsUnlockedByDefault = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastTravel")
	bool bIsUnlocked = false;
};

/**
 * Interactive NPC Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNPCInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NPCID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString ZoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	EEOSNPCRole Role = EEOSNPCRole::Civilian;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString DialogueSetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString RequiredWorldStateID;
};

/**
 * Comprehensive World Zone Definition
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSWorldZone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString ZoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	EEOSWorldRegion Region = EEOSWorldRegion::Konoha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	EEOSZoneType ZoneType = EEOSZoneType::District;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	EEOSWeatherTimeVariant ActiveWeather = EEOSWeatherTimeVariant::Day;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	TArray<FString> ConnectedZoneIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString FastTravelPointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	TArray<FString> LorePropIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	TArray<FString> NPCIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString MissionBoardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString BountyBoardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString ShopID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString RequiredWorldStateID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString CanonStatus = "CANON_VERIFIED";
};
