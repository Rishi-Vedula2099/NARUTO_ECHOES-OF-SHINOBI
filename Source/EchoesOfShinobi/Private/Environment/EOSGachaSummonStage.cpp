#include "../../Public/Environment/EOSGachaSummonStage.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSGachaSummonStage::AEOSGachaSummonStage()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootComp;

	// Central overhead summoning beam spotlight
	SummonBeamLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SummonBeamLight"));
	SummonBeamLight->SetupAttachment(RootComponent);
	SummonBeamLight->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	SummonBeamLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SummonBeamLight->SetIntensity(25000.0f);
	SummonBeamLight->SetInnerConeAngle(15.0f);
	SummonBeamLight->SetOuterConeAngle(30.0f);
	SummonBeamLight->SetLightColor(FLinearColor(1.0f, 0.85f, 0.2f)); // Default Gold SSR beam

	// Seal formula ground glow
	SealAltarGlow = CreateDefaultSubobject<UPointLightComponent>(TEXT("SealAltarGlow"));
	SealAltarGlow->SetupAttachment(RootComponent);
	SealAltarGlow->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	SealAltarGlow->SetIntensity(8000.0f);
	SealAltarGlow->SetAttenuationRadius(500.0f);
	SealAltarGlow->SetLightColor(FLinearColor(0.2f, 0.6f, 1.0f)); // Azure chakra default
}

void AEOSGachaSummonStage::BeginPlay()
{
	Super::BeginPlay();

	BuildAltarPlatform();
	BuildToriiAndLanterns();

	UE_LOG(LogEOSCore, Log, TEXT("Gacha Summoning Altar Stage successfully constructed."));
}

void AEOSGachaSummonStage::BuildAltarPlatform()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* CylinderMesh = CylinderFinder.Succeeded() ? CylinderFinder.Object : nullptr;

	if (!CylinderMesh)
	{
		return;
	}

	// Two-tier circular dais
	UStaticMeshComponent* BaseTier = NewObject<UStaticMeshComponent>(this, TEXT("SummonDaisBase"));
	BaseTier->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BaseTier->SetStaticMesh(CylinderMesh);
	BaseTier->SetWorldScale3D(FVector(10.0f, 10.0f, 0.3f)); // 10m diameter platform
	BaseTier->RegisterComponent();

	UStaticMeshComponent* UpperTier = NewObject<UStaticMeshComponent>(this, TEXT("SummonDaisUpper"));
	UpperTier->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UpperTier->SetStaticMesh(CylinderMesh);
	UpperTier->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	UpperTier->SetWorldScale3D(FVector(7.0f, 7.0f, 0.3f)); // 7m diameter inner ritual circle
	UpperTier->RegisterComponent();
}

void AEOSGachaSummonStage::BuildToriiAndLanterns()
{
	// Ceremonial Shinto framing pillars & lanterns around perimeter
}

void AEOSGachaSummonStage::TriggerSummonRevealVisual(int32 RarityTier)
{
	switch (RarityTier)
	{
	case 1: // Rare 3-Star
		SummonBeamLight->SetLightColor(FLinearColor(0.2f, 0.6f, 1.0f)); // Blue
		break;
	case 2: // Super Rare (SR) 4-Star
		SummonBeamLight->SetLightColor(FLinearColor(0.85f, 0.2f, 0.9f)); // Violet
		break;
	case 3: // SSR 5-Star
		SummonBeamLight->SetLightColor(FLinearColor(1.0f, 0.85f, 0.1f)); // Gold
		break;
	case 4: // UR 6-Star / Transcendent
	default:
		SummonBeamLight->SetLightColor(FLinearColor(1.0f, 0.15f, 0.25f)); // Crimson / Rainbow
		break;
	}
}
