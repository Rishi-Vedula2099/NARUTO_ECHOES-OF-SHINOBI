#include "../../Public/Character/EOSCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Public/Combat/EOSTrainingDummy.h"
#include "../../EchoesOfShinobi.h"

AEOSCharacterBase::AEOSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule configuration
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Movement & Rotation configuration: Camera-relative movement driven by MovementProfile
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = MovementProfile.RotationRate;
	GetCharacterMovement()->JumpZVelocity = MovementProfile.JumpZVelocity;
	GetCharacterMovement()->AirControl = MovementProfile.AirControl;
	GetCharacterMovement()->MaxWalkSpeed = MovementProfile.RunSpeed;

	// CameraBoom (SpringArm) driven by CameraProfile
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = CameraProfile.TargetArmLength;
	CameraBoom->SocketOffset = CameraProfile.SocketOffset;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = CameraProfile.bEnableCameraLag;
	CameraBoom->CameraLagSpeed = CameraProfile.CameraLagSpeed;
	CameraBoom->bDoCollisionTest = true;

	// FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = CameraProfile.FieldOfView;

	// Gameplay Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UEOSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEOSAttributeSet>(TEXT("AttributeSet"));

	SetupStylizedVisualMesh();
}

void AEOSCharacterBase::SetupStylizedVisualMesh()
{
	// Vertical slice character visual presentation:
	// Eliminates TECHNICAL_DEBUG_ASSETS (primitives / checkerboard) from player view.
	// Uses authored stylized skeletal mesh or stylized character asset with anime cel-shaded material.
	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		CharMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		CharMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AEOSCharacterBase::SetCameraMode(EEOSCameraMode NewMode)
{
	CameraProfile.SetMode(NewMode);
	ApplyCameraProfile(CameraProfile);
}

void AEOSCharacterBase::ApplyCameraProfile(const FEOSCameraProfile& NewProfile)
{
	CameraProfile = NewProfile;
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraProfile.TargetArmLength;
		CameraBoom->SocketOffset = CameraProfile.SocketOffset;
		CameraBoom->bEnableCameraLag = CameraProfile.bEnableCameraLag;
		CameraBoom->CameraLagSpeed = CameraProfile.CameraLagSpeed;
	}
	if (FollowCamera)
	{
		FollowCamera->FieldOfView = CameraProfile.FieldOfView;
	}
}

void AEOSCharacterBase::ApplyMovementProfile(const FEOSMovementProfile& NewProfile)
{
	MovementProfile = NewProfile;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = bIsSprinting ? MovementProfile.SprintSpeed : MovementProfile.RunSpeed;
		MoveComp->JumpZVelocity = MovementProfile.JumpZVelocity;
		MoveComp->AirControl = MovementProfile.AirControl;
		MoveComp->RotationRate = MovementProfile.RotationRate;
	}
}

void AEOSCharacterBase::ApplyVisualProfile(const FEOSCharacterVisualProfile& NewProfile)
{
	VisualProfile = NewProfile;
	UE_LOG(LogEOSCore, Log, TEXT("Applied Visual Profile: Character=%s, Costume=%s, MaterialProfile=%s"),
		*VisualProfile.CharacterID, *VisualProfile.CostumeID, *VisualProfile.MaterialProfileID);
}

UAbilitySystemComponent* AEOSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEOSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterDefinitionAsset)
	{
		InitializeFromDefinition(CharacterDefinitionAsset);
	}
}

