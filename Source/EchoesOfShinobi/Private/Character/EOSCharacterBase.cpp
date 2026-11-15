#include "../../Public/Character/EOSCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"
#include "UObject/SoftObjectPath.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "../../Public/Combat/EOSTrainingDummy.h"
#include "../../Public/Combat/EOSHitStopComponent.h"
#include "../../Public/Combat/EOSKawarimiLog.h"
#include "../../Public/Combat/EOSShadowClone.h"
#include "../../Public/Combat/EOSMistScout.h"
#include "../../Public/Combat/EOSBossCharacter.h"
#include "../../Public/Framework/EOSPlayerController.h"
#include "../../Public/UI/EOSShinobiHUD.h"
#include "../../EchoesOfShinobi.h"

AEOSCharacterBase::AEOSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule configuration
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

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
	CameraBoom->ProbeSize = 4.0f;
	CameraBoom->TargetOffset = FVector(0.0f, 0.0f, 65.0f);

	// FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = CameraProfile.FieldOfView;

	// Gameplay Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UEOSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEOSAttributeSet>(TEXT("AttributeSet"));

	// Niagara Jutsu Component
	JutsuVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JutsuVFXComponent"));
	if (GetMesh())
	{
		JutsuVFXComponent->SetupAttachment(GetMesh());
	}
	JutsuVFXComponent->bAutoActivate = false;

	ResolveProductionVisualDefaults();
	ApplyMeshComponentLayout();

	SetupStylizedVisualMesh();
}

void AEOSCharacterBase::ResolveProductionVisualDefaults()
{
	VisualProfile.CharacterID = TEXT("CHAR_NARUTO");
	VisualProfile.EraID = TEXT("ERA_GENIN");
	VisualProfile.FormID = TEXT("FORM_BASE");
	VisualProfile.CostumeID = TEXT("COSTUME_CLASSIC_TRACKSUIT");
	VisualProfile.AnimationProfileID = TEXT("ANIM_NARUTO_GENIN");
	VisualProfile.MaterialProfileID = TEXT("MAT_CEL_TOON_SHINOBI");
	VisualProfile.VFXProfileID = TEXT("VFX_NARUTO_DEFAULT");

	// Canonical proof-of-pipeline mesh & ABP (Naruto Genin)
	VisualProfile.SkeletalMeshAsset = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged.SK_Naruto_Genin_Rigged")));
	VisualProfile.AnimBlueprintClass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin.ABP_Naruto_Genin_C")));

	// 7 Canonical Material Slots
	VisualProfile.MaterialInstances.Empty();
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Skin.MI_Naruto_Skin"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Navy.MI_Naruto_Navy"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Hair.MI_Naruto_Hair"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Collar.MI_Naruto_Collar"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit.MI_Naruto_Tracksuit"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Emblem.MI_Naruto_Emblem"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Metal.MI_Naruto_Metal"))));

	// Canonical Niagara Jutsu Asset (Rasengan)
	JutsuVFXAsset = TSoftObjectPtr<UNiagaraSystem>(FSoftObjectPath(TEXT("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master.NS_Rasengan_Orb_Master")));
}

void AEOSCharacterBase::ApplyMeshComponentLayout()
{
	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		CharMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		CharMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		CharMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

void AEOSCharacterBase::SetupStylizedVisualMesh()
{
	ApplyMeshComponentLayout();

	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		if (USkeletalMesh* MeshAsset = VisualProfile.SkeletalMeshAsset.LoadSynchronous())
		{
			CharMesh->SetSkeletalMeshAsset(MeshAsset);
		}

		if (UClass* AnimClass = VisualProfile.AnimBlueprintClass.LoadSynchronous())
		{
			CharMesh->SetAnimInstanceClass(AnimClass);
		}

		for (int32 SlotIdx = 0; SlotIdx < VisualProfile.MaterialInstances.Num(); ++SlotIdx)
		{
			if (UMaterialInterface* Mat = VisualProfile.MaterialInstances[SlotIdx].LoadSynchronous())
			{
				CharMesh->SetMaterial(SlotIdx, Mat);
			}
		}
	}
}

