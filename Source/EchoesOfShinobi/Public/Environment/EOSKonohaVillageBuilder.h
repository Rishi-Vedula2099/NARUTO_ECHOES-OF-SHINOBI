#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/EOSPresentationDataTypes.h"
#include "EOSKonohaVillageBuilder.generated.h"

class UDirectionalLightComponent;
class USkyLightComponent;
class USkyAtmosphereComponent;
class UExponentialHeightFogComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;

/**
 * Procedural/Authored Konoha Vertical-Slice Village District Builder (500m x 500m).
 * Fully reconstructs the visual presentation into an authored anime shinobi world:
 * - Warm ~5400K directional anime sunlight, sky fill, soft toon shadow bands
 * - Majestic Hokage Rock Monument cliff face towering on the horizon (no empty horizons)
 * - Main Street stone pavement, Ichiraku Ramen shop, 2-story modular Japanese buildings
 * - Terracotta roof tiles, timber posts, Konoha banners, hanging lanterns, Sakura trees
 * - Deterministic PlayerStart and mission dispatch handoff point
 * - Eliminates all TECHNICAL_DEBUG_ASSETS from player camera path.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSKonohaVillageBuilder : public AActor
{
	GENERATED_BODY()

public:
	AEOSKonohaVillageBuilder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village")
	FEOSPerformanceProfile PerformanceProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Village")
	int32 TotalBuildingCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Village")
	int32 TotalPropCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Village")
	int32 TotalFoliageCount = 0;

	// VISUAL-RECTIFICATION-MASTER-1.1: production asset soft references (registry-canonical paths).
	// These resolve at BeginPlay; until authored meshes are imported into /Game/, the builder
	// keeps Engine/BasicShapes graybox strictly tagged TECHNICAL_DEBUG_ASSET and logs
	// deterministic MISSING_PRODUCTION_ASSET errors. Visual runtime activation stays BLOCKED.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_ResidenceA; // /Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_ResidenceB; // /Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_IchirakuRamen; // /Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_RoadStone; // /Game/Environment/Konoha/Roads/SM_KonohaRoadStone

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_StoneLantern; // /Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Village|ProductionAssets")
	TSoftObjectPtr<UStaticMesh> ProductionMesh_SakuraTree; // /Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom

	// True when ALL production slots resolved to authored /Game/ packages (runtime activation unblocked).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Village")
	bool bUsingProductionMeshes = false;

	// Slot IDs that failed to resolve; drives the TECHNICAL_DEBUG_ASSET tag & MISSING_PRODUCTION_ASSET logs.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Village")
	TArray<FString> MissingProductionMeshIDs;

protected:
	virtual void BeginPlay() override;

	/** Attempt to bind every production soft reference; returns true only if ALL slots resolved. */
	bool ResolveProductionMeshes();

	/** Bind resolved production meshes onto the instanced components (graybox stays as fallback). */
	void ApplyResolvedProductionMeshes();

	void BuildAnimeDaylightLighting();
	void BuildHokageRockMonumentVista();
	void BuildMainStreetAndPlaza();
	void BuildIchirakuRamenAndShops();
	void BuildResidentialBuildings();
	void BuildVillageGateAndDispatchPost();
	void BuildBannersLanternsAndStreetProps();
	void BuildSakuraFoliageAndVegetation();
	void SpawnLivingVillageNPCs();
	void SpawnVerticalSlicePlayerStartAndBossEncounter();

	// Lighting & Atmosphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Lighting")
	TObjectPtr<UDirectionalLightComponent> DirectionalSunLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Lighting")
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Lighting")
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Lighting")
	TObjectPtr<UExponentialHeightFogComponent> HeightFog;

	// Instanced Mesh Components for high performance (GTX 1650 budget)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> BuildingWallsInstanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> RoofTilesInstanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> TimberBeamsInstanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> StoneRoadInstanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> LanternsInstanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Instanced")
	TObjectPtr<UInstancedStaticMeshComponent> SakuraTreesInstanced;
};
