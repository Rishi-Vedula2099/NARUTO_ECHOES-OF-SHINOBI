#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "EOSEventMessageSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEOSEventMessageDelegate, FGameplayTag, ChannelTag, const FString&, MessagePayload);

UCLASS()
class ECHOESOFSHINOBI_API UEOSEventMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category = "EOS Events")
	FEOSEventMessageDelegate OnGameplayEventMessage;

	UFUNCTION(BlueprintCallable, Category = "EOS Events")
	void BroadcastMessage(FGameplayTag ChannelTag, const FString& MessagePayload);
};
