#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSJutsuTrialDataTypes.h"
#include "EOSJutsuTrialSubsystem.generated.h"

/**
 * Subsystem managing Jutsu Eligibility, Learning Trials, Hand-Sign QTEs, Chakra Balancing,
 * Mastery Level Progression, and Archive Loadout Management.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSJutsuTrialSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSJutsuTrialSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Register Jutsu Trial Record
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	void RegisterJutsuRecord(const FEOSJutsuTrialRecord& InRecord);

	// Evaluate Jutsu Eligibility based on Advancement Node Milestone
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool EvaluateEligibility(const FString& JutsuID, const FString& ReachedAdvancementNodeID);

	// Start Trial Sequence
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool StartTrial(const FString& JutsuID);

	// Validate Hand-Sign Execution Sequence
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool ValidateHandSignSequence(const FString& JutsuID, const TArray<EEOSHandSign>& SubmittedSigns, float TimeTakenSeconds);

	// Validate Chakra Control Stability
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool ValidateChakraControl(const FString& JutsuID, float CurrentGaugeValue, float DurationHeldSeconds);

	// Finalize Trial Attempt (Success or Failure with Practice XP Retention)
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	EEOSJutsuTrialState ProcessTrialResult(const FString& JutsuID, bool bPassed, float EarnedPracticeXP);

	// Loadout Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool EquipJutsu(const FString& JutsuID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool StoreJutsu(const FString& JutsuID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|JutsuTrial")
	bool ReplaceJutsu(const FString& OldJutsuID, const FString& NewJutsuID);

	// Queries
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|JutsuTrial")
	bool GetJutsuRecord(const FString& JutsuID, FEOSJutsuTrialRecord& OutRecord) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|JutsuTrial")
	TArray<FEOSJutsuTrialRecord> GetLearnedArchive(const FString& CharacterID) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|JutsuTrial")
	TArray<FEOSJutsuTrialRecord> GetEquippedJutsu(const FString& CharacterID) const;

private:
	UPROPERTY()
	TMap<FString, FEOSJutsuTrialRecord> JutsuRegistry;
};
