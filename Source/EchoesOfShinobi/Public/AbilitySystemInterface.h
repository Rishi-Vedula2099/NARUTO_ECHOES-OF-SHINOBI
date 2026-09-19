#pragma once

#if __has_include_next(<AbilitySystemInterface.h>)
#include_next <AbilitySystemInterface.h>
#else
#include "CoreMinimal.h"

class IAbilitySystemInterface
{
public:
    virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const = 0;
};
#endif
