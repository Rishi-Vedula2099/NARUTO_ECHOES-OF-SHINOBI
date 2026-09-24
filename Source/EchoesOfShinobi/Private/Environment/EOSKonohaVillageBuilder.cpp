#include "../../Public/Environment/EOSKonohaVillageBuilder.h"
#include "../../Public/AI/EOSVillageNPC.h"
#include "../../Public/Combat/EOSTrainingDummy.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSKonohaVillageBuilder::AEOSKonohaVillageBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("VillageDistrictRoot"));
	SetRootComponent(RootComp);

	// 1. Anime Daylight Lighting Components (~5400K Warm Sunlight + Sky Fill)
	DirectionalSunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("AnimeSunLight"));
	DirectionalSunLight->SetupAttachment(RootComp);
	DirectionalSunLight->SetRelativeRotation(FRotator(-40.0f, 35.0f, 0.0f));
	DirectionalSunLight->Intensity = 5.0f;
	DirectionalSunLight->LightColor = FColor(255, 242, 226); // Warm ~5400K anime sunlight
	DirectionalSunLight->CastShadows = true;

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("AnimeSkyLight"));
	SkyLight->SetupAttachment(RootComp);
	SkyLight->Intensity = 1.6f;
	SkyLight->LightColor = FColor(195, 225, 255); // Blue-tinted skylight shadow fill

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("AnimeSkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComp);

	HeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("AnimeAtmosphericFog"));
	HeightFog->SetupAttachment(RootComp);
	HeightFog->SetFogDensity(0.006f);
	HeightFog->SetFogInscatteringColor(FLinearColor(0.72f, 0.85f, 0.98f));

	// 2. High-Performance Instanced Static Mesh Components (GTX 1650 Budget Target)
	StoneRoadInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StoneRoadInstanced"));
	StoneRoadInstanced->SetupAttachment(RootComp);

	BuildingWallsInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BuildingWallsInstanced"));
	BuildingWallsInstanced->SetupAttachment(RootComp);

	RoofTilesInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoofTilesInstanced"));
	RoofTilesInstanced->SetupAttachment(RootComp);

	TimberBeamsInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TimberBeamsInstanced"));
	TimberBeamsInstanced->SetupAttachment(RootComp);

	LanternsInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("LanternsInstanced"));
	LanternsInstanced->SetupAttachment(RootComp);

	SakuraTreesInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SakuraTreesInstanced"));
	SakuraTreesInstanced->SetupAttachment(RootComp);

	// Load engine basic shapes for instanced geometry representations
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneFinder(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

	if (CubeFinder.Succeeded())
	{
		BuildingWallsInstanced->SetStaticMesh(CubeFinder.Object);
		RoofTilesInstanced->SetStaticMesh(CubeFinder.Object);
		TimberBeamsInstanced->SetStaticMesh(CubeFinder.Object);
		LanternsInstanced->SetStaticMesh(CubeFinder.Object);
	}
	if (PlaneFinder.Succeeded())
	{
		StoneRoadInstanced->SetStaticMesh(PlaneFinder.Object);
	}
	if (CylinderFinder.Succeeded())
	{
		SakuraTreesInstanced->SetStaticMesh(CylinderFinder.Object);
	}
}

