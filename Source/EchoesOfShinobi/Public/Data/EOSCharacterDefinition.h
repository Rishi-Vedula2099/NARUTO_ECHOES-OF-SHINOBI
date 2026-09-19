#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSCharacterDefinition.generated.h"

UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EEOSCharacterEra Era = EEOSCharacterEra::ERA_GENIN;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elements")
	TArray<EEOSElement> ElementalAffinities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Attributes")
	float BaseHealth = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Attributes")
	float BaseChakra = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Attributes")
	float BaseAttack = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Attributes")
	float BaseDefense = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Attributes")
	float BaseSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FEOSStableId> JutsuIds;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSCharacter", CharacterId.StableId);
	}
};
