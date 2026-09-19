#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/EOSAbilityDefinition.h"
#include "EOSGameplayAbility.generated.h"

UCLASS()
class ECHOESOFSHINOBI_API UEOSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEOSGameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS Ability Data")
	TObjectPtr<UEOSAbilityDefinition> AbilityDataAsset;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
