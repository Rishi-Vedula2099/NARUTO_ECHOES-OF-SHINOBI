#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSKonohaTrainingGroundsBuilder.generated.h"

class UDirectionalLightComponent;
class USkyLightComponent;
class USkyAtmosphereComponent;
class UExponentialHeightFogComponent;
class UStaticMeshComponent;

/**
 * Procedural Konoha Training Grounds Environment Builder.
 * Replaces default Unreal template presentation with an intentional shinobi training ground:
 * - Directional sunlight, skylight, atmosphere, and morning fog
 * - Ground planes with dirt training ring and stone paths
 * - Wooden training posts, ninja target boards, training platforms, boundary trees, and rocks
 * - Automatic spawn of PlayerStart and AEOSTrainingDummy
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSKonohaTrainingGroundsBuilder : public AActor
{
	GENERATED_BODY()

public:
	AEOSKonohaTrainingGroundsBuilder();

protected:
	virtual void BeginPlay() override;
	void BuildAtmosphericLighting();
	void BuildGroundAndTrainingCircle();
	void BuildTrainingPostsAndProps();
	void BuildPerimeterForestAndRocks();
	void SpawnCombatDummyAndPlayerStart();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Environment")
	TObjectPtr<UDirectionalLightComponent> DirectionalSunLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Environment")
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Environment")
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Environment")
	TObjectPtr<UExponentialHeightFogComponent> HeightFog;
};