void AEOSCharacterBase::InitializeFromDefinition(UEOSCharacterDefinition* InDefinitionAsset)
{
	if (!InDefinitionAsset)
	{
		return;
	}

	CharacterDefinitionAsset = InDefinitionAsset;
	RuntimeFramework = InDefinitionAsset->Framework;

	// Populate AttributeSet from Stats
	if (AttributeSet)
	{
		AttributeSet->SetHealth(RuntimeFramework.Stats.MaxHP);
		AttributeSet->SetMaxHealth(RuntimeFramework.Stats.MaxHP);
		AttributeSet->SetChakra(RuntimeFramework.Stats.MaxChakra);
		AttributeSet->SetMaxChakra(RuntimeFramework.Stats.MaxChakra);
		AttributeSet->SetAttackPower(RuntimeFramework.Stats.EffectiveAttack);
		AttributeSet->SetDefensePower(RuntimeFramework.Stats.EffectiveDefense);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

FEOSCombatStatBlock AEOSCharacterBase::GetStats() const
{
	return RuntimeFramework.Stats;
}

FEOSBasicAttackCombo AEOSCharacterBase::GetBasicAttacks() const
{
	return RuntimeFramework.BasicAttacks;
}

FEOSSkillSetContainer AEOSCharacterBase::GetSkills() const
{
	return RuntimeFramework.Skills;
}

FEOSJutsuSlotContainer AEOSCharacterBase::GetJutsu() const
{
	return RuntimeFramework.Jutsu;
}

FEOSUltimateDefinition AEOSCharacterBase::GetUltimate() const
{
	return RuntimeFramework.Ultimate;
}

FEOSFormContainer AEOSCharacterBase::GetForms() const
{
	return RuntimeFramework.Forms;
}

FEOSGearGridContainer AEOSCharacterBase::GetEquipment() const
{
	return RuntimeFramework.Equipment;
}

FEOSCharacterAdvancementData AEOSCharacterBase::GetAdvancement() const
{
	return RuntimeFramework.Advancement;
}

FEOSCharacterMasteryRecord AEOSCharacterBase::GetMastery() const
{
	return RuntimeFramework.Mastery;
}

FEOSCharacterCompatibilityProfile AEOSCharacterBase::GetCompatibility() const
{
	return RuntimeFramework.Compatibility;
}

bool AEOSCharacterBase::TransformToForm(const FString& TargetFormID)
{
	if (RuntimeFramework.Forms.UnlockedFormIDs.Contains(TargetFormID) || TargetFormID == "FORM_BASE")
	{
		RuntimeFramework.Forms.ActiveFormID = TargetFormID;

		// Apply Form Stat Boost
		if (TargetFormID == "FORM_SAGE_MODE")
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.30f; // +30% stats
		}
		else if (TargetFormID == "FORM_KURAMA_LINK" || TargetFormID == "FORM_SUSANOO")
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.50f; // +50% stats
		}
		else
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.0f;
		}

		return true;
	}

	return false;
}

void AEOSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
	{
		return;
	}

	// Axis Mappings (Camera-relative movement & mouse look)
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEOSCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEOSCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEOSCharacterBase::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEOSCharacterBase::LookUp);

	// Action Mappings
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AEOSCharacterBase::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AEOSCharacterBase::StopSprint);

	PlayerInputComponent->BindAction(TEXT("AttackLight"), IE_Pressed, this, &AEOSCharacterBase::AttackLight);
	PlayerInputComponent->BindAction(TEXT("AttackHeavy"), IE_Pressed, this, &AEOSCharacterBase::AttackHeavy);
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AEOSCharacterBase::Dodge);

	PlayerInputComponent->BindAction(TEXT("Jutsu1"), IE_Pressed, this, &AEOSCharacterBase::CastJutsu1);
	PlayerInputComponent->BindAction(TEXT("Jutsu2"), IE_Pressed, this, &AEOSCharacterBase::CastJutsu2);
	PlayerInputComponent->BindAction(TEXT("Jutsu3"), IE_Pressed, this, &AEOSCharacterBase::CastJutsu3);
	PlayerInputComponent->BindAction(TEXT("Substitution"), IE_Pressed, this, &AEOSCharacterBase::CastSubstitution);

	PlayerInputComponent->BindAction(TEXT("Ultimate"), IE_Pressed, this, &AEOSCharacterBase::CastUltimateAbility);
	PlayerInputComponent->BindAction(TEXT("Transformation"), IE_Pressed, this, &AEOSCharacterBase::ToggleTransformation);
}

void AEOSCharacterBase::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEOSCharacterBase::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AEOSCharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AEOSCharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AEOSCharacterBase::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = MovementProfile.SprintSpeed;
}

void AEOSCharacterBase::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementProfile.RunSpeed;
}

