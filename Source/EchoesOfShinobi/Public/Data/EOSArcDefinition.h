#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSStageDataTypes.h"
#include "EOSArcDefinition.generated.h"

UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSArcDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId ArcId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText ArcName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	int32 ArcIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chapters & Nodes")
	TArray<FEOSStageNode> StageNodes;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSArc", ArcId.StableId);
	}
};
