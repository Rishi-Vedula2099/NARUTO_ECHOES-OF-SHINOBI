#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSGameModeDataTypes.h"
#include "EOSGameModeSubsystem.generated.h"

/**
 * GameInstanceSubsystem managing the unified Game Mode Framework,
 * lifecycle state machines, scoring engines, idempotent rewards,
 * weekly seeded bounties, and adaptive boss rematches.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSGameModeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSGameModeSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Master Database Initialization
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	void InitializeFullModeDatabase();

	// Mode Registry & Query
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	void RegisterGameMode(const FEOSGameModeRecord& Record);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	bool GetGameMode(const FString& ModeID, FEOSGameModeRecord& OutRecord) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	TArray<FEOSGameModeRecord> GetModesByCategory(EEOSGameModeCategory Category) const;

	// Mode Lifecycle State Machine
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool StartModeRun(const FString& ModeID, FString& OutRunID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool PauseModeRun();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool ResumeModeRun();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool CompleteModeRun(const FString& RunID, float ClearTime, int32 DamageTaken, int32 MaxCombo, int32 PerfectDodges, FEOSModeRunResult& OutResult);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool FailModeRun(const FString& RunID, const FString& FailureReason);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	EEOSModeState GetActiveModeState() const { return CurrentState; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	FString GetActiveRunID() const { return ActiveRunID; }

	// Objective Tracking & Scoring
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool EvaluateObjective(const FString& ObjectiveID, int32 ProgressCount);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	int32 CalculateRunScore(float ClearTime, int32 DamageTaken, int32 MaxCombo, int32 PerfectDodges, int32 CompletedObjectives, FString& OutGrade) const;

	// Idempotent Rewards
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool ClaimRunRewards(const FString& RunID, TArray<FEOSModeRewardEntry>& OutGrantedRewards);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	bool GetModeProgress(const FString& ModeID, FEOSModeProgress& OutProgress) const;

	// Weekly Bounty System
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	void RegisterWeeklyBounty(const FEOSBountyDefinition& Bounty);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	bool GetActiveWeeklyBounty(FEOSBountyDefinition& OutBounty) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	bool ClaimBountyReward(const FString& BountyID, TArray<FEOSModeRewardEntry>& OutRewards);

	// Adaptive Boss Rematch Integration
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	bool LoadBossRematchMemory(const FString& BossID, FString& OutMemoryProfileID) const;

	// Auditing & Counts
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|GameModes")
	int32 GetTotalRegisteredModesCount() const { return ModeRegistry.Num(); }

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|GameModes")
	void AuditGameModeCoverage(int32& OutStoryCount, int32& OutTrainingCount, int32& OutChallengeCount, int32& OutLiveCount, int32& OutWorldCount) const;

private:
	UPROPERTY()
	TMap<FString, FEOSGameModeRecord> ModeRegistry;

	UPROPERTY()
	TMap<FString, FEOSModeProgress> ProgressRegistry;

	UPROPERTY()
	TMap<FString, FEOSBountyDefinition> BountyRegistry;

	UPROPERTY()
	TMap<FString, FEOSModeRunResult> RunResultRegistry;

	UPROPERTY()
	EEOSModeState CurrentState = EEOSModeState::Available;

	UPROPERTY()
	FString ActiveRunID = "";

	UPROPERTY()
	FString ActiveModeID = "";

	UPROPERTY()
	FString ActiveBountyID = "BOUNTY_WEEK_01_ZABUZA";

	int32 RunCounter = 0;
};
