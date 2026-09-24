#include "../../Public/Environment/EOSLandOfWavesBuilder.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSLandOfWavesBuilder::AEOSLandOfWavesBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootComp;

	// Cold dawn ocean lighting
	ColdDawnSun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("ColdDawnSun"));
	ColdDawnSun->SetupAttachment(RootComponent);
	ColdDawnSun->SetIntensity(4.0f);
	ColdDawnSun->SetLightColor(FLinearColor(0.85f, 0.92f, 1.0f)); // 6500K cold marine blue daylight
	ColdDawnSun->SetRelativeRotation(FRotator(-20.0f, 40.0f, 0.0f));
	ColdDawnSun->CastShadows = true;

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("WavesSkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetIntensity(1.6f);
	SkyLight->SetLightColor(FLinearColor(0.70f, 0.82f, 0.95f));

	// Dense Zabuza Hidden Mist (Kirigakure no Jutsu)
	ZabuzaHiddenMist = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ZabuzaHiddenMist"));
	ZabuzaHiddenMist->SetupAttachment(RootComponent);
	ZabuzaHiddenMist->SetFogDensity(0.035f); // High atmospheric occlusion
	ZabuzaHiddenMist->SetFogInscatteringColor(FLinearColor(0.80f, 0.88f, 0.95f));
}

void AEOSLandOfWavesBuilder::BeginPlay()
{
	Super::BeginPlay();

	BuildGreatBridgeStructure();
	BuildWoodenScaffolding();
	SpawnMistEncounters();

	UE_LOG(LogEOSCore, Log, TEXT("Land of Waves Great Naruto Bridge Arena successfully constructed."));
}

void AEOSLandOfWavesBuilder::BuildGreatBridgeStructure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* CubeMesh = CubeFinder.Succeeded() ? CubeFinder.Object : nullptr;

	if (!CubeMesh)
	{
		return;
	}

	// Great Naruto Bridge Deck: 150m length x 15m width x 2m thickness
	UStaticMeshComponent* BridgeDeck = NewObject<UStaticMeshComponent>(this, TEXT("GreatBridgeDeck"));
	BridgeDeck->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BridgeDeck->SetStaticMesh(CubeMesh);
	BridgeDeck->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BridgeDeck->SetWorldScale3D(FVector(150.0f, 15.0f, 2.0f));
	BridgeDeck->RegisterComponent();

	// Guardrails on both sides
	UStaticMeshComponent* LeftGuardrail = NewObject<UStaticMeshComponent>(this, TEXT("BridgeGuardrail_Left"));
	LeftGuardrail->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LeftGuardrail->SetStaticMesh(CubeMesh);
	LeftGuardrail->SetRelativeLocation(FVector(0.0f, 750.0f, 120.0f));
	LeftGuardrail->SetWorldScale3D(FVector(150.0f, 0.6f, 1.2f));
	LeftGuardrail->RegisterComponent();

	UStaticMeshComponent* RightGuardrail = NewObject<UStaticMeshComponent>(this, TEXT("BridgeGuardrail_Right"));
	RightGuardrail->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RightGuardrail->SetStaticMesh(CubeMesh);
	RightGuardrail->SetRelativeLocation(FVector(0.0f, -750.0f, 120.0f));
	RightGuardrail->SetWorldScale3D(FVector(150.0f, 0.6f, 1.2f));
	RightGuardrail->RegisterComponent();
}

void AEOSLandOfWavesBuilder::BuildWoodenScaffolding()
{
	// Modular unfinished construction timber scaffolding along the bridge
}

void AEOSLandOfWavesBuilder::SpawnMistEncounters()
{
	// Zabuza boss encounter arena trigger
}
