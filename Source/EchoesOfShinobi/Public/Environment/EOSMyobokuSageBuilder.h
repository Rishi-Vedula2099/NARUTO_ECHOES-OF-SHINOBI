#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSMyobokuSageBuilder.generated.h"

class UDirectionalLightComponent;
class USkyLightComponent;
class UExponentialHeightFogComponent;
class UStaticMeshComponent;

/**
 * Phase 4: Mount Myoboku Sage Mode Trial Environment Builder.
 * Authoritatively creates the sacred toad sanctuary for Sage Mode Trials:
 * - Amber twilight atmosphere (3200K) and emerald nature energy mist
 * - Sacred stone toad statues and balancing monolith pedestals
 * - Giant oil lily pads and reflective meditation ponds
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSMyobokuSageBuilder : public AActor
{
	GENERATED_BODY()

public:
	AEOSMyobokuSageBuilder();

protected:
	virtual void BeginPlay() override;

	void BuildSageAtmosphere();
	void BuildMeditationSanctuary();
	void BuildBalancingPedestals();
	void BuildToadMonoliths();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Sage Environment")
	TObjectPtr<UDirectionalLightComponent> AmberSunLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Sage Environment")
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Sage Environment")
	TObjectPtr<UExponentialHeightFogComponent> NatureEnergyMist;
};
