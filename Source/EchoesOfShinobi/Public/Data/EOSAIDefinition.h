#pragma once

#include "../CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EOSAIDataTypes.h"
#include "EOSAIDefinition.generated.h"

/**
 * Data asset defining AI Boss/Minion profiles and StateTree strategy bindings.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSAIDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEOSAIDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIProfile")
	FString AIProfileID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIProfile")
	EEOSAITier AITier = EEOSAITier::MajorBoss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIProfile")
	EEOSMinionRole MinionRole = EEOSMinionRole::MeleeRush;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
