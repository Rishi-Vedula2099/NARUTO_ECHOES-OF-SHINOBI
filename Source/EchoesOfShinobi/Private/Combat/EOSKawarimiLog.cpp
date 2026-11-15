#include "../../Public/Combat/EOSKawarimiLog.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSKawarimiLog::AEOSKawarimiLog()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LogMeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetGenerateOverlapEvents(true);

	// Authored training log mesh
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LogMeshRef(TEXT("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post"));
	if (LogMeshRef.Succeeded())
	{
		MeshComponent->SetStaticMesh(LogMeshRef.Get());
	}

	static ConstructorHelpers::FObjectFinderOptional<UMaterialInterface> WoodMatRef(TEXT("/Game/Materials/Environment/MI_AnimeWood"));
	if (WoodMatRef.Succeeded())
	{
		MeshComponent->SetMaterial(0, WoodMatRef.Get());
	}

	// Smoke burst Niagara VFX
	static ConstructorHelpers::FObjectFinderOptional<UNiagaraSystem> SmokeFXRef(TEXT("/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst"));
	if (SmokeFXRef.Succeeded())
	{
		SmokeBurstFX = SmokeFXRef.Get();
	}

	InitialLifeSpan = 3.5f;
}

void AEOSKawarimiLog::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent && !MeshComponent->GetMaterial(0))
	{
		if (UMaterialInterface* WoodMat = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Materials/Environment/MI_AnimeWood"))))
		{
			MeshComponent->SetMaterial(0, WoodMat);
		}
	}

	if (SmokeBurstFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SmokeBurstFX,
			GetActorLocation() + FVector(0.0f, 0.0f, 40.0f),
			GetActorRotation(),
			FVector(2.5f, 2.5f, 2.5f)
		);
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Decoy training log spawned at %s with 3.5s lifespan and wood anime material."),
		*GetActorLocation().ToString());
}

float AEOSKawarimiLog::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI LOG] Decoy log absorbed %.1f incoming damage from %s!"),
		DamageAmount, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));
	return DamageAmount;
}

