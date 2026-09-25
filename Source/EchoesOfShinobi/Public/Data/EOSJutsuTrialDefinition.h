#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSJutsuTrialDataTypes.h"
#include "EOSJutsuTrialDefinition.generated.h"

/**
 * Data asset defining a Jutsu Learning Trial and move specification.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSJutsuTrialDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSJutsuTrialDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JutsuTrial")
	FEOSJutsuTrialRecord Record;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
