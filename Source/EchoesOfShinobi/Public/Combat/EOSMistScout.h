#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EOSMistScout.generated.h"

class UNiagaraSystem;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EEOSMistScoutRole : uint8
{
	MeleeScout UMETA(DisplayName = "Melee Dual Kunai Scout"),
	RangedScout UMETA(DisplayName = "Ranged Water Jutsu Scout")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSMistScoutDefeatedSignature, AEOSMistScout*, DefeatedScout);

/**
 * Humanoid Mist Scout Shinobi minion enemy for the Outskirts Gauntlet.
 * Distinguishes Melee Dual Kunai vs Ranged Water Jutsu roles with telegraphs and hit reactions.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSMistScout : public ACharacter
{
	GENERATED_BODY()

public:
	AEOSMistScout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Scout")
	EEOSMistScoutRole ScoutRole = EEOSMistScoutRole::MeleeScout;

	UFUNCTION(BlueprintPure, Category = "EOS|Scout")
	EEOSMistScoutRole GetScoutRole() const { return ScoutRole; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Scout")
	float MaxHealth = 350.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Scout")
	float CurrentHealth = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Scout")
	float BaseAttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Scout")
	float AttackTelegraphDuration = 0.6f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Scout")
	bool bIsTelegraphing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Scout")
	bool bIsDefeated = false;

	UPROPERTY(BlueprintAssignable, Category = "EOS|Scout")
	FEOSMistScoutDefeatedSignature OnScoutDefeated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS|Visuals")
	TObjectPtr<UNiagaraSystem> DefeatSmokeFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMeshComponent* ScoutStaticMesh;

	UFUNCTION(BlueprintCallable, Category = "EOS|Scout")
	void SetScoutRole(EEOSMistScoutRole NewRole);

	UFUNCTION(BlueprintPure, Category = "EOS|Scout")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void StartAttackTelegraph();
	void ExecuteAttack();

private:
	TWeakObjectPtr<AActor> CombatTarget;
	FTimerHandle TimerHandle_AttackTelegraph;
	FTimerHandle TimerHandle_AttackCooldown;
	float AttackCooldown = 2.2f;
	bool bCanAttack = true;
	void ResetAttackCooldown() { bCanAttack = true; }
};

