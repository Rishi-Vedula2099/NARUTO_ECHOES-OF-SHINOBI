#include "../../Public/Combat/EOSShadowClone.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Combat/EOSBossCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSShadowClone::AEOSShadowClone()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(38.0f, 85.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 550.0f;
	}

	// Distinctive Cyan Chakra point light aura for visual identification
	ChakraGlowComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("ChakraGlowComponent"));
	ChakraGlowComponent->SetupAttachment(RootComponent);
	ChakraGlowComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	ChakraGlowComponent->SetLightColor(FLinearColor(0.2f, 0.7f, 1.0f));
	ChakraGlowComponent->SetIntensity(2500.0f);
	ChakraGlowComponent->SetAttenuationRadius(250.0f);
	ChakraGlowComponent->SetCastShadows(false);

	// Visual proxy skeletal mesh matching Naruto Genin
	static ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> MeshRef(
		TEXT("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"));
	if (MeshRef.Succeeded() && GetMesh())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Get());
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	// Smoke burst Niagara VFX
	static ConstructorHelpers::FObjectFinderOptional<UNiagaraSystem> SmokeFXRef(
		TEXT("/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst"));
	if (SmokeFXRef.Succeeded())
	{
		SmokeBurstFX = SmokeFXRef.Get();
	}
}

void AEOSShadowClone::BeginPlay()
{
	Super::BeginPlay();

	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		SkelMesh->SetRenderCustomDepth(true);
		SkelMesh->SetCustomDepthStencilValue(CloneIndex == 1 ? 250 : 251);
	}

	if (SmokeBurstFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SmokeBurstFX,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void AEOSShadowClone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_AttackLoop);
		World->GetTimerManager().ClearTimer(TimerHandle_Lifetime);
	}

	Super::EndPlay(EndPlayReason);
}

void AEOSShadowClone::InitializeClone(AEOSCharacterBase* InSummoner, const FVector& SpawnOffset)
{
	SummonerShinobi = InSummoner;

	if (UWorld* World = GetWorld())
	{
		// 6.0 seconds lifetime then clean poof
		World->GetTimerManager().SetTimer(
			TimerHandle_Lifetime,
			this,
			&AEOSShadowClone::DisappearWithSmoke,
			CloneLifetime,
			false
		);

		// Tactical attack loop (every 1.2s)
		World->GetTimerManager().SetTimer(
			TimerHandle_AttackLoop,
			this,
			&AEOSShadowClone::ExecuteCloneAttack,
			1.2f,
			true,
			0.6f // Initial delay
		);
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[SHADOW CLONE] Clone spawned at %s with %.1fs lifespan (Summoner: %s)."),
		*GetActorLocation().ToString(), CloneLifetime, InSummoner ? *InSummoner->GetName() : TEXT("None"));
}

void AEOSShadowClone::SetAssignedTarget(AActor* InTarget)
{
	CurrentTarget = InTarget;
	if (InTarget)
	{
		UE_LOG(LogEOSCombat, Log, TEXT("[SHADOW CLONE %d] Assigned target: %s (Location: %s)"),
			CloneIndex, *InTarget->GetName(), *InTarget->GetActorLocation().ToString());
	}
}

void AEOSShadowClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDisappearing) return;

	// Search for nearest enemy if target invalid
	if (!CurrentTarget.IsValid())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

		float BestDistSq = MAX_FLT;
		AActor* BestTarget = nullptr;

		for (AActor* Candidate : FoundActors)
		{
			if (!Candidate || Candidate == this || Candidate == SummonerShinobi.Get() || Candidate->IsA<AEOSShadowClone>())
			{
				continue;
			}

			// Check if actor is an enemy / boss / dummy
			const bool bIsEnemy = Candidate->IsA<AEOSBossCharacter>() ||
				Candidate->ActorHasTag(TEXT("Enemy")) ||
				Candidate->GetName().Contains(TEXT("MistScout")) ||
				Candidate->GetName().Contains(TEXT("Zabuza")) ||
				Candidate->GetName().Contains(TEXT("Dummy"));

			if (bIsEnemy)
			{
				const float DistSq = FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation());
				if (DistSq < BestDistSq && DistSq < (1800.0f * 1800.0f))
				{
					BestDistSq = DistSq;
					BestTarget = Candidate;
				}
			}
		}

		if (BestTarget)
		{
			CurrentTarget = BestTarget;
		}
	}

	// Move toward target
	if (CurrentTarget.IsValid())
	{
		const FVector Direction = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		const float Dist = FVector::Dist2D(GetActorLocation(), CurrentTarget->GetActorLocation());

		if (Dist > 160.0f)
		{
			AddMovementInput(Direction, 1.0f);
		}
	}
}

void AEOSShadowClone::ExecuteCloneAttack()
{
	if (bIsDisappearing || !CurrentTarget.IsValid()) return;

	const float Dist = FVector::Dist2D(GetActorLocation(), CurrentTarget->GetActorLocation());
	if (Dist <= 320.0f)
	{
		// Deal 25% damage contribution (capped at 15.0 to prevent boss-killing exploit)
		const float CappedDamage = FMath::Clamp(AttackDamage, 5.0f, 15.0f);

		UGameplayStatics::ApplyDamage(
			CurrentTarget.Get(),
			CappedDamage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);

		UE_LOG(LogEOSCombat, Log, TEXT("[SHADOW CLONE %d] Clone struck target %s for %.1f damage! Aggro successfully drawn."),
			CloneIndex, *CurrentTarget->GetName(), CappedDamage);
	}
}

float AEOSShadowClone::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDisappearing) return 0.0f;

	CloneHealth -= DamageAmount;
	UE_LOG(LogEOSCombat, Log, TEXT("[SHADOW CLONE] Clone %s took %.1f damage (Remaining HP: %.1f)."),
		*GetName(), DamageAmount, CloneHealth);

	if (CloneHealth <= 0.0f)
	{
		DisappearWithSmoke();
	}

	return DamageAmount;
}

void AEOSShadowClone::DisappearWithSmoke()
{
	if (bIsDisappearing) return;
	bIsDisappearing = true;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_AttackLoop);
		World->GetTimerManager().ClearTimer(TimerHandle_Lifetime);

		if (SmokeBurstFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				SmokeBurstFX,
				GetActorLocation(),
				GetActorRotation()
			);
		}
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[SHADOW CLONE] Clone %s poofed away cleanly in smoke."), *GetName());
	Destroy();
}

