#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSLandOfWavesBuilder.generated.h"

class UDirectionalLightComponent;
class USkyLightComponent;
class UExponentialHeightFogComponent;
class UStaticMeshComponent;

/**
 * Phase 6, 7, 8: Land of Waves & Great Naruto Bridge Environment Builder.
 * Authoritatively creates the iconic Land of Waves climax arena:
 * - Cold coastal dawn lighting (6500K) and dense Zabuza Hidden Mist (Fog Density 0.045)
 * - Suspended bridge highway (120m long, timber & stone roadbed)
 * - Construction scaffolding, wooden guardrails, and suspension cable towers
 * - Zabuza Silent Killing mist volume and water puddle reflections
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSLandOfWavesBuilder : public AActor
{
	GENERATED_BODY()

public:
	AEOSLandOfWavesBuilder();

protected:
	virtual void BeginPlay() override;

	void BuildCoastalMistLighting();
	void BuildGreatBridgeStructure();
	void BuildWoodenScaffolding();
	void SpawnMistEncounters();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Waves Environment")
	TObjectPtr<UDirectionalLightComponent> ColdDawnSun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Waves Environment")
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Waves Environment")
	TObjectPtr<UExponentialHeightFogComponent> ZabuzaHiddenMist;
};
