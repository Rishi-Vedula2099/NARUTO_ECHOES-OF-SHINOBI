#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSStoryDataTypes.h"
#include "EOSStoryDefinition.generated.h"

/**
 * Data asset for Arcs, Chapters, and Stages content authoring.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSStoryDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSStoryDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	FEOSArcRecord ArcRecord;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	FEOSChapterRecord ChapterRecord;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	FEOSStageRecord StageRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
