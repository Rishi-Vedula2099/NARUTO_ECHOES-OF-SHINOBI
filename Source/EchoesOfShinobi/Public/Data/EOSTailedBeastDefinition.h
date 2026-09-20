#pragma once

#include "../CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSTailedBeastDataTypes.h"
#include "EOSTailedBeastDefinition.generated.h"

/**
 * Data Asset defining a Tailed Beast Entity and Remnant Power parameters.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSTailedBeastDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSTailedBeastDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TailedBeast")
	FEOSTailedBeastDefinitionRecord Record;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
