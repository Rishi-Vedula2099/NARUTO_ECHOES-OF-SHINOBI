#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EOSAnimInstance.generated.h"

class AEOSCharacterBase;
class UCharacterMovementComponent;

UENUM(BlueprintType)
enum class EEOSLocomotionState : uint8
{
	Idle,
	Walk,
	Run,
	Sprint
};

UENUM(BlueprintType)
enum class EEOSAerialState : uint8
{
	Grounded,
	JumpStart,
	Apex,
	Falling,
	Landing
};

/**
 * Authoritative Shinobi Animation Instance
 * Provides ground locomotion (Idle/Walk/Ninja Run/Sprint), aerial states (Jump/Apex/Fall/Land),
 * and combat state flags driving the Animation Blueprint state machine.
 */
UCLASS(Blueprintable, BlueprintType)
class ECHOESOFSHINOBI_API UEOSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEOSAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Locomotion & Speed Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	float GroundSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	float MovementDirection = 0.0f; // -180 to +180 degrees

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	EEOSLocomotionState LocomotionState = EEOSLocomotionState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Locomotion")
	EEOSAerialState AerialState = EEOSAerialState::Grounded;

	// Combat & Action Flags
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	bool bIsInCombat = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	bool bIsGuarding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Combat")
	bool bIsDodging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Aiming")
	float AimPitch = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Aiming")
	float AimYaw = 0.0f;

	// Cached References
	UPROPERTY(BlueprintReadOnly, Category = "EOS|References")
	TObjectPtr<AEOSCharacterBase> Character;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
