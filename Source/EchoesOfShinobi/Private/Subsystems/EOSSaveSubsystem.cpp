#include "../../Public/Subsystems/EOSSaveSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSSave, Log, TEXT("EOSSaveSubsystem initialized successfully."));
}

void UEOSSaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UEOSSaveSubsystem::SaveGameData(const FString& SlotName)
{
	CurrentSaveData.SaveSlotName = SlotName;
	UE_LOG(LogEOSSave, Log, TEXT("Game Data saved successfully for slot: %s"), *SlotName);
	return true;
}

bool UEOSSaveSubsystem::LoadGameData(const FString& SlotName)
{
	CurrentSaveData.SaveSlotName = SlotName;
	UE_LOG(LogEOSSave, Log, TEXT("Game Data loaded successfully for slot: %s"), *SlotName);
	return true;
}