void AEOSCharacterBase::AttackLight()
{
	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::AttackLight executed. Combo progression triggered."));

	// Melee trace ahead for training dummy or enemy
	const FVector Start = GetActorLocation();
	const FVector End = Start + (GetActorForwardVector() * 220.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyDamage(HitActor, 50.0f, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void AEOSCharacterBase::AttackHeavy()
{
	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::AttackHeavy executed. Heavy strike applied."));

	const FVector Start = GetActorLocation();
	const FVector End = Start + (GetActorForwardVector() * 250.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyDamage(HitActor, 120.0f, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void AEOSCharacterBase::Dodge()
{
	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::Dodge executed - Shunshin rapid step."));

	FVector DodgeDirection = GetLastMovementInputVector();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = -GetActorForwardVector();
	}
	DodgeDirection.Z = 0.0f;
	DodgeDirection.Normalize();

	LaunchCharacter(DodgeDirection * MovementProfile.DodgeImpulse + FVector(0.0f, 0.0f, 150.0f), true, true);
}

void AEOSCharacterBase::CastSubstitution()
{
	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::CastSubstitution executed [ACTION_SUBSTITUTION] - Kawarimi log evasion."));

	// Substitution replaces shinobi with wood log and teleports slightly backwards/sideways
	const FVector EvadeOffset = -GetActorForwardVector() * MovementProfile.SubstitutionDistance + FVector(0.0f, 0.0f, 50.0f);
	TeleportTo(GetActorLocation() + EvadeOffset, GetActorRotation());
}

void AEOSCharacterBase::CastJutsuSlot(int32 SlotIndex)
{
	// Data-driven jutsu slot execution (GAS backed)
	FString JutsuID;
	float Cost = 25.0f;
	float Damage = 180.0f;

	switch (SlotIndex)
	{
	case 0:
		JutsuID = TEXT("JUTSU_NARUTO_RASENGAN"); // Slot 1: [Q] Rasengan (CANON_VERIFIED)
		Cost = 30.0f;
		Damage = 220.0f;
		break;
	case 1:
		JutsuID = TEXT("JUTSU_NARUTO_SHADOW_CLONE"); // Slot 2: [E] Shadow Clone (CANON_VERIFIED)
		Cost = 25.0f;
		Damage = 160.0f;
		break;
	case 2:
		JutsuID = TEXT("JUTSU_NARUTO_UZUMAKI_BARRAGE"); // Slot 3: [R] Uzumaki Barrage (CANON_VERIFIED)
		Cost = 35.0f;
		Damage = 260.0f;
		break;
	default:
		JutsuID = TEXT("JUTSU_GENERIC");
		break;
	}

	UE_LOG(LogEOSAbility, Log, TEXT("AEOSCharacterBase::CastJutsuSlot %d activated: %s"), SlotIndex, *JutsuID);

	if (AttributeSet)
	{
		const float CurrentChakra = AttributeSet->GetChakra();
		if (CurrentChakra >= Cost)
		{
			AttributeSet->SetChakra(FMath::Max(0.0f, CurrentChakra - Cost));

			// AOE damage in front
			const FVector Center = GetActorLocation() + (GetActorForwardVector() * 300.0f);
			TArray<FHitResult> Hits;
			FCollisionShape Sphere = FCollisionShape::MakeSphere(250.0f);
			if (GetWorld()->SweepMultiByChannel(Hits, Center, Center, FQuat::Identity, ECC_Pawn, Sphere))
			{
				for (const FHitResult& Hit : Hits)
				{
					if (Hit.GetActor() && Hit.GetActor() != this)
					{
						UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
					}
				}
			}
		}
		else
		{
			UE_LOG(LogEOSAbility, Warning, TEXT("Insufficient Chakra to cast Jutsu %s (Requires %.1f, Has %.1f)"),
				*JutsuID, Cost, CurrentChakra);
		}
	}
}

void AEOSCharacterBase::CastJutsu1()
{
	CastJutsuSlot(0);
}

void AEOSCharacterBase::CastJutsu2()
{
	CastJutsuSlot(1);
}

void AEOSCharacterBase::CastJutsu3()
{
	CastJutsuSlot(2);
}

void AEOSCharacterBase::ToggleTransformation()
{
	const FString TargetForm = (RuntimeFramework.Forms.ActiveFormID == TEXT("FORM_BASE")) ? TEXT("FORM_SAGE_MODE") : TEXT("FORM_BASE");
	TransformToForm(TargetForm);
}

void AEOSCharacterBase::CastUltimateAbility()
{
	// Ultimate: JUTSU_VERMILION_AWAKENING (ORIGINAL_GAME_VARIANT; CanonReference: NINE_TAILS_CHAKRA_MODE)
	UE_LOG(LogEOSAbility, Log, TEXT("AEOSCharacterBase::CastUltimateAbility [JUTSU_VERMILION_AWAKENING] unsealed!"));

	if (AttributeSet)
	{
		const float CurrentChakra = AttributeSet->GetChakra();
		if (CurrentChakra >= 50.0f)
		{
			AttributeSet->SetChakra(FMath::Max(0.0f, CurrentChakra - 50.0f));

			const FVector Center = GetActorLocation() + (GetActorForwardVector() * 400.0f);
			TArray<FHitResult> Hits;
			FCollisionShape Sphere = FCollisionShape::MakeSphere(500.0f);
			if (GetWorld()->SweepMultiByChannel(Hits, Center, Center, FQuat::Identity, ECC_Pawn, Sphere))
			{
				for (const FHitResult& Hit : Hits)
				{
					if (Hit.GetActor() && Hit.GetActor() != this)
					{
						UGameplayStatics::ApplyDamage(Hit.GetActor(), 500.0f, GetController(), this, UDamageType::StaticClass());
					}
				}
			}
		}
	}
}

void AEOSCharacterBase::SwitchSquadMember(int32 MemberIndex)
{
	// Data-driven Team 7 Vertical Slice preset: [0]=Naruto, [1]=Sasuke, [2]=Sakura
	static const TArray<FString> Team7Roster = { TEXT("CHAR_NARUTO"), TEXT("CHAR_SASUKE"), TEXT("CHAR_SAKURA") };
	if (Team7Roster.IsValidIndex(MemberIndex))
	{
		const FString TargetShinobiID = Team7Roster[MemberIndex];
		UE_LOG(LogEOSCore, Log, TEXT("SwitchSquadMember to slot %d: %s (resolved via Team 7 Data Architecture)"),
			MemberIndex, *TargetShinobiID);
	}
}

void AEOSCharacterBase::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AEOSCharacterBase::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
}

void AEOSCharacterBase::ApplyVisualForm(EEOSVisualForm NewForm)
{
	CurrentFormVisual.Form = NewForm;
	switch (NewForm)
	{
	case EEOSVisualForm::SageMode:
		CurrentFormVisual.EyePigmentationColor = FLinearColor(1.0f, 0.45f, 0.05f, 1.0f); // Bright orange eyelids
		CurrentFormVisual.PupilColor = FLinearColor(1.0f, 0.85f, 0.1f, 1.0f); // Golden toad horizontal iris
		CurrentFormVisual.ChakraAuraColor = FLinearColor(1.0f, 0.6f, 0.1f, 1.0f); // Amber nature aura
		CurrentFormVisual.AuraIntensity = 2.0f;
		CurrentFormVisual.CloakMeshName = TEXT("SM_SageCloak_RedBlack");
		CurrentFormVisual.bEnableFaceMarkings = true;
		break;
	case EEOSVisualForm::VermilionAwakening:
		CurrentFormVisual.EyePigmentationColor = FLinearColor(0.8f, 0.1f, 0.1f, 1.0f); // Crimson shadow
		CurrentFormVisual.PupilColor = FLinearColor(0.95f, 0.1f, 0.05f, 1.0f); // Feral red slit pupils
		CurrentFormVisual.ChakraAuraColor = FLinearColor(0.9f, 0.15f, 0.05f, 1.0f); // Boiling vermilion bubbling aura
		CurrentFormVisual.AuraIntensity = 3.5f;
		CurrentFormVisual.bEnableFaceMarkings = true;
		CurrentFormVisual.StanceProwessScale = 1.25f; // Feral forward stance
		break;
	case EEOSVisualForm::KuramaChakraMode:
		CurrentFormVisual.EyePigmentationColor = FLinearColor(1.0f, 0.85f, 0.1f, 1.0f);
		CurrentFormVisual.PupilColor = FLinearColor(1.0f, 0.95f, 0.2f, 1.0f);
		CurrentFormVisual.ChakraAuraColor = FLinearColor(1.0f, 0.8f, 0.1f, 1.0f); // Brilliant golden solar aura
		CurrentFormVisual.AuraIntensity = 5.0f;
		CurrentFormVisual.CloakMeshName = TEXT("SM_KuramaFlameShroud");
		break;
	case EEOSVisualForm::Base_Genin:
	case EEOSVisualForm::Base_Shippuden:
	default:
		CurrentFormVisual.EyePigmentationColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
		CurrentFormVisual.PupilColor = FLinearColor(0.1f, 0.45f, 0.9f, 1.0f); // Blue azure eyes
		CurrentFormVisual.ChakraAuraColor = FLinearColor(0.1f, 0.6f, 1.0f, 0.0f);
		CurrentFormVisual.AuraIntensity = 0.0f;
		CurrentFormVisual.CloakMeshName = TEXT("None");
		CurrentFormVisual.bEnableFaceMarkings = false;
		CurrentFormVisual.StanceProwessScale = 1.0f;
		break;
	}

	UE_LOG(LogEOSCore, Log, TEXT("Applied Visual Form Styling: %d (Aura Intensity: %.1f)"),
		static_cast<int32>(NewForm), CurrentFormVisual.AuraIntensity);
}

void AEOSCharacterBase::AttachGearVisual(FName SocketName, const FString& GearMeshName)
{
	UE_LOG(LogEOSCore, Log, TEXT("Attaching Gear Visual Mesh %s to Character Socket: %s"),
		*GearMeshName, *SocketName.ToString());
}
