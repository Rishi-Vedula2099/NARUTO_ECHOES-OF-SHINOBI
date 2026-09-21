#pragma once

#include "../CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSGachaDataTypes.generated.h"

/**
 * High-level Summon / Banner Category
 */
UENUM(BlueprintType)
enum class EEOSSummonType : uint8
{
	Standard      UMETA(DisplayName = "Standard Permanent Pool"),
	Featured      UMETA(DisplayName = "Featured Rate-Up Banner"),
	Limited       UMETA(DisplayName = "Time-Limited Event Banner"),
	Event         UMETA(DisplayName = "Event-Exclusive Pool"),
	Gear          UMETA(DisplayName = "Shinobi Gear & Weapon Pool"),
	Guaranteed    UMETA(DisplayName = "Milestone Guaranteed Pool"),
	Ticket        UMETA(DisplayName = "Ticket-Restricted Banner"),
	Free          UMETA(DisplayName = "Daily / Milestone Free Pull")
};

/**
 * Data-Driven Shinobi Rarity Tier
 */
UENUM(BlueprintType)
enum class EEOSShinobiRarity : uint8
{
	Rarity_3      UMETA(DisplayName = "3-Star Common Shinobi"),
	Rarity_4      UMETA(DisplayName = "4-Star Elite Shinobi"),
	Rarity_5      UMETA(DisplayName = "5-Star Legendary Shinobi")
};

/**
 * Banner Lifecycle States
 */
UENUM(BlueprintType)
enum class EEOSBannerState : uint8
{
	Draft         UMETA(DisplayName = "Draft / Authoring"),
	Scheduled     UMETA(DisplayName = "Scheduled For Release"),
	Active        UMETA(DisplayName = "Active / Available"),
	Expiring      UMETA(DisplayName = "Expiring (Final 24H)"),
	Ended         UMETA(DisplayName = "Ended (No New Pulls)"),
	Archived      UMETA(DisplayName = "Archived In History")
};

/**
 * Currency Categories in the LiveOps Economy
 */
UENUM(BlueprintType)
enum class EEOSCurrencyType : uint8
{
	Premium       UMETA(DisplayName = "Shinobi Gems (Premium)"),
	Free          UMETA(DisplayName = "Ryo (In-Game Free)"),
	TicketStd     UMETA(DisplayName = "Standard Summon Ticket"),
	TicketEvt     UMETA(DisplayName = "Event Summon Ticket"),
	EventCurrency UMETA(DisplayName = "Time-Limited Event Currency"),
	BountyMedal   UMETA(DisplayName = "Weekly Bounty Medal")
};

/**
 * Expiration Behavior for Event Currencies
 */
UENUM(BlueprintType)
enum class EEOSCurrencyExpiration : uint8
{
	NeverExpire             UMETA(DisplayName = "Never Expire"),
	ExpireAtEventEnd        UMETA(DisplayName = "Expire Immediately At Event End"),
	ExpireAfterClaimWindow  UMETA(DisplayName = "Expire After 7-Day Claim Window"),
	ConvertToRyo            UMETA(DisplayName = "Automatically Convert To Ryo")
};

/**
 * Limited Event Top-Level Type
 */
UENUM(BlueprintType)
enum class EEOSEventType : uint8
{
	StoryEvent     UMETA(DisplayName = "Narrative Story Event"),
	BossEvent      UMETA(DisplayName = "Adaptive Boss Battle Event"),
	CharacterEvent UMETA(DisplayName = "Featured Character Release Event"),
	LimitedTrial   UMETA(DisplayName = "Limited Character Test Trial"),
	SeasonalEvent  UMETA(DisplayName = "Seasonal Festival Event"),
	WorldEvent     UMETA(DisplayName = "World-State Disturbance Event")
};

/**
 * Multi-Phase Event Progression
 */
UENUM(BlueprintType)
enum class EEOSEventPhase : uint8
{
	Phase1_Threat        UMETA(DisplayName = "Phase 1: Emerging Threat"),
	Phase2_Escalation    UMETA(DisplayName = "Phase 2: Spatial Escalation"),
	Phase3_Revelation    UMETA(DisplayName = "Phase 3: Shinobi Revelation"),
	Phase4_FinalBattle   UMETA(DisplayName = "Phase 4: Climax & Final Confrontation")
};

/**
 * Bounty Target Classification
 */
UENUM(BlueprintType)
enum class EEOSBountyTargetType : uint8
{
	Minion         UMETA(DisplayName = "Minion Mob"),
	Elite          UMETA(DisplayName = "Elite Shinobi Squad"),
	MiniBoss       UMETA(DisplayName = "Mini-Boss"),
	Boss           UMETA(DisplayName = "Major Story Boss"),
	EventBoss      UMETA(DisplayName = "Adaptive Event Boss")
};

