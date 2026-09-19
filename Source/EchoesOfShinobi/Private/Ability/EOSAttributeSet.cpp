#include "Ability/EOSAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "EchoesOfShinobi.h"

UEOSAttributeSet::UEOSAttributeSet()
{
	InitHealth(1000.0f);
	InitMaxHealth(1000.0f);
	InitChakra(500.0f);
	InitMaxChakra(500.0f);
	InitUltimateEnergy(0.0f);
	InitMaxUltimateEnergy(100.0f);
	InitAttackPower(100.0f);
	InitDefensePower(50.0f);
	InitMovementSpeed(600.0f);
}

void UEOSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetChakraAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxChakra());
	}
	else if (Attribute == GetUltimateEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxUltimateEnergy());
	}
}

void UEOSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