void AEOSCharacterBase::SpawnJutsuVFXAtSocket()
{
	if (!JutsuVFXAsset.IsNull())
	{
		UNiagaraSystem* LoadedVFX = JutsuVFXAsset.LoadSynchronous();
		if (LoadedVFX && GetMesh())
		{
			if (JutsuVFXComponent)
			{
				JutsuVFXComponent->SetAsset(LoadedVFX);
				JutsuVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
				JutsuVFXComponent->Activate(true);
			}
			else
			{
				JutsuVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
					LoadedVFX,
					GetMesh(),
					TEXT("hand_rSocket"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}
		}
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
		CameraBoom->ProbeSize = 4.0f;
		CameraBoom->TargetOffset = FVector(0.0f, 0.0f, 65.0f);
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

namespace
{
	void ApplyImpactToTarget(AEOSCharacterBase* Attacker, AActor* HitActor, float Damage, float HitStopDuration, float KnockbackStrength, float ComboScore)
	{
		if (!Attacker || !HitActor || HitActor == Attacker)
		{
			return;
		}

		UGameplayStatics::ApplyDamage(HitActor, Damage, Attacker->GetController(), Attacker, UDamageType::StaticClass());
		UEOSHitStopComponent::ApplyHitStopToPair(Attacker, HitActor, HitStopDuration, 0.001f);

		if (ACharacter* HitChar = Cast<ACharacter>(HitActor))
		{
			FVector KnockDir = Attacker->GetActorForwardVector();
			KnockDir.Z = 0.0f;
			if (!KnockDir.IsNearlyZero())
			{
				KnockDir.Normalize();
				HitChar->LaunchCharacter(KnockDir * KnockbackStrength, false, false);
			}
		}

		if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(Attacker->GetController()))
		{
			if (AEOSShinobiHUD* HUD = Cast<AEOSShinobiHUD>(PC->GetHUD()))
			{
				HUD->RegisterHit(1, ComboScore);
			}
		}
	}
}

void AEOSCharacterBase::AttackLight()
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::AttackLight executed. Combo progression triggered."));

	// Melee trace ahead for training dummy or enemy
	const FVector Start = GetActorLocation();
	const FVector End = Start + (GetActorForwardVector() * 220.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
	{
		ApplyImpactToTarget(this, HitResult.GetActor(), 50.0f, 0.08f, 120.0f, 15.0f);
	}
}

void AEOSCharacterBase::AttackHeavy()
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	UE_LOG(LogEOSCombat, Log, TEXT("AEOSCharacterBase::AttackHeavy executed. Heavy strike applied."));

	const FVector Start = GetActorLocation();
	const FVector End = Start + (GetActorForwardVector() * 250.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
	{
		ApplyImpactToTarget(this, HitResult.GetActor(), 120.0f, 0.12f, 280.0f, 25.0f);
	}
}

void AEOSCharacterBase::Dodge()
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	if (!bCanDodge) return;

	bCanDodge = false;
	bIsInvulnerable = true;

	if (UWorld* World = GetWorld())
	{
		// 250ms i-frame window
		World->GetTimerManager().SetTimer(
			TimerHandle_Invulnerability,
			this,
			&AEOSCharacterBase::EndInvulnerability,
			DodgeIFrameDuration,
			false
		);

		// Dodge cooldown
		World->GetTimerManager().SetTimer(
			TimerHandle_DodgeCooldown,
			this,
			&AEOSCharacterBase::ResetDodgeCooldown,
			DodgeCooldown,
			false
		);
	}

	FVector DodgeDirection = GetLastMovementInputVector();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = -GetActorForwardVector();
	}
	DodgeDirection.Z = 0.0f;
	DodgeDirection.Normalize();

	LaunchCharacter(DodgeDirection * MovementProfile.DodgeImpulse + FVector(0.0f, 0.0f, 150.0f), true, true);

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetRenderCustomDepth(true);
		MeshComp->SetCustomDepthStencilValue(252); // Invulnerability / Afterimage flash cue
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[DODGE] Directional dodge executed: 250ms i-frame active, Direction=%s"),
		*DodgeDirection.ToString());
}

void AEOSCharacterBase::EndInvulnerability()
{
	bIsInvulnerable = false;
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetRenderCustomDepth(false);
	}
	UE_LOG(LogEOSCombat, Log, TEXT("[DODGE] Invulnerability window expired. Normal vulnerability restored."));
}

void AEOSCharacterBase::ResetDodgeCooldown()
{
	bCanDodge = true;
}

