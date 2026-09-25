#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSWorldDataTypes.h"
#include "EOSWorldDefinition.generated.h"

/**
 * DataAsset for authoring an individual World Zone / District
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSWorldZoneDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	FEOSWorldZone ZoneData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TArray<FEOSLoreProp> AuthoredLoreProps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TArray<FEOSNPCInteractable> AuthoredNPCs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	FEOSFastTravelPoint AuthoredFastTravelPoint;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSWorldZone"), FName(*ZoneData.ZoneID));
	}
};
