#pragma once

#include "CoreMinimal.h"
#include "EOSDataTypes.h"
#include "EOSStageDataTypes.generated.h"

UENUM(BlueprintType)
enum class EEOSStageType : uint8
{
	STORY UMETA(DisplayName = "Story Stage"),
	ELITE UMETA(DisplayName = "Elite Encounter Stage"),
	CHALLENGE UMETA(DisplayName = "Challenge Stage"),
	MINI_BOSS UMETA(DisplayName = "Mini-Boss Stage"),
	BOSS UMETA(DisplayName = "Adaptive Boss Stage")
};

UENUM(BlueprintType)
enum class EEOSStageObjectiveType : uint8
{
	DEFEAT_ALL UMETA(DisplayName = "Defeat All Enemies"),
	DEFEAT_BOSS UMETA(DisplayName = "Defeat Target Boss"),
	TIME_TRIAL UMETA(DisplayName = "Clear Within Time Limit"),
	PERFECT_DODGE_COUNT UMETA(DisplayName = "Perform N Perfect Dodges"),
	PROTECT_TARGET UMETA(DisplayName = "Protect Ally / Escort"),
	NO_HEALING UMETA(DisplayName = "Clear Without Healing")
};

USTRUCT(BlueprintType)
struct FEOSStageObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FEOSStableId ObjectiveId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	EEOSStageObjectiveType ObjectiveType = EEOSStageObjectiveType::DEFEAT_ALL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 TargetValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	float RewardMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FEOSStageNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FEOSStableId NodeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FEOSStableId StageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	TArray<FEOSStableId> PrerequisiteNodeIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	EEOSStageType StageType = EEOSStageType::STORY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 RecommendedPower = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	bool bIsUnlockedByDefault = false;
};
