#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSStageDataTypes.h"
#include "EOSStageDefinition.generated.h"

UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSStageDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId StageId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId ChapterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId ArcId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Type")
	EEOSStageType StageType = EEOSStageType::Story;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Power")
	int32 RecommendedPower = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objectives")
	TArray<FEOSStageObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	FEOSStableId TargetBossId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
	FEOSStableId MotionMangaOutroId;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSStage", StageId.StableId);
	}
};
