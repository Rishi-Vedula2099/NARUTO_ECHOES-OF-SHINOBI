#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSGearDefinition.generated.h"

UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSGearDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId GearId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	EEOSGearSlot Slot = EEOSGearSlot::HEADBAND;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BasePrimaryStatBonus = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float StatGrowthPerLevel = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Bonus")
	FEOSStableId GearSetId;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSGear", GearId.StableId);
	}
};
