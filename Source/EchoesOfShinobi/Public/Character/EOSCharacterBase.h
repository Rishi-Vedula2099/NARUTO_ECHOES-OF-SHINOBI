#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "../Ability/EOSAbilitySystemComponent.h"
#include "../Ability/EOSAttributeSet.h"
#include "../Data/EOSCharacterDefinition.h"
#include "../Data/EOSPresentationDataTypes.h"
#include "EOSCharacterBase.generated.h"

/**
 * Unified Base Character Class for all 78 playable shinobi, ANBU units, and bosses.
 * Prevents 78 separate hardcoded C++ classes by consuming authorable 10-layer data assets.
 * Phase 13: Integrated with data-driven camera, movement, and stylized visual profiles.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEOSCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UEOSAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Data-Driven Initialization
	UFUNCTION(BlueprintCallable, Category = "EOS|Character")
	void InitializeFromDefinition(UEOSCharacterDefinition* InDefinitionAsset);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOS Data")
	TObjectPtr<UEOSCharacterDefinition> CharacterDefinitionAsset;

	// Framework Layer Accessors
	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSCombatStatBlock GetStats() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSBasicAttackCombo GetBasicAttacks() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSSkillSetContainer GetSkills() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSJutsuSlotContainer GetJutsu() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSUltimateDefinition GetUltimate() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSFormContainer GetForms() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSGearGridContainer GetEquipment() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSCharacterAdvancementData GetAdvancement() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSCharacterMasteryRecord GetMastery() const;

	UFUNCTION(BlueprintPure, Category = "EOS|Character|Framework")
	FEOSCharacterCompatibilityProfile GetCompatibility() const;

	// Form Transformation Execution
	UFUNCTION(BlueprintCallable, Category = "EOS|Character|Form")
	bool TransformToForm(const FString& TargetFormID);

	// Camera Components & Data-Driven Profiles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Camera")
	FEOSCameraProfile CameraProfile;

	UFUNCTION(BlueprintCallable, Category = "EOS|Camera")
	void SetCameraMode(EEOSCameraMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "EOS|Camera")
	void ApplyCameraProfile(const FEOSCameraProfile& NewProfile);

	// Movement Handlers & Data Profile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Movement")
	FEOSMovementProfile MovementProfile;

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void ApplyMovementProfile(const FEOSMovementProfile& NewProfile);

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void Turn(float Value);

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void LookUp(float Value);

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "EOS|Movement")
	void StopSprint();

	// Combat Handlers
	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void AttackLight();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void AttackHeavy();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastJutsuSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastJutsu1();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastJutsu2();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastJutsu3();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastSubstitution();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void CastUltimateAbility();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void ToggleTransformation();

	UFUNCTION(BlueprintCallable, Category = "EOS|Combat")
	void SwitchSquadMember(int32 MemberIndex);

	// Movement Tuning State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Movement")
	bool bIsSprinting = false;

	// Visual Presentation Profile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Visuals")
	FEOSCharacterVisualProfile VisualProfile;

	UFUNCTION(BlueprintCallable, Category = "EOS|Visuals")
	void ApplyVisualProfile(const FEOSCharacterVisualProfile& NewProfile);

	// Visual Form Transformations (Phases 1, 2, 4, 5)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Visuals")
	FEOSFormVisualProfile CurrentFormVisual;

	UFUNCTION(BlueprintCallable, Category = "EOS|Visuals")
	void ApplyVisualForm(EEOSVisualForm NewForm);

	// Gear Visual Attachments (Phase 11)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Gear")
	FEOSGearSocketProfile GearSockets;

	UFUNCTION(BlueprintCallable, Category = "EOS|Gear")
	void AttachGearVisual(FName SocketName, const FString& GearMeshName);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS Ability")
	TObjectPtr<UEOSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS Ability")
	TObjectPtr<UEOSAttributeSet> AttributeSet;

	// Active Runtime Framework State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS State")
	FEOSCharacterFrameworkBlock RuntimeFramework;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	void SetupStylizedVisualMesh();
};
