#include "Animation/EOSAnimInstance.h"
#include "Character/EOSCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEOSAnimInstance::UEOSAnimInstance()
{
}

void UEOSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn)
	{
		Character = Cast<AEOSCharacterBase>(OwnerPawn);
		if (Character)
		{
			MovementComponent = Character->GetCharacterMovement();
		}
	}
}

void UEOSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character)
	{
		APawn* OwnerPawn = TryGetPawnOwner();
		if (OwnerPawn)
		{
			Character = Cast<AEOSCharacterBase>(OwnerPawn);
		}
	}

	if (!Character)
	{
		return;
	}

	if (!MovementComponent)
	{
		MovementComponent = Character->GetCharacterMovement();
	}

	const FVector Velocity = Character->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	bIsFalling = MovementComponent ? MovementComponent->IsFalling() : false;
	bIsSprinting = Character->bIsSprinting;

	const bool bHasAcceleration = MovementComponent && (MovementComponent->GetCurrentAcceleration().SizeSquared() > 0.0f);
	bShouldMove = (GroundSpeed > 5.0f) && bHasAcceleration;

	// Calculate movement direction relative to actor facing (-180 to +180)
	if (GroundSpeed > 5.0f)
	{
		const FRotator ActorRotation = Character->GetActorRotation();
		const FRotator VelocityRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
		const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRotation, ActorRotation);
		MovementDirection = DeltaRot.Yaw;
	}
	else
	{
		MovementDirection = 0.0f;
	}

	// Update LocomotionState based on GroundSpeed and Sprinting
	const float WalkThreshold = Character->MovementProfile.WalkSpeed + 50.0f;
	const float RunThreshold = Character->MovementProfile.RunSpeed + 50.0f;

	if (GroundSpeed <= 10.0f)
	{
		LocomotionState = EEOSLocomotionState::Idle;
	}
	else if (GroundSpeed <= WalkThreshold && !bIsSprinting)
	{
		LocomotionState = EEOSLocomotionState::Walk;
	}
	else if (bIsSprinting || GroundSpeed > RunThreshold)
	{
		LocomotionState = EEOSLocomotionState::Sprint;
	}
	else
	{
		LocomotionState = EEOSLocomotionState::Run;
	}

	// Update AerialState
	if (bIsFalling)
	{
		if (Velocity.Z > 100.0f)
		{
			AerialState = EEOSAerialState::JumpStart;
		}
		else if (FMath::Abs(Velocity.Z) <= 100.0f)
		{
			AerialState = EEOSAerialState::Apex;
		}
		else
		{
			AerialState = EEOSAerialState::Falling;
		}
	}
	else
	{
		if (AerialState == EEOSAerialState::Falling)
		{
			AerialState = EEOSAerialState::Landing;
		}
		else
		{
			AerialState = EEOSAerialState::Grounded;
		}
	}

	// Calculate Aim Pitch & Yaw
	const FRotator AimRotation = Character->GetBaseAimRotation();
	const FRotator ActorRotation = Character->GetActorRotation();
	const FRotator DeltaAim = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	AimPitch = DeltaAim.Pitch;
	AimYaw = DeltaAim.Yaw;
}
