#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSDataTypes.h"
#include "EOSAbilityDefinition.generated.h"

UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSAbilityDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FEOSStableId AbilityId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EEOSJutsuType JutsuType = EEOSJutsuType::JUTSU_PRIMARY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
	EEOSElement Element = EEOSElement::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost & Timing")
	float ChakraCost = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost & Timing")
	float CooldownSeconds = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost & Timing")
	float CastTimeSeconds = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DamageMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hand-Sign Trial")
	FEOSStableId HandSignSequenceId;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EOSAbility", AbilityId.StableId);
	}
};
