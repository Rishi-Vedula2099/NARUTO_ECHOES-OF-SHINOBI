#include "Subsystems/EOSEventMessageSubsystem.h"
#include "EchoesOfShinobi.h"

void UEOSEventMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSCore, Log, TEXT("EOSEventMessageSubsystem initialized."));
}

void UEOSEventMessageSubsystem::BroadcastMessage(FGameplayTag ChannelTag, const FString& MessagePayload)
{
	UE_LOG(LogEOSCore, Verbose, TEXT("Broadcasting Event Message on Channel '%s': %s"), *ChannelTag.ToString(), *MessagePayload);
	OnGameplayEventMessage.Broadcast(ChannelTag, MessagePayload);
}
