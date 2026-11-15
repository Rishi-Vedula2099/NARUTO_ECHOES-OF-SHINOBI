#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EOSHitStopComponent.generated.h"

/**
 * Authoritative Hit-Stop Component for NARUTO: ECHOES OF SHINOBI.
 * Implements actor-level hit-stop using CustomTimeDilation with concurrency safety.
 *
 * Rules:
 * 1. Global time dilation (UGameplayStatics::SetGlobalTimeDilation) is strictly prohibited.
 * 2. Hit-stop operates at the actor level using AActor::CustomTimeDilation.
 * 3. Deadlines are evaluated in unscaled world time (GetRealTimeSeconds).
 * 4. Overlapping hits extend the active deadline without overwriting PriorCustomTimeDilation.
 * 5. Restoring captured dilation happens exactly once upon final deadline expiration.
 * 6. Actor destruction and EndPlay cleanly invalidate timers.
 */
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class ECHOESOFSHINOBI_API UEOSHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEOSHitStopComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Apply hit-stop to the owning actor.
	 * @param DurationSeconds Duration in unscaled seconds.
	 * @param HitStopDilation Dilation factor during hit-stop (default 0.001f).
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Combat|HitStop")
	void ApplyHitStop(float DurationSeconds, float HitStopDilation = 0.001f);

	/**
	 * Cancel any active hit-stop immediately and restore prior dilation.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Combat|HitStop")
	void CancelHitStop();

	/**
	 * Check if hit-stop is currently active.
	 */
	UFUNCTION(BlueprintPure, Category = "EOS|Combat|HitStop")
	bool IsHitStopActive() const { return bIsHitStopActive; }

	/**
	 * Get the prior custom time dilation that will be restored.
	 */
	UFUNCTION(BlueprintPure, Category = "EOS|Combat|HitStop")
	float GetPriorCustomTimeDilation() const { return PriorCustomTimeDilation; }

	/**
	 * Get the expiration timestamp in unscaled world time.
	 */
	UFUNCTION(BlueprintPure, Category = "EOS|Combat|HitStop")
	double GetHitStopExpirationTime() const { return HitStopExpirationUnscaledTime; }

	/**
	 * Static helper to apply synchronized hit-stop to both attacker and target.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Combat|HitStop")
	static void ApplyHitStopToPair(AActor* Attacker, AActor* Target, float DurationSeconds, float HitStopDilation = 0.001f);

	/**
	 * Static helper to get or create a hit-stop component on an actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Combat|HitStop")
	static UEOSHitStopComponent* GetOrCreateHitStopComponent(AActor* TargetActor);

	/** Timer callback when hit-stop duration expires (public for unit test validation). */
	UFUNCTION()
	void OnHitStopTimerExpired();

private:
	UPROPERTY(VisibleAnywhere, Category = "EOS|Combat|HitStop")
	bool bIsHitStopActive;

	UPROPERTY(VisibleAnywhere, Category = "EOS|Combat|HitStop")
	float PriorCustomTimeDilation;

	UPROPERTY(VisibleAnywhere, Category = "EOS|Combat|HitStop")
	double HitStopExpirationUnscaledTime;

	FTimerHandle HitStopTimerHandle;
};

