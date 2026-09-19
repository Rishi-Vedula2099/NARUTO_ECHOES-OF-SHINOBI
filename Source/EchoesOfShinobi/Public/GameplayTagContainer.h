#pragma once

#if __has_include_next(<GameplayTagContainer.h>)
#include_next <GameplayTagContainer.h>
#else
#include "CoreMinimal.h"

struct FGameplayTag
{
    FName TagName;
    FGameplayTag() {}
    FGameplayTag(FName InName) : TagName(InName) {}
    bool IsValid() const { return !TagName.IsNone(); }
    FString ToString() const { return TagName.ToString(); }
};
#endif
