#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSKawarimiLog.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

/**
 * Decoy wooden training log spawned during Kawarimi (Substitution Jutsu).
 * Disrupts enemy targeting, absorbs incoming attacks, and cleanly despawns after a set lifetime.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSKawarimiLog : public AActor
{
	GENERATED_BODY()

public:
	AEOSKawarimiLog();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS|Visuals")
	TObjectPtr<UNiagaraSystem> SmokeBurstFX;

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};

