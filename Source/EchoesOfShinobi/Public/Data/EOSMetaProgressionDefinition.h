#pragma once

#include "../CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSMetaProgressionDataTypes.h"
#include "EOSMetaProgressionDefinition.generated.h"

/**
 * DataAsset representing an authored Weapon / Gear Item
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear")
	FEOSGearItemRecord ItemRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSWeaponItem"), FName(*ItemRecord.ItemID));
	}
};

/**
 * DataAsset representing a Gear Set Bonus
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSGearSetDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GearSet")
	FEOSGearSetBonus SetBonus;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSGearSet"), FName(*SetBonus.SetID));
	}
};

/**
 * DataAsset representing a Character Relationship Bond
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSBondDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bond")
	FEOSCharacterBondRecord BondRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSBond"), FName(*BondRecord.BondID));
	}
};

/**
 * DataAsset representing an entry in the Shinobi Intel Encyclopedia
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSIntelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Intel")
	FEOSShinobiIntelRecord IntelRecord;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("EOSIntel"), FName(*IntelRecord.IntelID));
	}
};
