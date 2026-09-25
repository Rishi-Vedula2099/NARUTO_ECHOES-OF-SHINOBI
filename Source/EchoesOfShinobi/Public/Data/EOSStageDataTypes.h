#pragma once

#include "CoreMinimal.h"
#include "EOSDataTypes.h"
#include "EOSStoryDataTypes.h"
#include "EOSStageDataTypes.generated.h"

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
	EEOSStageType StageType = EEOSStageType::Story;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 RecommendedPower = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	bool bIsUnlockedByDefault = false;
};
