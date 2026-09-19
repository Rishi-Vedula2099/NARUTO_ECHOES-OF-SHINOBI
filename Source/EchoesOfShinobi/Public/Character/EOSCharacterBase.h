#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Ability/EOSAbilitySystemComponent.h"
#include "Ability/EOSAttributeSet.h"
#include "Data/EOSCharacterDefinition.h"
#include "EOSCharacterBase.generated.h"

UCLASS()
class ECHOESOFSHINOBI_API AEOSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEOSCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UEOSAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOS Data")
	TObjectPtr<UEOSCharacterDefinition> CharacterDefinitionAsset;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS Ability")
	TObjectPtr<UEOSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS Ability")
	TObjectPtr<UEOSAttributeSet> AttributeSet;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
};
