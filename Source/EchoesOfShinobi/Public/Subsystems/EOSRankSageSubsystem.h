#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSRankSageDataTypes.h"
#include "EOSRankSageSubsystem.generated.h"

/**
 * GameInstanceSubsystem managing Rank Advancement, Mentor Training,
 * Sage Mode Compatibility, Nature Energy Stability Math, and Trial Persistence.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSRankSageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSRankSageSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Registration Functions
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	void RegisterRankProgression(const FEOSRankProgressionRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	void RegisterSagePathway(const FEOSSagePathwayRecord& Record);

	// Rank Progression Logic
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	bool EvaluateRankEligibility(const FString& CharacterID, EEOSShinobiRank TargetRank, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	bool AdvanceRank(const FString& CharacterID, EEOSShinobiRank NewRank);

	// Mentor System
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	FEOSMentorTrainingRecord ExecuteMentorSession(const FString& CharacterID, const FString& MentorID, const FString& TeachingFocus);

	// Sage Compatibility & Nature Energy Control
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|RankSage")
	EEOSSageCompatibility CheckSageCompatibility(const FString& CharacterID, EEOSCharacterEra Era) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|RankSage")
	bool ValidateNatureEnergyBalance(const FEOSNatureEnergyModel& Model, float CurrentEnergyValue, float DurationHeldSeconds) const;

	// Rank & Sage Trial Completion
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	bool ProcessRankTrialResult(const FString& CharacterID, const FString& TrialID, bool bPassed, float TimeTakenSeconds, float DamageTaken);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|RankSage")
	bool ProcessSageTrialResult(const FString& CharacterID, const FString& SagePathID, bool bPassed);

	// Queries
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|RankSage")
	bool GetRankRecord(const FString& CharacterID, FEOSRankProgressionRecord& OutRecord) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|RankSage")
	bool GetSageRecord(const FString& CharacterID, FEOSSagePathwayRecord& OutRecord) const;

private:
	UPROPERTY()
	TMap<FString, FEOSRankProgressionRecord> RankRegistry;

	UPROPERTY()
	TMap<FString, FEOSSagePathwayRecord> SageRegistry;
};
