#include "../../Public/Combat/EOSHitStopComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../../EchoesOfShinobi.h"

UEOSHitStopComponent::UEOSHitStopComponent()
	: bIsHitStopActive(false)
	, PriorCustomTimeDilation(1.0f)
	, HitStopExpirationUnscaledTime(0.0)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEOSHitStopComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEOSHitStopComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CancelHitStop();
	Super::EndPlay(EndPlayReason);
}

void UEOSHitStopComponent::ApplyHitStop(float DurationSeconds, float HitStopDilation)
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (!Owner || !World || DurationSeconds <= 0.0f)
	{
		return;
	}

	const double CurrentTime = World->GetTimeSeconds();
	const double NewDeadline = CurrentTime + static_cast<double>(DurationSeconds);

	if (!bIsHitStopActive)
	{
		// Capture prior custom time dilation exactly once
		PriorCustomTimeDilation = Owner->CustomTimeDilation;
		bIsHitStopActive = true;
		HitStopExpirationUnscaledTime = NewDeadline;

		// Apply actor-level slowdown (never global time dilation)
		Owner->CustomTimeDilation = FMath::Clamp(HitStopDilation, 0.0f, 1.0f);

		UE_LOG(LogEOSCombat, Log, TEXT("[HIT_STOP] %s entered hit-stop for %.3fs (PriorDilation=%.3f, HitDilation=%.4f)"),
			*Owner->GetName(), DurationSeconds, PriorCustomTimeDilation, HitStopDilation);
	}
	else
	{
		// Overlapping hit: extend expiration deadline without mutating PriorCustomTimeDilation
		HitStopExpirationUnscaledTime = FMath::Max(HitStopExpirationUnscaledTime, NewDeadline);

		UE_LOG(LogEOSCombat, Log, TEXT("[HIT_STOP] %s hit-stop extended by %.3fs to deadline %.3f"),
			*Owner->GetName(), DurationSeconds, HitStopExpirationUnscaledTime);
	}

	// Calculate remaining time until the active deadline
	const float RemainingSeconds = FMath::Max(0.001f, static_cast<float>(HitStopExpirationUnscaledTime - CurrentTime));

	// Reset timer handle to expire at the new deadline
	World->GetTimerManager().ClearTimer(HitStopTimerHandle);
	World->GetTimerManager().SetTimer(HitStopTimerHandle, this, &UEOSHitStopComponent::OnHitStopTimerExpired, RemainingSeconds, false);
}

void UEOSHitStopComponent::CancelHitStop()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HitStopTimerHandle);
	}
	HitStopTimerHandle.Invalidate();

	if (bIsHitStopActive)
	{
		if (AActor* Owner = GetOwner())
		{
			Owner->CustomTimeDilation = PriorCustomTimeDilation;
			UE_LOG(LogEOSCombat, Verbose, TEXT("[HIT_STOP] %s hit-stop cancelled, restored CustomTimeDilation=%.2f"),
				*Owner->GetName(), PriorCustomTimeDilation);
		}

		bIsHitStopActive = false;
		PriorCustomTimeDilation = 1.0f;
		HitStopExpirationUnscaledTime = 0.0;
	}
}

void UEOSHitStopComponent::OnHitStopTimerExpired()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Paused-world guard: If the world is paused, hit-stop remains frozen
	if (World->IsPaused())
	{
		UE_LOG(LogEOSCombat, Verbose, TEXT("[HIT_STOP] World is paused, deferring hit-stop expiration"));
		return;
	}

	const double CurrentTime = World->GetTimeSeconds();

	// Guard against premature timer firing if deadline was extended
	if (CurrentTime < HitStopExpirationUnscaledTime - 0.002)
	{
		const float RemainingSeconds = static_cast<float>(HitStopExpirationUnscaledTime - CurrentTime);
		World->GetTimerManager().SetTimer(HitStopTimerHandle, this, &UEOSHitStopComponent::OnHitStopTimerExpired, RemainingSeconds, false);
		return;
	}

	// Restore captured dilation exactly once
	if (AActor* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = PriorCustomTimeDilation;
		UE_LOG(LogEOSCombat, Log, TEXT("[HIT_STOP] %s hit-stop completed, restored CustomTimeDilation=%.3f"),
			*Owner->GetName(), PriorCustomTimeDilation);
	}

	bIsHitStopActive = false;
	PriorCustomTimeDilation = 1.0f;
	HitStopExpirationUnscaledTime = 0.0;
	HitStopTimerHandle.Invalidate();
}

void UEOSHitStopComponent::ApplyHitStopToPair(AActor* Attacker, AActor* Target, float DurationSeconds, float HitStopDilation)
{
	if (Attacker)
	{
		UEOSHitStopComponent* AttackerComp = GetOrCreateHitStopComponent(Attacker);
		if (AttackerComp)
		{
			AttackerComp->ApplyHitStop(DurationSeconds, HitStopDilation);
		}
	}

	if (Target)
	{
		UEOSHitStopComponent* TargetComp = GetOrCreateHitStopComponent(Target);
		if (TargetComp)
		{
			TargetComp->ApplyHitStop(DurationSeconds, HitStopDilation);
		}
	}
}

UEOSHitStopComponent* UEOSHitStopComponent::GetOrCreateHitStopComponent(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return nullptr;
	}

	UEOSHitStopComponent* Comp = TargetActor->FindComponentByClass<UEOSHitStopComponent>();
	if (!Comp)
	{
		Comp = NewObject<UEOSHitStopComponent>(TargetActor, UEOSHitStopComponent::StaticClass(), TEXT("EOSHitStopComponent"));
		if (Comp)
		{
			Comp->RegisterComponent();
		}
	}
	return Comp;
}

