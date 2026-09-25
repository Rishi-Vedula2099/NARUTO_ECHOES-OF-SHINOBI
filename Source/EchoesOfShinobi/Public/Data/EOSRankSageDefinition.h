#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSRankSageDataTypes.h"
#include "EOSRankSageDefinition.generated.h"

/**
 * Data asset for Rank Progression and Sage Trial specifications.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSRankSageDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSRankSageDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RankSage")
	FEOSRankProgressionRecord RankRecord;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RankSage")
	FEOSSagePathwayRecord SageRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
