#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EOSAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFSHINOBI_API UEOSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UEOSAbilitySystemComponent();

	UFUNCTION(BlueprintCallable, Category = "EOS Ability")
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "EOS Ability")
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
};
