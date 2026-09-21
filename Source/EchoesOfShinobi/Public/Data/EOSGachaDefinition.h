#pragma once

#include "../CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSGachaDataTypes.h"
#include "EOSGachaDefinition.generated.h"

/**
 * DataAsset representing an authored Summon Banner
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSBannerDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Banner")
	FEOSBannerDefinition BannerDefinition;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSBanner"), FName(*BannerDefinition.BannerID));
	}
};

/**
 * DataAsset representing an authored Limited Live Event
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSLimitedEventDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	FEOSLimitedEventDefinition EventDefinition;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSEvent"), FName(*EventDefinition.EventID));
	}
};

/**
 * DataAsset representing an authored Weekly Bounty
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSWeeklyBountyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bounty")
	FEOSWeeklyBountyRecord BountyRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSWeeklyBounty"), FName(*BountyRecord.BountyID));
	}
};
