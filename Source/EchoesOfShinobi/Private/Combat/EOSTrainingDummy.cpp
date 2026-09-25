#include "../../Public/Combat/EOSTrainingDummy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../Public/UI/EOSShinobiHUD.h"
#include "Kismet/GameplayStatics.h"
#include "../../EchoesOfShinobi.h"

AEOSTrainingDummy::AEOSTrainingDummy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(45.0f, 110.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleComponent;

	PostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostMesh"));
	PostMesh->SetupAttachment(RootComponent);
	PostMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	PostMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 2.2f));

	TargetBoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetBoardMesh"));
	TargetBoardMesh->SetupAttachment(PostMesh);
	TargetBoardMesh->SetRelativeLocation(FVector(20.0f, 0.0f, 60.0f));
	TargetBoardMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	TargetBoardMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.2f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderFinder.Succeeded())
	{
		PostMesh->SetStaticMesh(CylinderFinder.Object);
		TargetBoardMesh->SetStaticMesh(CylinderFinder.Object);
	}
}

void AEOSTrainingDummy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	BaseRotation = GetActorRotation();
}

void AEOSTrainingDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WobbleTimer > 0.0f)
	{
		WobbleTimer -= DeltaTime;
		const float WobbleAngle = FMath::Sin(WobbleTimer * 28.0f) * (WobbleTimer * 14.0f);
		SetActorRotation(BaseRotation + FRotator(WobbleAngle, 0.0f, WobbleAngle * 0.5f));
	}
	else
	{
		SetActorRotation(BaseRotation);
	}
}

float AEOSTrainingDummy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
	WobbleTimer = 0.45f; // 450ms hit wobble

	UE_LOG(LogEOSCombat, Log, TEXT("Training Dummy received %.1f damage. Remaining HP: %.1f / %.1f"), ActualDamage, CurrentHealth, MaxHealth);

	// Update Player HUD target bar
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AEOSShinobiHUD* HUD = Cast<AEOSShinobiHUD>(PC->GetHUD()))
		{
			HUD->SetTargetBossInfo(TEXT("Kakashi Training Post - Target Board"), GetHealthPercent());
			HUD->RegisterHit(1, ActualDamage * 1.5f);
		}
	}

	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogEOSCombat, Log, TEXT("Training Dummy destroyed! Resetting post HP for continuous practice."));
		CurrentHealth = MaxHealth; // Auto-reset for continuous training
	}

	return ActualDamage;
}
