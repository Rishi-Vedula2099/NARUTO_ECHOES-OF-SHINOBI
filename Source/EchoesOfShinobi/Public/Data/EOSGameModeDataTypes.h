#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSGameModeDataTypes.generated.h"

/**
 * Top-Level Game Mode Category
 */
UENUM(BlueprintType)
enum class EEOSGameModeCategory : uint8
{
	Story      UMETA(DisplayName = "Story & Campaign Narrative"),
	Training   UMETA(DisplayName = "Training & Shinobi Mastery"),
	Challenge  UMETA(DisplayName = "Replayable Combat & Survival Challenges"),
	Live       UMETA(DisplayName = "Live-Ops Bounties & Events"),
	World      UMETA(DisplayName = "World Exploration & Expeditions")
};

/**
 * Discrete Game Mode Types
 */
UENUM(BlueprintType)
enum class EEOSGameModeType : uint8
{
	// Story
	StoryCampaign      UMETA(DisplayName = "Story Campaign"),
	ShinobiChronicles  UMETA(DisplayName = "Shinobi Chronicles"),
	LegacyArchive      UMETA(DisplayName = "Legacy Archive"),
	MotionManga        UMETA(DisplayName = "Motion Manga Presentation"),

	// Training
	TrainingArena      UMETA(DisplayName = "Free Training Arena"),
	GeninTraining      UMETA(DisplayName = "Genin Fundamentals"),
	ChuninTraining     UMETA(DisplayName = "Chunin Tactical Combos"),
	JoninTraining      UMETA(DisplayName = "Jonin Advanced Mastery"),
	SageTraining       UMETA(DisplayName = "Sage Mode Balance Trials"),
	JutsuArena         UMETA(DisplayName = "Jutsu Practice Arena"),
	ChakraTrials       UMETA(DisplayName = "Chakra Control Precision Trials"),
	MentorDojo         UMETA(DisplayName = "Mentor Dojo Master Challenges"),
	BeastResonance     UMETA(DisplayName = "Tailed Beast Resonance Trials"),

	// Challenge
	ShinobiTower       UMETA(DisplayName = "Shinobi Tower Floor Climb"),
	SurvivalWaves      UMETA(DisplayName = "Survival Enemy Waves"),
	BossRush           UMETA(DisplayName = "Sequential Boss Rush"),
	BossRematch        UMETA(DisplayName = "Adaptive Boss Rematch Memory"),
	TimeTrial          UMETA(DisplayName = "Time Trial Speedrun"),
	NinjaRelay         UMETA(DisplayName = "Ninja Traversal Relay"),
	TeamSynergy        UMETA(DisplayName = "Trio Team Synergy Trials"),
	PerfectMission     UMETA(DisplayName = "Flawless Perfect Mission"),
	RogueBoard         UMETA(DisplayName = "Rogue Mission Board"),
	DimensionalRift    UMETA(DisplayName = "Dimensional Rift Alternate Trial"),

	// Live
	WeeklyBounty       UMETA(DisplayName = "Weekly Seeded Bounty Hunt"),
	LimitedEvent       UMETA(DisplayName = "Limited Character Release Event"),
	LimitedTrial       UMETA(DisplayName = "Limited Character Test Trial"),
	VillageDefense     UMETA(DisplayName = "Village Invasion Defense"),
	RotatingChallenge  UMETA(DisplayName = "Rotating Condition Challenge"),

	// World
	WorldExploration   UMETA(DisplayName = "Free-Roam World Exploration"),
	Expedition         UMETA(DisplayName = "Regional Expedition & Extraction"),
	ScrollHunt         UMETA(DisplayName = "Hidden Lore Scroll Hunt"),
	WorldEvent         UMETA(DisplayName = "Dynamic World Event")
};

/**
 * Mode Lifecycle State Machine States
 */