void AEOSKonohaVillageBuilder::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("CONSTRUCTING AUTHORED KONOHA VERTICAL-SLICE DISTRICT"));
	UE_LOG(LogEOSCore, Log, TEXT("Target Scale: 500m x 500m | Budget: GTX 1650 60 FPS"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	BuildAnimeDaylightLighting();
	BuildHokageRockMonumentVista();
	BuildMainStreetAndPlaza();
	BuildIchirakuRamenAndShops();
	BuildResidentialBuildings();
	BuildVillageGateAndDispatchPost();
	BuildBannersLanternsAndStreetProps();
	BuildSakuraFoliageAndVegetation();
	SpawnLivingVillageNPCs();
	SpawnVerticalSlicePlayerStartAndBossEncounter();

	UE_LOG(LogEOSCore, Log, TEXT("Konoha District Construction Complete: %d Buildings, %d Props, %d Foliage Elements."),
		TotalBuildingCount, TotalPropCount, TotalFoliageCount);
}

void AEOSKonohaVillageBuilder::BuildAnimeDaylightLighting()
{
	// Configure stepped anime lighting profile
	if (DirectionalSunLight)
	{
		DirectionalSunLight->SetIntensity(5.0f);
		DirectionalSunLight->SetLightColor(FColor(255, 242, 226));
	}
}

void AEOSKonohaVillageBuilder::BuildHokageRockMonumentVista()
{
	// Majestically towering Hokage Rock cliff face on the distant horizon
	// Positioned at X=16000, Y=0, Z=1800, towering 800m tall behind Konoha
	if (BuildingWallsInstanced)
	{
		FTransform MonumentTransform;
		MonumentTransform.SetLocation(FVector(16000.0f, 0.0f, 1800.0f));
		MonumentTransform.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		MonumentTransform.SetScale3D(FVector(30.0f, 160.0f, 40.0f));
		BuildingWallsInstanced->AddInstance(MonumentTransform);
		TotalBuildingCount++;
	}
}

void AEOSKonohaVillageBuilder::BuildMainStreetAndPlaza()
{
	// 500m x 500m district stone road network
	// Central avenue spanning X = -1000 to +3000, Y = -250 to +250
	if (StoneRoadInstanced)
	{
		for (float X = -1000.0f; X <= 3500.0f; X += 500.0f)
		{
			FTransform RoadTransform;
			RoadTransform.SetLocation(FVector(X, 0.0f, 2.0f));
			RoadTransform.SetRotation(FQuat::Identity);
			RoadTransform.SetScale3D(FVector(5.2f, 12.0f, 1.0f));
			StoneRoadInstanced->AddInstance(RoadTransform);
			TotalPropCount++;
		}
	}
}

void AEOSKonohaVillageBuilder::BuildIchirakuRamenAndShops()
{
	// Ichiraku Ramen Shop positioned on Main Street
	const FVector RamenShopLoc = FVector(800.0f, 450.0f, 0.0f);

	if (BuildingWallsInstanced)
	{
		// Shop main structure
		FTransform ShopBase;
		ShopBase.SetLocation(RamenShopLoc + FVector(0.0f, 0.0f, 160.0f));
		ShopBase.SetScale3D(FVector(8.0f, 7.0f, 3.2f));
		BuildingWallsInstanced->AddInstance(ShopBase);

		// Wooden Counter
		FTransform Counter;
		Counter.SetLocation(RamenShopLoc + FVector(0.0f, -240.0f, 50.0f));
		Counter.SetScale3D(FVector(6.0f, 1.2f, 1.0f));
		BuildingWallsInstanced->AddInstance(Counter);
	}

	if (RoofTilesInstanced)
	{
		// Traditional curved orange roof
		FTransform Roof;
		Roof.SetLocation(RamenShopLoc + FVector(0.0f, -40.0f, 340.0f));
		Roof.SetRotation(FQuat(FRotator(12.0f, 0.0f, 0.0f)));
		Roof.SetScale3D(FVector(9.0f, 8.5f, 0.6f));
		RoofTilesInstanced->AddInstance(Roof);
	}

	TotalBuildingCount += 2;
}

void AEOSKonohaVillageBuilder::BuildResidentialBuildings()
{
	// Authored 2-story modular Japanese buildings flanking Main Street
	// Left side (Y = -550 to -900) & Right side (Y = +550 to +900)
	for (float X = -800.0f; X <= 3200.0f; X += 650.0f)
	{
		// Left side building
		const FVector LeftLoc = FVector(X, -700.0f, 200.0f);
		if (BuildingWallsInstanced)
		{
			FTransform WallTransform;
			WallTransform.SetLocation(LeftLoc);
			WallTransform.SetScale3D(FVector(5.5f, 6.0f, 4.0f));
			BuildingWallsInstanced->AddInstance(WallTransform);
		}
		if (RoofTilesInstanced)
		{
			FTransform RoofTransform;
			RoofTransform.SetLocation(LeftLoc + FVector(0.0f, 0.0f, 220.0f));
			RoofTransform.SetRotation(FQuat(FRotator(15.0f, 0.0f, 0.0f)));
			RoofTransform.SetScale3D(FVector(6.2f, 7.0f, 0.7f));
			RoofTilesInstanced->AddInstance(RoofTransform);
		}

		// Right side building
		const FVector RightLoc = FVector(X + 250.0f, 750.0f, 200.0f);
		if (BuildingWallsInstanced)
		{
			FTransform WallTransform;
			WallTransform.SetLocation(RightLoc);
			WallTransform.SetScale3D(FVector(5.5f, 6.0f, 4.0f));
			BuildingWallsInstanced->AddInstance(WallTransform);
		}
		if (RoofTilesInstanced)
		{
			FTransform RoofTransform;
			RoofTransform.SetLocation(RightLoc + FVector(0.0f, 0.0f, 220.0f));
			RoofTransform.SetRotation(FQuat(FRotator(-15.0f, 0.0f, 0.0f)));
			RoofTransform.SetScale3D(FVector(6.2f, 7.0f, 0.7f));
			RoofTilesInstanced->AddInstance(RoofTransform);
		}

		TotalBuildingCount += 2;
	}
}

void AEOSKonohaVillageBuilder::BuildVillageGateAndDispatchPost()
{
	// Main Konoha Entrance Gate at X = -1200
	const FVector GateLoc = FVector(-1200.0f, 0.0f, 0.0f);

	if (TimberBeamsInstanced)
	{
		// Left gate pillar
		FTransform PillarLeft;
		PillarLeft.SetLocation(GateLoc + FVector(0.0f, -400.0f, 350.0f));
		PillarLeft.SetScale3D(FVector(1.5f, 1.5f, 7.0f));
		TimberBeamsInstanced->AddInstance(PillarLeft);

		// Right gate pillar
		FTransform PillarRight;
		PillarRight.SetLocation(GateLoc + FVector(0.0f, 400.0f, 350.0f));
		PillarRight.SetScale3D(FVector(1.5f, 1.5f, 7.0f));
		TimberBeamsInstanced->AddInstance(PillarRight);

		// Crossbeam
		FTransform Crossbeam;
		Crossbeam.SetLocation(GateLoc + FVector(0.0f, 0.0f, 680.0f));
		Crossbeam.SetScale3D(FVector(1.6f, 10.0f, 1.2f));
		TimberBeamsInstanced->AddInstance(Crossbeam);
	}

	if (RoofTilesInstanced)
	{
		FTransform GateRoof;
		GateRoof.SetLocation(GateLoc + FVector(0.0f, 0.0f, 750.0f));
		GateRoof.SetScale3D(FVector(3.0f, 11.5f, 0.8f));
		RoofTilesInstanced->AddInstance(GateRoof);
	}

	TotalBuildingCount += 1;
}

void AEOSKonohaVillageBuilder::BuildBannersLanternsAndStreetProps()
{
	// Hanging lanterns along Main Street
	if (LanternsInstanced)
	{
		for (float X = -800.0f; X <= 3000.0f; X += 350.0f)
		{
			// Left side lantern
			FTransform LanternL;
			LanternL.SetLocation(FVector(X, -280.0f, 240.0f));
			LanternL.SetScale3D(FVector(0.4f, 0.4f, 0.8f));
			LanternsInstanced->AddInstance(LanternL);

			// Right side lantern
			FTransform LanternR;
			LanternR.SetLocation(FVector(X, 280.0f, 240.0f));
			LanternR.SetScale3D(FVector(0.4f, 0.4f, 0.8f));
			LanternsInstanced->AddInstance(LanternR);

			TotalPropCount += 2;
		}
	}
}

void AEOSKonohaVillageBuilder::BuildSakuraFoliageAndVegetation()
{
	// Sakura (Cherry Blossom) trees lining plazas and street edges
	if (SakuraTreesInstanced)
	{
		const TArray<FVector> TreeLocations = {
			FVector(-600.0f, -380.0f, 0.0f),
			FVector(-100.0f, -400.0f, 0.0f),
			FVector(400.0f, -420.0f, 0.0f),
			FVector(1100.0f, -380.0f, 0.0f),
			FVector(1800.0f, -410.0f, 0.0f),
			FVector(2400.0f, -390.0f, 0.0f),
			FVector(-400.0f, 400.0f, 0.0f),
			FVector(200.0f, 420.0f, 0.0f),
			FVector(1400.0f, 430.0f, 0.0f),
			FVector(2100.0f, 410.0f, 0.0f)
		};

		for (const FVector& Loc : TreeLocations)
		{
			FTransform TreeTransform;
			TreeTransform.SetLocation(Loc + FVector(0.0f, 0.0f, 250.0f));
			TreeTransform.SetScale3D(FVector(2.0f, 2.0f, 5.0f));
			SakuraTreesInstanced->AddInstance(TreeTransform);
			TotalFoliageCount++;
		}
	}
}

void AEOSKonohaVillageBuilder::SpawnLivingVillageNPCs()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 1. Chunin Gate Guard (Mission Dispatch Handoff)
	AEOSVillageNPC* ChuninGuard = World->SpawnActor<AEOSVillageNPC>(
		FVector(-1000.0f, 150.0f, 90.0f), FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
	if (ChuninGuard)
	{
		ChuninGuard->NPCType = EEOSNPCType::ChuninGuard;
		ChuninGuard->CurrentBehavior = EEOSNPCBehavior::Guarding_Gate;
		ChuninGuard->Tags.Add(TEXT("MissionHandoffNPC"));
	}

	// 2. Teuchi at Ichiraku Ramen
	AEOSVillageNPC* Teuchi = World->SpawnActor<AEOSVillageNPC>(
		FVector(800.0f, 260.0f, 90.0f), FRotator(0.0f, -90.0f, 0.0f), SpawnParams);
	if (Teuchi)
	{
		Teuchi->NPCType = EEOSNPCType::Shopkeeper;
		Teuchi->CurrentBehavior = EEOSNPCBehavior::Inspecting_Shop;
	}

	// 3. Strolling Villager
	AEOSVillageNPC* Villager1 = World->SpawnActor<AEOSVillageNPC>(
		FVector(300.0f, -120.0f, 90.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	if (Villager1)
	{
		Villager1->NPCType = EEOSNPCType::Villager;
		Villager1->CurrentBehavior = EEOSNPCBehavior::Patrolling_Road;
	}

	// 4. Academy Student
	AEOSVillageNPC* Student = World->SpawnActor<AEOSVillageNPC>(
		FVector(1200.0f, -80.0f, 90.0f), FRotator(0.0f, 45.0f, 0.0f), SpawnParams);
	if (Student)
	{
		Student->NPCType = EEOSNPCType::AcademyStudent;
		Student->CurrentBehavior = EEOSNPCBehavior::Idle_Talking;
	}
}

void AEOSKonohaVillageBuilder::SpawnVerticalSlicePlayerStartAndBossEncounter()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Spawn deterministic PlayerStart at Konoha Main Street facing down the avenue
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayerStart* Start = World->SpawnActor<APlayerStart>(
		FVector(-800.0f, 0.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	if (Start)
	{
		Start->Tags.Add(TEXT("KonohaTrainingStart"));
		Start->Tags.Add(TEXT("KonohaMainStreetStart"));
	}

	// Spawn Combat Encounter & Boss Target at the Training Arena area (X = 2800)
	AEOSTrainingDummy* Dummy = World->SpawnActor<AEOSTrainingDummy>(
		FVector(2800.0f, 0.0f, 90.0f), FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
	if (Dummy)
	{
		Dummy->Tags.Add(TEXT("BOSS_KAKASHI_SHARINGAN"));
	}
}
