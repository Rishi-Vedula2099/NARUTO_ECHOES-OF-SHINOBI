#include "../../Public/Environment/EOSKonohaTrainingGroundsBuilder.h"
#include "../../Public/Combat/EOSTrainingDummy.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../../EchoesOfShinobi.h"

AEOSKonohaTrainingGroundsBuilder::AEOSKonohaTrainingGroundsBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootComp;

	// 1. Directional Sunlight (Anime Daylight Setup)
	DirectionalSunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalSunLight"));
	DirectionalSunLight->SetupAttachment(RootComponent);
	DirectionalSunLight->SetIntensity(6.5f);
	DirectionalSunLight->SetLightColor(FLinearColor(1.0f, 0.96f, 0.90f)); // Warm daylight
	DirectionalSunLight->SetRelativeRotation(FRotator(-40.0f, 50.0f, 0.0f));
	DirectionalSunLight->bAtmosphereSunLight = true;
	DirectionalSunLight->CastShadows = true;

	// 2. Sky Light (Soft ambient shadow fill)
	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetIntensity(1.5f);
	SkyLight->SetLightColor(FLinearColor(0.85f, 0.92f, 1.0f));
	SkyLight->bRealTimeCapture = true;

	// 3. Sky Atmosphere
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComponent);

	// 4. Exponential Height Fog (Konoha Forest Morning Mist)
	HeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("HeightFog"));
	HeightFog->SetupAttachment(RootComponent);
	HeightFog->SetFogDensity(0.006f);
	HeightFog->SetFogHeightFalloff(0.15f);
	HeightFog->SetFogInscatteringColor(FLinearColor(0.75f, 0.85f, 0.95f));
}

void AEOSKonohaTrainingGroundsBuilder::BeginPlay()
{
	Super::BeginPlay();

	BuildGroundAndTrainingCircle();
	BuildTrainingPostsAndProps();
	BuildPerimeterForestAndRocks();
	SpawnCombatDummyAndPlayerStart();

	UE_LOG(LogEOSCore, Log, TEXT("Konoha Training Grounds visual slice successfully constructed."));
}

void AEOSKonohaTrainingGroundsBuilder::BuildGroundAndTrainingCircle()
{
	UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));

	// Main Training Field Base (Grass Clearing)
	if (PlaneMesh)
	{
		UStaticMeshComponent* BaseClearing = NewObject<UStaticMeshComponent>(this);
		BaseClearing->SetStaticMesh(PlaneMesh);
		BaseClearing->SetupAttachment(RootComponent);
		BaseClearing->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
		BaseClearing->SetWorldScale3D(FVector(60.0f, 60.0f, 1.0f)); // 60m x 60m clearing
		BaseClearing->RegisterComponent();
	}

	// Dirt Training Circle (Worn earth ring where shinobi spar)
	if (CylinderMesh)
	{
		UStaticMeshComponent* DirtCircle = NewObject<UStaticMeshComponent>(this);
		DirtCircle->SetStaticMesh(CylinderMesh);
		DirtCircle->SetupAttachment(RootComponent);
		DirtCircle->SetWorldLocation(FVector(300.0f, 0.0f, 1.0f));
		DirtCircle->SetWorldScale3D(FVector(16.0f, 16.0f, 0.02f)); // 16m diameter sparring ring
		DirtCircle->RegisterComponent();
	}
}

void AEOSKonohaTrainingGroundsBuilder::BuildTrainingPostsAndProps()
{
	UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (!CylinderMesh)
	{
		return;
	}

	// The 3 Famous Konoha Training Log Posts (Bell Test)
	const FVector PostLocations[] = {
		FVector(350.0f, -80.0f, 0.0f),
		FVector(350.0f, 0.0f, 0.0f),
		FVector(350.0f, 80.0f, 0.0f)
	};

	for (int32 i = 0; i < 3; ++i)
	{
		UStaticMeshComponent* Post = NewObject<UStaticMeshComponent>(this);
		Post->SetStaticMesh(CylinderMesh);
		Post->SetupAttachment(RootComponent);
		Post->SetWorldLocation(PostLocations[i]);
		Post->SetWorldScale3D(FVector(0.55f, 0.55f, 2.0f));
		Post->SetCollisionProfileName(TEXT("BlockAll"));
		Post->RegisterComponent();

		// Rope Wrappings on Posts
		if (CubeMesh)
		{
			UStaticMeshComponent* Rope = NewObject<UStaticMeshComponent>(this);
			Rope->SetStaticMesh(CubeMesh);
			Rope->SetupAttachment(Post);
			Rope->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
			Rope->SetRelativeScale3D(FVector(1.15f, 1.15f, 0.15f));
			Rope->RegisterComponent();
		}
	}

	// Surrounding Target Boards
	const FVector TargetLocations[] = {
		FVector(600.0f, -250.0f, 0.0f),
		FVector(650.0f, 200.0f, 0.0f),
		FVector(500.0f, 350.0f, 0.0f)
	};

	for (int32 i = 0; i < 3; ++i)
	{
		UStaticMeshComponent* Stand = NewObject<UStaticMeshComponent>(this);
		Stand->SetStaticMesh(CylinderMesh);
		Stand->SetupAttachment(RootComponent);
		Stand->SetWorldLocation(TargetLocations[i]);
		Stand->SetWorldScale3D(FVector(0.3f, 0.3f, 1.8f));
		Stand->SetCollisionProfileName(TEXT("BlockAll"));
		Stand->RegisterComponent();

		UStaticMeshComponent* Board = NewObject<UStaticMeshComponent>(this);
		Board->SetStaticMesh(CylinderMesh);
		Board->SetupAttachment(Stand);
		Board->SetRelativeLocation(FVector(15.0f, 0.0f, 55.0f));
		Board->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		Board->SetWorldScale3D(FVector(1.2f, 1.2f, 0.15f));
		Board->RegisterComponent();
	}
}

