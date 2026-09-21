#pragma once

#include "../CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSGameModeDataTypes.h"
#include "EOSGameModeDefinition.generated.h"

/**
 * DataAsset representing an authored Game Mode
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSGameModeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	FEOSGameModeRecord ModeRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSGameMode"), FName(*ModeRecord.ModeID));
	}
};

/**
 * DataAsset representing a Weekly Bounty
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSBountyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bounty")
	FEOSBountyDefinition BountyRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSBounty"), FName(*BountyRecord.BountyID));
	}
};
