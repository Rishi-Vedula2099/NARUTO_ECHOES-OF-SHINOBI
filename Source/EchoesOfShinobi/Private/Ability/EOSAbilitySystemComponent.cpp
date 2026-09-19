#include "../../Public/Ability/EOSAbilitySystemComponent.h"
#include "../../EchoesOfShinobi.h"

UEOSAbilitySystemComponent::UEOSAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEOSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	UE_LOG(LogEOSAbility, Verbose, TEXT("Ability Input Tag Pressed: %s"), *InputTag.ToString());
}

void UEOSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	UE_LOG(LogEOSAbility, Verbose, TEXT("Ability Input Tag Released: %s"), *InputTag.ToString());
}
