#pragma once

#include "../CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "../Ability/EOSAbilitySystemComponent.h"
#include "../Ability/EOSAttributeSet.h"
#include "../Data/EOSCharacterDefinition.h"
#include "EOSCharacterBase.generated.h"

/**
 * Unified Base Character Class for all 78 playable shinobi, ANBU units, and bosses.
 * Prevents 78 separate hardcoded C++ classes by consuming authorable 10-layer data assets.
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
};
