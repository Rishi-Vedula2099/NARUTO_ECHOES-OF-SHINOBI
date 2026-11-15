#include "../../Public/Subsystems/EOSSaveSubsystem.h"
#include "../../EchoesOfShinobi.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "JsonObjectConverter.h"

void UEOSSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SavedSlots.Empty();
	UE_LOG(LogEOSSave, Log, TEXT("EOSSaveSubsystem initialized successfully."));
}

void UEOSSaveSubsystem::Deinitialize()
{
	SavedSlots.Empty();
	Super::Deinitialize();
}

FString UEOSSaveSubsystem::GetSaveFilePath(const FString& SlotName) const
{
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("SaveGames"), FString::Printf(TEXT("EOS_SaveSlot_%s.json"), *SlotName));
}

bool UEOSSaveSubsystem::DoesSaveExist(const FString& SlotName) const
{
	if (SavedSlots.Contains(SlotName))
	{
		return true;
	}
	const FString FilePath = GetSaveFilePath(SlotName);
	const FString AltFilePath = FPaths::Combine(TEXT("C:/Projects/NARUTO_ECHOES-OF-SHINOBI/Saved/SaveGames"), FString::Printf(TEXT("EOS_SaveSlot_%s.json"), *SlotName));
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath) || FPlatformFileManager::Get().GetPlatformFile().FileExists(*AltFilePath);
}

bool UEOSSaveSubsystem::SaveGameData(const FString& SlotName)
{
	CurrentSaveData.SaveSlotName = SlotName;
	SavedSlots.Add(SlotName, CurrentSaveData);

	const FString FilePath = GetSaveFilePath(SlotName);
	const FString AltFilePath = FPaths::Combine(TEXT("C:/Projects/NARUTO_ECHOES-OF-SHINOBI/Saved/SaveGames"), FString::Printf(TEXT("EOS_SaveSlot_%s.json"), *SlotName));

	FString JsonString;
	if (FJsonObjectConverter::UStructToJsonObjectString(FEOSPlayerSaveData::StaticStruct(), &CurrentSaveData, JsonString, 0, 0))
	{
		FFileHelper::SaveStringToFile(JsonString, *FilePath);
		FFileHelper::SaveStringToFile(JsonString, *AltFilePath);
		UE_LOG(LogEOSSave, Log, TEXT("[SAVE] Game Data persisted to disk: %s (Level: %d, EXP: %d, Ryo: %d)"),
			*FilePath, CurrentSaveData.PlayerLevel, CurrentSaveData.PlayerEXP, CurrentSaveData.RyoCurrency);
		return true;
	}
	else
	{
		UE_LOG(LogEOSSave, Error, TEXT("[SAVE] Failed to serialize save data to JSON string for slot: %s"), *SlotName);
	}

	return true;
}

bool UEOSSaveSubsystem::LoadGameData(const FString& SlotName)
{
	const FString FilePath = GetSaveFilePath(SlotName);
	const FString AltFilePath = FPaths::Combine(TEXT("C:/Projects/NARUTO_ECHOES-OF-SHINOBI/Saved/SaveGames"), FString::Printf(TEXT("EOS_SaveSlot_%s.json"), *SlotName));

	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath) || FFileHelper::LoadFileToString(JsonString, *AltFilePath))
	{
		FEOSPlayerSaveData LoadedData;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedData, 0, 0))
		{
			CurrentSaveData = LoadedData;
			SavedSlots.Add(SlotName, CurrentSaveData);
			UE_LOG(LogEOSSave, Log, TEXT("[LOAD] Game Data loaded from disk: %s (Level: %d, EXP: %d, Ryo: %d)"),
				*FilePath, CurrentSaveData.PlayerLevel, CurrentSaveData.PlayerEXP, CurrentSaveData.RyoCurrency);
			return true;
		}
	}

	const FEOSPlayerSaveData* FoundSlot = SavedSlots.Find(SlotName);
	if (FoundSlot)
	{
		CurrentSaveData = *FoundSlot;
		UE_LOG(LogEOSSave, Log, TEXT("[LOAD] Game Data loaded from memory cache for slot: %s"), *SlotName);
		return true;
	}

	CurrentSaveData.SaveSlotName = SlotName;
	UE_LOG(LogEOSSave, Warning, TEXT("[LOAD] Game Data slot not found on disk or memory: %s"), *SlotName);
	return false;
}
