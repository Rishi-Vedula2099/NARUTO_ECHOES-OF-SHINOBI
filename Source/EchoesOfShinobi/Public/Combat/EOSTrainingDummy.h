#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOSTrainingDummy.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

/**
 * Interactive Training Dummy / Post for Konoha Training Grounds combat slice.
 * Responds to player attacks, tracks damage, and provides visual wobble feedback.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSTrainingDummy : public AActor
{
	GENERATED_BODY()

public:
	AEOSTrainingDummy();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Components")
	TObjectPtr<UStaticMeshComponent> PostMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Components")
	TObjectPtr<UStaticMeshComponent> TargetBoardMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Stats")
	float MaxHealth = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Stats")
	float CurrentHealth = 1000.0f;

	UFUNCTION(BlueprintPure, Category = "EOS|Stats")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f) : 0.0f; }

protected:
	virtual void BeginPlay() override;

	float WobbleTimer = 0.0f;
	FRotator BaseRotation;
};
