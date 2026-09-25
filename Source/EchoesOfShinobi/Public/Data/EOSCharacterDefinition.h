#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSCharacterDataTypes.h"
#include "EOSCharacterDefinition.generated.h"

/**
 * Data-Driven Primary DataAsset defining a Shinobi Character across the 10-Layer Framework.
 * Prevents creation of 78 independent C++ classes by housing all stats, attacks, jutsu, skills,
 * ultimates, forms, gear grids, advancement, mastery, and compatibility as authorable data.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSCharacterDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EEOSCharacterEra Era = EEOSCharacterEra::ERA_GENIN;

	// Unified 10-Layer Character Framework Block
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterFramework")
	FEOSCharacterFrameworkBlock Framework;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSCharacter", FName(*CharacterId.StableId.ToString()));
	}
};