void AEOSKonohaTrainingGroundsBuilder::BuildPerimeterForestAndRocks()
{
	UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	// Perimeter Forest Trees (Clearing boundary)
	const int32 NumTrees = 16;
	const float Radius = 1400.0f;

	for (int32 i = 0; i < NumTrees; ++i)
	{
		const float Angle = (float)i / (float)NumTrees * 2.0f * PI;
		const FVector TreePos = FVector(FMath::Cos(Angle) * Radius + 300.0f, FMath::Sin(Angle) * Radius, 0.0f);

		// Tree Trunk
		if (CylinderMesh)
		{
			UStaticMeshComponent* Trunk = NewObject<UStaticMeshComponent>(this);
			Trunk->SetStaticMesh(CylinderMesh);
			Trunk->SetupAttachment(RootComponent);
			Trunk->SetWorldLocation(TreePos);
			Trunk->SetWorldScale3D(FVector(1.2f, 1.2f, 7.0f)); // Tall forest canopy
			Trunk->SetCollisionProfileName(TEXT("BlockAll"));
			Trunk->RegisterComponent();
		}

		// Tree Foliage Canopy
		if (SphereMesh)
		{
			UStaticMeshComponent* Foliage = NewObject<UStaticMeshComponent>(this);
			Foliage->SetStaticMesh(SphereMesh);
			Foliage->SetupAttachment(RootComponent);
			Foliage->SetWorldLocation(TreePos + FVector(0.0f, 0.0f, 650.0f));
			Foliage->SetWorldScale3D(FVector(6.0f, 6.0f, 4.5f));
			Foliage->RegisterComponent();
		}
	}

	// Scattered Rocks & Boulders in Midground
	const FVector RockLocations[] = {
		FVector(200.0f, -400.0f, 0.0f),
		FVector(450.0f, -500.0f, 0.0f),
		FVector(750.0f, -100.0f, 0.0f),
		FVector(300.0f, 450.0f, 0.0f)
	};

	if (SphereMesh)
	{
		for (int32 i = 0; i < 4; ++i)
		{
			UStaticMeshComponent* Rock = NewObject<UStaticMeshComponent>(this);
			Rock->SetStaticMesh(SphereMesh);
			Rock->SetupAttachment(RootComponent);
			Rock->SetWorldLocation(RockLocations[i]);
			Rock->SetWorldScale3D(FVector(1.8f, 2.2f, 1.2f));
			Rock->SetCollisionProfileName(TEXT("BlockAll"));
			Rock->RegisterComponent();
		}
	}
}

void AEOSKonohaTrainingGroundsBuilder::SpawnCombatDummyAndPlayerStart()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 1. Spawn deterministic PlayerStart facing the training field
	TArray<AActor*> ExistingStarts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), ExistingStarts);
	if (ExistingStarts.Num() == 0)
	{
		FActorSpawnParameters StartParams;
		StartParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APlayerStart* NewStart = World->SpawnActor<APlayerStart>(FVector(-150.0f, 0.0f, 50.0f), FRotator(0.0f, 0.0f, 0.0f), StartParams);
		if (NewStart)
		{
			NewStart->Tags.Add(TEXT("KonohaTrainingStart"));
			UE_LOG(LogEOSCore, Log, TEXT("Created PlayerStart at Konoha Training Grounds facing training posts."));
		}
	}

	// 2. Spawn Interactive Training Dummy in the center of the ring
	TArray<AActor*> ExistingDummies;
	UGameplayStatics::GetAllActorsOfClass(World, AEOSTrainingDummy::StaticClass(), ExistingDummies);
	if (ExistingDummies.Num() == 0)
	{
		FActorSpawnParameters DummyParams;
		DummyParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEOSTrainingDummy* Dummy = World->SpawnActor<AEOSTrainingDummy>(FVector(250.0f, 0.0f, 100.0f), FRotator(0.0f, 180.0f, 0.0f), DummyParams);
		if (Dummy)
		{
			UE_LOG(LogEOSCore, Log, TEXT("Spawned AEOSTrainingDummy at Konoha sparring ring center."));
		}
	}
}
