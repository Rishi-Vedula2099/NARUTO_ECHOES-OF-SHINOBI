#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EOSDataTypes.h"
#include "EOSSaveSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FEOSPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FString SaveSlotName = "DefaultShinobiSlot";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	int32 RyoCurrency = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	TArray<FName> UnlockedCharacterIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	TArray<FName> CompletedStageIds;
};

UCLASS()
class ECHOESOFSHINOBI_API UEOSSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "EOS Save")
	bool SaveGameData(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "EOS Save")
	bool LoadGameData(const FString& SlotName);

	UFUNCTION(BlueprintPure, Category = "EOS Save")
	const FEOSPlayerSaveData& GetCurrentSaveData() const { return CurrentSaveData; }

private:
	FEOSPlayerSaveData CurrentSaveData;
};
