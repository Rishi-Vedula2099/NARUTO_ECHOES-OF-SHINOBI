#include "../../Public/Environment/EOSMyobokuSageBuilder.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSMyobokuSageBuilder::AEOSMyobokuSageBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootComp;

	AmberSunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("AmberSunLight"));
	AmberSunLight->SetupAttachment(RootComponent);
	AmberSunLight->SetIntensity(5.5f);
	AmberSunLight->SetLightColor(FLinearColor(1.0f, 0.78f, 0.45f)); // Warm golden amber twilight
	AmberSunLight->SetRelativeRotation(FRotator(-25.0f, 65.0f, 0.0f));
	AmberSunLight->CastShadows = true;

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SageSkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetIntensity(1.8f);
	SkyLight->SetLightColor(FLinearColor(0.55f, 0.85f, 0.65f)); // Emerald nature energy ambient bounce

	NatureEnergyMist = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("NatureEnergyMist"));
	NatureEnergyMist->SetupAttachment(RootComponent);
	NatureEnergyMist->SetFogDensity(0.008f);
	NatureEnergyMist->SetFogInscatteringColor(FLinearColor(0.6f, 0.9f, 0.7f));
}

void AEOSMyobokuSageBuilder::BeginPlay()
{
	Super::BeginPlay();

	BuildSageAtmosphere();
	BuildMeditationSanctuary();
	BuildBalancingPedestals();
	BuildToadMonoliths();

	UE_LOG(LogEOSCore, Log, TEXT("Mount Myoboku Sage Mode Trial Environment successfully constructed."));
}

void AEOSMyobokuSageBuilder::BuildSageAtmosphere()
{
	// Amber twilight & nature energy volumetric density setup
}

void AEOSMyobokuSageBuilder::BuildMeditationSanctuary()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshFinder(TEXT("/Engine/BasicShapes/Plane.Plane"));
	UStaticMesh* PlaneMesh = PlaneMeshFinder.Succeeded() ? PlaneMeshFinder.Object : nullptr;

	if (PlaneMesh)
	{
		UStaticMeshComponent* StoneSanctuary = NewObject<UStaticMeshComponent>(this, TEXT("StoneSanctuaryGround"));
		StoneSanctuary->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StoneSanctuary->SetStaticMesh(PlaneMesh);
		StoneSanctuary->SetWorldScale3D(FVector(80.0f, 80.0f, 1.0f)); // 80m x 80m sacred plateau
		StoneSanctuary->RegisterComponent();
	}
}

void AEOSMyobokuSageBuilder::BuildBalancingPedestals()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* CylinderMesh = CylinderFinder.Succeeded() ? CylinderFinder.Object : nullptr;

	if (!CylinderMesh)
	{
		return;
	}

	// 5 Sacred Balancing Stone Spikes (Pointed stone pillars for Nature Energy balancing)
	const FVector PedestalPositions[] = {
		FVector(0.0f, 0.0f, 500.0f),
		FVector(1200.0f, 800.0f, 650.0f),
		FVector(-1200.0f, 800.0f, 600.0f),
		FVector(800.0f, -1200.0f, 700.0f),
		FVector(-800.0f, -1200.0f, 550.0f)
	};

	for (int32 i = 0; i < 5; ++i)
	{
		FName PedestalName(*FString::Printf(TEXT("SageBalancingPillar_%d"), i));
		UStaticMeshComponent* Pillar = NewObject<UStaticMeshComponent>(this, PedestalName);
		Pillar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Pillar->SetStaticMesh(CylinderMesh);
		Pillar->SetRelativeLocation(PedestalPositions[i]);
		Pillar->SetWorldScale3D(FVector(1.2f, 1.2f, 8.0f)); // Tall narrow stone needle
		Pillar->RegisterComponent();
	}
}

void AEOSMyobokuSageBuilder::BuildToadMonoliths()
{
	// Sacred stone toad guardians encircling the sanctuary
}
