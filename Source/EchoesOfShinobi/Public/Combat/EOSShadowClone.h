#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EOSShadowClone.generated.h"

class UNiagaraSystem;
class AEOSCharacterBase;
class UPointLightComponent;

/**
 * Tactical Shadow Clone (Kage Bunshin) combat companion.
 * Spawns in smoke, engages valid nearby enemies, draws aggro,
 * contributes 25% damage per hit (capped), and cleanly poofs after 6 seconds.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSShadowClone : public ACharacter
{
	GENERATED_BODY()

public:
	AEOSShadowClone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS|Visuals")
	TObjectPtr<UNiagaraSystem> SmokeBurstFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Visuals")
	UPointLightComponent* ChakraGlowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	TWeakObjectPtr<AEOSCharacterBase> SummonerShinobi;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Combat")
	int32 CloneIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Combat")
	float CloneHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Combat")
	float AttackDamage = 12.0f; // 25% of player base light melee (capped at 15.0)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Combat")
	float CloneLifetime = 6.0f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void InitializeClone(AEOSCharacterBase* InSummoner, const FVector& SpawnOffset);
	void SetAssignedTarget(AActor* InTarget);
	void ExecuteCloneAttack();
	void DisappearWithSmoke();

private:
	FTimerHandle TimerHandle_AttackLoop;
	FTimerHandle TimerHandle_Lifetime;
	bool bIsDisappearing = false;
};

