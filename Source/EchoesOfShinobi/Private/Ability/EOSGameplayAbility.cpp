#include "../../Public/Ability/EOSGameplayAbility.h"
#include "../../EchoesOfShinobi.h"

UEOSGameplayAbility::UEOSGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEOSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AbilityDataAsset)
	{
		UE_LOG(LogEOSAbility, Log, TEXT("Activated Ability: %s (Type: %d, Chakra Cost: %f)"),
			*AbilityDataAsset->DisplayName.ToString(),
			static_cast<int32>(AbilityDataAsset->JutsuType),
			AbilityDataAsset->ChakraCost);
	}
}