/**
 * Rarity Configuration Record
 */
USTRUCT(BlueprintType)
struct FEOSRarityDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSShinobiRarity RarityTier = EEOSShinobiRarity::Rarity_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName = TEXT("Common");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAcquisitionWeight = 85.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DuplicateCharacterShards = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DuplicateAwakeningCurrency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DuplicateBondTokens = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RevealVisualID = TEXT("VFX_SUMMON_SILVER");
};

/**
 * Discrete Summon Pool Entry
 */
USTRUCT(BlueprintType)
struct FEOSSummonPoolEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EntryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EraID = TEXT("ERA_DEFAULT");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FormID = TEXT("FORM_BASE");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSShinobiRarity Rarity = EEOSShinobiRarity::Rarity_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFeatured = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GearID;
};

/**
 * Pity & Guarantee Rules Engine
 */
USTRUCT(BlueprintType)
struct FEOSPityRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PityRuleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSSummonType TargetBannerType = EEOSSummonType::Featured;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SoftPityStart = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HardPity = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoftPityRateIncreasePerPull = 6.0f; // +6.0% 5★ probability per pull above 60

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FeaturedRateOn5Star = 0.50f; // 50% chance for featured unit on 5★

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGuaranteedFeaturedOnNext5StarIfMissed = true; // 50/50 loss guarantee

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCarryOverPityToNextBanner = true;
};

/**
 * Banner Definition Record
 */
USTRUCT(BlueprintType)
struct FEOSBannerDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BannerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSSummonType SummonType = EEOSSummonType::Featured;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSBannerState CurrentState = EEOSBannerState::Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PoolID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSCurrencyType CostCurrency = EEOSCurrencyType::Premium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SinglePullCost = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MultiPullCost = 1500; // 10x pull

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MultiPullCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PityRuleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> FeaturedCharacterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StartTimeUTC = TEXT("2026-09-01T00:00:00Z");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EndTimeUTC = TEXT("2026-10-01T00:00:00Z");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AssociatedEventID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;
};

/**
 * Single Acquisition Item Result
 */
USTRUCT(BlueprintType)
struct FEOSAcquisitionResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EraID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FormID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSShinobiRarity Rarity = EEOSShinobiRarity::Rarity_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsNewUnlock = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDuplicate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrantedCharacterShards = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrantedAwakeningCurrency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrantedBondTokens = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GearID;
};

/**
 * Complete Transaction Result from a Summon Action
 */
USTRUCT(BlueprintType)
struct FEOSSummonResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TransactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BannerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PullCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PityBefore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PityAfter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTriggeredHardPity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTriggeredGuaranteedFeatured = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrencySpent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEOSAcquisitionResult> Acquisitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSuccess = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StatusMessage;
};

/**
 * Event Shop Item Specification
 */
USTRUCT(BlueprintType)
struct FEOSShopItemDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RewardType; // "CharacterShards", "SummonTicket", "AwakeningMaterial", "GearRecipe"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PurchaseLimit = 5; // e.g. Max 5 purchases

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentPurchases = 0;
};

/**
 * Limited-Time Event Package Specification
 */
USTRUCT(BlueprintType)
struct FEOSLimitedEventDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSEventType EventType = EEOSEventType::StoryEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSEventPhase CurrentPhase = EEOSEventPhase::Phase1_Threat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AssociatedBannerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventCurrencyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventBossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MotionMangaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WorldStateDisturbanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventShopID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEOSShopItemDefinition> ShopItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> EventMissionStageIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StartTimeUTC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EndTimeUTC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClaimEndTimeUTC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
};

/**
 * Configurable Backend Rules for Weekly Bounty Reset
 */
USTRUCT(BlueprintType)
struct FEOSBountyScheduleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResetDay = TEXT("Thursday"); // Configurable default: Thursday

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResetTime = TEXT("00:00:00"); // Configurable default: 00:00:00

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Timezone = TEXT("UTC");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ActiveBountyCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowCrossModeAttachment = true;
};

/**
 * Discrete Weekly Bounty Record
 */
USTRUCT(BlueprintType)
struct FEOSWeeklyBountyRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BountyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetBossName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEOSBountyTargetType TargetType = EEOSBountyTargetType::Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocationModeID; // e.g. "MODE_STORY_CAMPAIGN" or "MODE_SURVIVAL_WAVES"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveDescription; // e.g. "Execute 8 Perfect Dodges during encounter"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredCount = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ModifierDescription; // e.g. "Mist Density 80% / Enemy Speed +25%"

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BountyMedalReward = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RyoReward = 50000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpecialMaterialDrop = TEXT("MAT_WATER_MIST_ESSENCE");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRewardClaimed = false;
};