UENUM(BlueprintType)
enum class EEOSModeState : uint8
{
	Locked,
	Available,
	Selected,
	Preparing,
	Loading,
	Initializing,
	Active,
	Paused,
	Completed,
	Failed,
	RewardPending,
	RewardClaimed,
	Archived
};

/**
 * Mode Objective Types
 */
UENUM(BlueprintType)
enum class EEOSModeObjectiveType : uint8
{
	DefeatAll,
	DefeatTarget,
	DefeatBoss,
	SurviveTime,
	ProtectTarget,
	NoDamage,
	ChakraEfficiency,
	ComboTarget,
	PerfectDodgeCount,
	CounterCount,
	SubstitutionCount,
	TeamSwapCount,
	ElementInteraction
};

/**
 * Mode Modifier Classification
 */
UENUM(BlueprintType)
enum class EEOSModeModifierType : uint8
{
	EnemyBuff,
	PlayerDebuff,
	EnvironmentalHazard,
	ResourceRestriction,
	TimePressure,
	TechniqueRestriction
};

/**
 * Reward Type
 */
UENUM(BlueprintType)
enum class EEOSModeRewardType : uint8
{
	CharacterEXP,
	AccountEXP,
	JutsuXP,
	MasteryXP,
	Gear,
	Artifact,
	Material,
	Currency,
	SummonCurrency,
	Lore,
	ArchiveUnlock,
	BondXP
};

/**
 * Objective Definition for a Game Mode
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSModeObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	EEOSModeObjectiveType ObjectiveType = EEOSModeObjectiveType::DefeatAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 TargetCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bIsOptional = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bIsCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 ScoreBonus = 250;
};

/**
 * Active Modifier affecting Game Mode Rules
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSModeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FString ModifierID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	EEOSModeModifierType ModifierType = EEOSModeModifierType::PlayerDebuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FString ConflictGroup = "NONE"; // e.g. GROUP_HEALING, GROUP_CHAKRA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float Magnitude = 1.0f;
};

/**
 * Reward Entry for Mode Completion
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSModeRewardEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	EEOSModeRewardType RewardType = EEOSModeRewardType::CharacterEXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	bool bFirstClearOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	bool bMasteryOnly = false;
};

/**
 * Master Record for a Game Mode
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGameModeRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FString ModeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	EEOSGameModeCategory Category = EEOSGameModeCategory::Story;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	EEOSGameModeType ModeType = EEOSGameModeType::StoryCampaign;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bReplayable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bScoreEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int32 RecommendedPower = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int32 MinimumPower = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	float TimeLimit = 300.0f; // in seconds

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int32 PartySize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FString TargetStageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TArray<FEOSModeObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TArray<FEOSModeModifier> ActiveModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TArray<FEOSModeRewardEntry> Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FString CanonStatus = "CANON_VERIFIED";
};

/**
 * Mode Run Result & Score Breakdown
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSModeRunResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FString RunID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FString ModeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	bool bIsCleared = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	int32 FinalScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FString CombatGrade = "S";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	float ClearTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	int32 DamageTaken = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	int32 MaxCombo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	int32 PerfectDodges = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	int32 CompletedObjectiveCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	TArray<FEOSModeRewardEntry> GrantedRewards;
};

/**
 * Persistent Player Progress for a Mode
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSModeProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	FString ModeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	int32 HighestScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	float BestTime = 9999.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	int32 HighestFloor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	bool bFirstClearClaimed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	int32 TotalClears = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	int32 MasteryLevel = 0;
};

/**
 * Weekly Bounty Rotation Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBountyDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString BountyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString TargetID; // e.g. BOSS_ZABUZA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FText TargetDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString LocationID; // e.g. ZONE_WAVES_BRIDGE

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString ActiveModifierID; // e.g. MOD_MIST_DENSITY

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString BonusObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	int32 RecommendedPower = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	TArray<FEOSModeRewardEntry> Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	FString ResetSchedule = "Thursday 00:00 UTC";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounty")
	bool bIsClaimedThisWeek = false;
};