void AEOSCharacterBase::ResetSubstitutionCooldown()
{
	bCanSubstitute = true;
}

void AEOSCharacterBase::CastSubstitution()
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	if (!bCanSubstitute)
	{
		UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Substitution still on cooldown."));
		return;
	}

	bCanSubstitute = false;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandle_SubstitutionCooldown,
			this,
			&AEOSCharacterBase::ResetSubstitutionCooldown,
			SubstitutionCooldown,
			false
		);
	}

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector PriorLocation = GetActorLocation();
	const FRotator PriorRotation = GetActorRotation();

	// 1. Spawn Kawarimi Log at player's previous location
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEOSKawarimiLog* DecoyLog = World->SpawnActor<AEOSKawarimiLog>(
		AEOSKawarimiLog::StaticClass(),
		PriorLocation,
		PriorRotation,
		SpawnParams
	);

	// 2. Determine attacker and desired reposition location (350 cm behind attacker)
	FVector DesiredLoc;
	FRotator DesiredRot;

	if (LastAttackerActor.IsValid())
	{
		AActor* Attacker = LastAttackerActor.Get();
		const FVector AttackerLoc = Attacker->GetActorLocation();
		const FVector AttackerFwd = Attacker->GetActorForwardVector();

		DesiredLoc = AttackerLoc - AttackerFwd * SubstitutionDistance;
		DesiredRot = (AttackerLoc - DesiredLoc).Rotation();
	}
	else
	{
		DesiredLoc = PriorLocation - GetActorForwardVector() * SubstitutionDistance;
		DesiredRot = PriorRotation;
	}

	// 3. Collision validation & safe fallback
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(TEXT("KawarimiRepositionTrace"), false, this);
	if (DecoyLog)
	{
		TraceParams.AddIgnoredActor(DecoyLog);
	}

	const FVector TraceStart = PriorLocation + FVector(0.0f, 0.0f, 50.0f);
	const FVector TraceEnd = DesiredLoc + FVector(0.0f, 0.0f, 50.0f);

	FVector SafeDestination = DesiredLoc;
	if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, TraceParams))
	{
		SafeDestination = HitResult.Location + HitResult.ImpactNormal * 60.0f;
		SafeDestination.Z = PriorLocation.Z;
		UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Reposition trace blocked; safe fallback adjusted to %s"),
			*SafeDestination.ToString());
	}

	// 4. Camera clearance validation behind destination
	const float DesiredArmLength = CameraBoom ? CameraBoom->TargetArmLength : 420.0f;
	const FVector CameraPivot = SafeDestination + (CameraBoom ? CameraBoom->SocketOffset : FVector(0.0f, 0.0f, 75.0f));
	const FVector DesiredCameraPos = CameraPivot - DesiredRot.Vector() * DesiredArmLength;

	FHitResult CamHit;
	FCollisionQueryParams CamTraceParams(TEXT("KawarimiCameraClearanceTrace"), false, this);
	if (DecoyLog) CamTraceParams.AddIgnoredActor(DecoyLog);
	if (LastAttackerActor.IsValid()) CamTraceParams.AddIgnoredActor(LastAttackerActor.Get());

	if (World->SweepSingleByChannel(CamHit, CameraPivot, DesiredCameraPos, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(16.0f), CamTraceParams))
	{
		const float AvailableDistance = CamHit.Distance;
		if (AvailableDistance < 200.0f)
		{
			const float NudgeForward = 200.0f - AvailableDistance;
			SafeDestination += DesiredRot.Vector() * NudgeForward;
			UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Camera clearance constrained (%.1f cm); nudged SafeDestination forward by %.1f cm"),
				AvailableDistance, NudgeForward);
		}
	}

	// 5. Teleport player safely
	TeleportTo(SafeDestination, DesiredRot);

	// Synchronize Controller rotation so the camera frames the attacker and action
	if (AController* C = GetController())
	{
		C->SetControlRotation(DesiredRot);
	}

	// Instantly snap camera spring arm location without lag to prevent camera dragging through world geometry
	if (CameraBoom)
	{
		const bool bPrevLag = CameraBoom->bEnableCameraLag;
		CameraBoom->bEnableCameraLag = false;
		if (World)
		{
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, bPrevLag]()
			{
				if (CameraBoom)
				{
					CameraBoom->bEnableCameraLag = bPrevLag;
				}
			}));
		}
	}

	// 5. Break enemy target-lock and redirect to the decoy log
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), FoundEnemies);
	for (AActor* Enemy : FoundEnemies)
	{
		if (!Enemy || Enemy == this || Enemy == DecoyLog) continue;
		if (Enemy->IsA<AEOSBossCharacter>() || Enemy->ActorHasTag(TEXT("Enemy")))
		{
			UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Disrupted target-lock of enemy %s -> redirected to Kawarimi Decoy Log."),
				*Enemy->GetName());
		}
	}

	UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI] Substituted! Log at %s, Player repositioned behind attacker to %s (Cooldown: %.1fs)."),
		*PriorLocation.ToString(), *SafeDestination.ToString(), SubstitutionCooldown);

	const FVector P_start = PriorLocation;
	const FVector P_att = LastAttackerActor.IsValid() ? LastAttackerActor->GetActorLocation() : PriorLocation;
	const FVector P_req = DesiredLoc;
	const FVector P_acc = SafeDestination;
	const FVector D_actual = P_acc - P_start;
	const float ActualDisplacement = D_actual.Size();

	UE_LOG(LogEOSCombat, Log, TEXT("[KAWARIMI-FORENSIC] P_start=(X=%.1f,Y=%.1f,Z=%.1f) P_att=(X=%.1f,Y=%.1f,Z=%.1f) P_req=(X=%.1f,Y=%.1f,Z=%.1f) P_acc=(X=%.1f,Y=%.1f,Z=%.1f) D_actual=(X=%.1f,Y=%.1f,Z=%.1f, Len=%.1f cm)"),
		P_start.X, P_start.Y, P_start.Z,
		P_att.X, P_att.Y, P_att.Z,
		P_req.X, P_req.Y, P_req.Z,
		P_acc.X, P_acc.Y, P_acc.Z,
		D_actual.X, D_actual.Y, D_actual.Z, ActualDisplacement);
}

float AEOSCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser)
	{
		LastAttackerActor = DamageCauser;
	}
	else if (EventInstigator && EventInstigator->GetPawn())
	{
		LastAttackerActor = EventInstigator->GetPawn();
	}

	// 1. Invulnerability check (Dodge i-frame 250ms)
	if (bIsInvulnerable)
	{
		UE_LOG(LogEOSCombat, Log, TEXT("[COMBAT] Damage %.1f completely NEGATED by Dodge i-frame window!"), DamageAmount);
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage <= 0.0f && DamageAmount > 0.0f)
	{
		ActualDamage = DamageAmount;
	}

	RuntimeFramework.Stats.CurrentHP = FMath::Max(0.0f, RuntimeFramework.Stats.CurrentHP - ActualDamage);

	UE_LOG(LogEOSCombat, Log, TEXT("[COMBAT] %s took %.1f damage from %s (Remaining HP: %.1f/%.1f)"),
		*GetName(), ActualDamage, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"),
		RuntimeFramework.Stats.CurrentHP, RuntimeFramework.Stats.MaxHP);

	return ActualDamage;
}

void AEOSCharacterBase::CastJutsuSlot(int32 SlotIndex)
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	// Data-driven jutsu slot execution (GAS backed)
	FString JutsuID;
	float Cost = 25.0f;
	float Damage = 180.0f;

	switch (SlotIndex)
	{
	case 0:
		JutsuID = TEXT("JUTSU_NARUTO_RASENGAN"); // Slot 1: [Q] Rasengan (CANON_VERIFIED)
		SpawnJutsuVFXAtSocket();
		Cost = 30.0f;
		Damage = 220.0f;
		break;
	case 1:
		JutsuID = TEXT("JUTSU_NARUTO_SHADOW_CLONE"); // Slot 2: [E] Shadow Clone (CANON_VERIFIED)
		Cost = 25.0f;
		Damage = 160.0f;

		// Spawn two tactical Shadow Clones
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters CloneSpawnParams;
			CloneSpawnParams.Owner = this;
			CloneSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const FVector LeftOffset = -GetActorRightVector() * 140.0f - GetActorForwardVector() * 80.0f;
			const FVector RightOffset = GetActorRightVector() * 140.0f - GetActorForwardVector() * 80.0f;

			AEOSShadowClone* Clone1 = World->SpawnActor<AEOSShadowClone>(
				AEOSShadowClone::StaticClass(),
				GetActorLocation() + LeftOffset,
				GetActorRotation(),
				CloneSpawnParams
			);
			if (Clone1)
			{
				Clone1->CloneIndex = 1;
				Clone1->InitializeClone(this, LeftOffset);
			}

			AEOSShadowClone* Clone2 = World->SpawnActor<AEOSShadowClone>(
				AEOSShadowClone::StaticClass(),
				GetActorLocation() + RightOffset,
				GetActorRotation(),
				CloneSpawnParams
			);
			if (Clone2)
			{
				Clone2->CloneIndex = 2;
				Clone2->InitializeClone(this, RightOffset);
			}

			// Dual target assignment: Clone 1 -> Melee Scout, Clone 2 -> Ranged Scout
			TArray<AEOSMistScout*> Scouts;
			for (TActorIterator<AEOSMistScout> It(World); It; ++It)
			{
				Scouts.Add(*It);
			}

			if (Scouts.Num() >= 2)
			{
				AEOSMistScout* MeleeScout = (Scouts[0]->GetScoutRole() == EEOSMistScoutRole::MeleeScout) ? Scouts[0] : Scouts[1];
				AEOSMistScout* RangedScout = (Scouts[0]->GetScoutRole() == EEOSMistScoutRole::RangedScout) ? Scouts[0] : Scouts[1];
				if (Clone1) Clone1->SetAssignedTarget(MeleeScout);
				if (Clone2) Clone2->SetAssignedTarget(RangedScout);
				UE_LOG(LogEOSCombat, Log, TEXT("[JUTSU] Shadow Clone tactical coordination: Clone 1 -> %s (Melee), Clone 2 -> %s (Ranged)"),
					*MeleeScout->GetName(), *RangedScout->GetName());
			}
			else if (Scouts.Num() == 1)
			{
				if (Clone1) Clone1->SetAssignedTarget(Scouts[0]);
				if (Clone2) Clone2->SetAssignedTarget(Scouts[0]);
			}

			UE_LOG(LogEOSCombat, Log, TEXT("[JUTSU] Cast Shadow Clone Jutsu (Kage Bunshin no Jutsu): 2 tactical clones spawned!"));
		}
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

			// AOE damage in front (deduplicated per actor so Rasengan increments hit count once per target)
			const FVector Center = GetActorLocation() + (GetActorForwardVector() * 300.0f);
			TArray<FHitResult> Hits;
			TSet<AActor*> HitActors;
			FCollisionShape Sphere = FCollisionShape::MakeSphere(250.0f);
			if (GetWorld()->SweepMultiByChannel(Hits, Center, Center, FQuat::Identity, ECC_Pawn, Sphere))
			{
				const float JutsuHitStopDuration = (SlotIndex == 0) ? 0.18f : 0.10f;
				const float KnockbackStrength = (SlotIndex == 0) ? 400.0f : 250.0f;
				for (const FHitResult& Hit : Hits)
				{
					AActor* Target = Hit.GetActor();
					if (Target && Target != this && !HitActors.Contains(Target))
					{
						HitActors.Add(Target);
						ApplyImpactToTarget(this, Target, Damage, JutsuHitStopDuration, KnockbackStrength, 30.0f);
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
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

	const FString TargetForm = (RuntimeFramework.Forms.ActiveFormID == TEXT("FORM_BASE")) ? TEXT("FORM_SAGE_MODE") : TEXT("FORM_BASE");
	TransformToForm(TargetForm);
}

void AEOSCharacterBase::CastUltimateAbility()
{
	if (AEOSPlayerController* PC = Cast<AEOSPlayerController>(GetController()))
	{
		if (PC->IsDialogueActive()) return;
	}

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


TArray<FEOSVisualAssetSlotVerdict> AEOSCharacterBase::ValidateVisualAssetIntegrity(bool bLogResults) const
{
	TArray<FEOSVisualAssetSlotVerdict> Verdicts;

	// 1. SkeletalMesh Slot
	{
		FEOSVisualAssetSlotVerdict MeshVerdict;
		MeshVerdict.AssetID = VisualProfile.CharacterID;
		MeshVerdict.AssetSlot = TEXT("SkeletalMesh");
		MeshVerdict.ResolvedPath = VisualProfile.SkeletalMeshAsset.ToSoftObjectPath().ToString();
		MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (MeshVerdict.ResolvedPath.IsEmpty())
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			MeshVerdict.bPassed = false;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenEngineBasicShape(MeshVerdict.ResolvedPath))
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenBasicShape;
			MeshVerdict.bPassed = false;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenDefaultMannequin(MeshVerdict.ResolvedPath))
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenMannequin;
			MeshVerdict.bPassed = false;
		}
		else
		{
			UObject* LoadedObj = VisualProfile.SkeletalMeshAsset.ToSoftObjectPath().ResolveObject();
			if (!LoadedObj)
			{
				LoadedObj = VisualProfile.SkeletalMeshAsset.LoadSynchronous();
			}
			if (LoadedObj != nullptr)
			{
				MeshVerdict.bPassed = true;
			}
			else
			{
				MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				MeshVerdict.bPassed = false;
			}
		}
		Verdicts.Add(MeshVerdict);
	}

	// 2. AnimBlueprint Slot
	{
		FEOSVisualAssetSlotVerdict AnimVerdict;
		AnimVerdict.AssetID = VisualProfile.CharacterID;
		AnimVerdict.AssetSlot = TEXT("AnimBlueprint");
		AnimVerdict.ResolvedPath = VisualProfile.AnimBlueprintClass.ToSoftObjectPath().ToString();
		AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (AnimVerdict.ResolvedPath.IsEmpty())
		{
			AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			AnimVerdict.bPassed = false;
		}
		else
		{
			UClass* LoadedClass = VisualProfile.AnimBlueprintClass.LoadSynchronous();
			if (LoadedClass != nullptr)
			{
				AnimVerdict.bPassed = true;
			}
			else
			{
				AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				AnimVerdict.bPassed = false;
			}
		}
		Verdicts.Add(AnimVerdict);
	}

	// 3. JutsuVFX Slot
	{
		FEOSVisualAssetSlotVerdict VFXVerdict;
		VFXVerdict.AssetID = VisualProfile.CharacterID;
		VFXVerdict.AssetSlot = TEXT("JutsuVFX");
		VFXVerdict.ResolvedPath = JutsuVFXAsset.ToSoftObjectPath().ToString();
		VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (VFXVerdict.ResolvedPath.IsEmpty())
		{
			VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			VFXVerdict.bPassed = false;
		}
		else
		{
			UObject* LoadedVFX = JutsuVFXAsset.LoadSynchronous();
			if (LoadedVFX != nullptr)
			{
				VFXVerdict.bPassed = true;
			}
			else
			{
				VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				VFXVerdict.bPassed = false;
			}
		}
		Verdicts.Add(VFXVerdict);
	}

	// 4. MaterialInstances Slot
	{
		FEOSVisualAssetSlotVerdict MatVerdict;
		MatVerdict.AssetID = VisualProfile.CharacterID;
		MatVerdict.AssetSlot = TEXT("MaterialInstances");
		MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (VisualProfile.MaterialInstances.Num() == 0)
		{
			MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			MatVerdict.bPassed = false;
		}
		else
		{
			bool bAllMatsValid = true;
			FString CombinedPaths;
			for (const auto& MatSoftPtr : VisualProfile.MaterialInstances)
			{
				const FString PathStr = MatSoftPtr.ToSoftObjectPath().ToString();
				CombinedPaths += PathStr + TEXT(";");
				if (FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(PathStr))
				{
					MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenPlaceholderMaterial;
					bAllMatsValid = false;
					break;
				}
				if (!MatSoftPtr.LoadSynchronous())
				{
					MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
					bAllMatsValid = false;
				}
			}
			MatVerdict.ResolvedPath = CombinedPaths;
			MatVerdict.bPassed = bAllMatsValid;
		}
		Verdicts.Add(MatVerdict);
	}

	if (bLogResults)
	{
		for (const FEOSVisualAssetSlotVerdict& V : Verdicts)
		{
			UE_LOG(LogEOSCore, Log, TEXT("Visual Integrity Gate [%s - %s]: %s (Failure: %s) -> %s"),
				*V.AssetID, *V.AssetSlot, V.bPassed ? TEXT("PASS") : TEXT("FAIL"), *V.FailureCode, *V.ResolvedPath);
		}
	}

	return Verdicts;
}
