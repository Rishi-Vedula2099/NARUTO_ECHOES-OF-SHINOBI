#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSStoryDataTypes.generated.h"

/**
 * Stage Classification Types
 */
UENUM(BlueprintType)
enum class EEOSStageType : uint8
{
	Story     UMETA(DisplayName = "Story Traversal & Minion Encounter"),
	Elite     UMETA(DisplayName = "Elite Shinobi Combat Challenge"),
	Challenge UMETA(DisplayName = "Mechanics & Timing Trial"),
	MiniBoss  UMETA(DisplayName = "Mini-Boss Encounter"),
	Boss      UMETA(DisplayName = "Major Multi-Phase Arc Boss Fight")
};

/**
 * Objective Types
 */
UENUM(BlueprintType)
enum class EEOSObjectiveType : uint8
{
	DefeatEnemy,
	DefeatTarget,
	DefeatBoss,
	Survive,
	Protect,
	Escort,
	ReachLocation,
	UseJutsu,
	UseForm,
	BreakBoss,
	AvoidDamage,
	TimeTrial
};

/**
 * Reusable Stage Objective Definition
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSStageObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	EEOSObjectiveType ObjectiveType = EEOSObjectiveType::DefeatEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 TargetCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bIsOptional = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bIsCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString RewardTableID;
};

/**
 * Unified Stage Data Record (ARC##_CH##_ST###)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSStageRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString StageID; // e.g. STG_01_01_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString ArcID; // e.g. ARC_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString ChapterID; // e.g. CH_01_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	EEOSStageType StageType = EEOSStageType::Story;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 RecommendedPower = 920;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 HardPowerFloor = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	TArray<FEOSStageObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString EnemySetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString EliteID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString MiniBossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString RewardTableID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString DialogueSetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString MotionMangaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString WorldStateDeltaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString CanonStatus = "CANON_VERIFIED";
};

/**
 * Chapter Record (4 Chapters per Arc)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSChapterRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FString ChapterID; // e.g. CH_01_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FString ArcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	int32 ChapterNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FText Synopsis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	TArray<FString> StageIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FString MotionMangaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chapter")
	FString WorldStateDeltaID;
};

/**
 * Arc Record (25 Arcs Total)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSArcRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	FString ArcID; // e.g. ARC_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	EEOSCharacterEra StoryEra = EEOSCharacterEra::ERA_GENIN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	TArray<FString> ChapterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	FString StartWorldStateID = "WORLD_STATE_KONOHA_NORMAL";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	FString EndWorldStateID = "WORLD_STATE_LAND_OF_WAVES_RESTORED";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
	FString CanonStatus = "CANON_VERIFIED";
};

/**
 * Motion Manga 2.5D Parallax Sequence
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMotionMangaSequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString MotionSequenceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ArcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ChapterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString StageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	float ParallaxDepth = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ArchiveUnlockID;
};

/**
 * World State Delta Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSWorldStateDelta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldState")
	FString DeltaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldState")
	FString SourceStageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldState")
	FString TargetWorldStateID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldState")
	FString EnvironmentVariant = "Env_Damaged";
};

/**
 * Story Archive Entry
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSStoryArchiveEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	FString ArchiveEntryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	FString EntryType = "MotionManga"; // MotionManga, Lore, BossMemory, Transformation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	FString SourceArcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	FString SourceChapterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StoryArchive")
	bool bIsUnlocked = false;
};
