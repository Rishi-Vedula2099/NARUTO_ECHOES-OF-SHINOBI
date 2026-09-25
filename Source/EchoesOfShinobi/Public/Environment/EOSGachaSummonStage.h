#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSGachaSummonStage.generated.h"

class UPointLightComponent;
class USpotLightComponent;
class UStaticMeshComponent;

/**
 * Phase 10: Gacha Summoning Altar Visual Stage.
 * Authoritatively creates the stylized 3D ritual chamber for character summons:
 * - Obsidian stone dais with glowing cinnabar seal calligraphy formula
 * - Central giant summoning toad contract scroll pedestal
 * - Tiered summoning lightning spotlights (Blue Rare, Violet Epic, Gold SSR, Crimson UR)
 * - Shinto torii gate framing and ceremonial stone lanterns
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSGachaSummonStage : public AActor
{
	GENERATED_BODY()

public:
	AEOSGachaSummonStage();

	UFUNCTION(BlueprintCallable, Category = "EOS|Gacha Stage")
	void TriggerSummonRevealVisual(int32 RarityTier);

protected:
	virtual void BeginPlay() override;

	void BuildAltarPlatform();
	void BuildToriiAndLanterns();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Gacha Stage")
	TObjectPtr<USpotLightComponent> SummonBeamLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Gacha Stage")
	TObjectPtr<UPointLightComponent> SealAltarGlow;
};
