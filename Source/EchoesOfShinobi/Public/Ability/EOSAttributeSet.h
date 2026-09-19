#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EOSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ECHOESOFSHINOBI_API UEOSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEOSAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Chakra")
	FGameplayAttributeData Chakra;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, Chakra)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Chakra")
	FGameplayAttributeData MaxChakra;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, MaxChakra)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Energy")
	FGameplayAttributeData UltimateEnergy;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, UltimateEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Energy")
	FGameplayAttributeData MaxUltimateEnergy;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, MaxUltimateEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Locomotion")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UEOSAttributeSet, MovementSpeed)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
