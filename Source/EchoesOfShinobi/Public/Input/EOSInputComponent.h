#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "EOSInputComponent.generated.h"

USTRUCT(BlueprintType)
struct FEOSInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> InputAction;
};

UCLASS()
class ECHOESOFSHINOBI_API UEOSInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const TArray<FEOSInputActionBinding>& ActionBindings, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UEOSInputComponent::BindAbilityActions(const TArray<FEOSInputActionBinding>& ActionBindings, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	for (const FEOSInputActionBinding& Binding : ActionBindings)
	{
		if (Binding.InputAction && Binding.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Binding.InputAction, ETriggerEvent::Started, Object, PressedFunc, Binding.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Binding.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Binding.InputTag);
			}
		}
	}
}
