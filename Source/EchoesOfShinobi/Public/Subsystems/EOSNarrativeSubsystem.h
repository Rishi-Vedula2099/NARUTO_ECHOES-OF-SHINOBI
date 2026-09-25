#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSNarrativeDataTypes.h"
#include "EOSNarrativeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSNarrativeStateChangedDelegate, EEOSNarrativeState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSNarrativeSequenceCompletedDelegate, const FString&, SequenceID);

/**
 * GameInstanceSubsystem orchestrating the post-mission narrative pipeline,
 * state machine, input handoff, HUD fade, music transitions, and idempotent transactions.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSNarrativeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSNarrativeSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Database Initialization
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void InitializeDefaultNarrativeDatabase();

	// Authoritative Post-Mission Trigger
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool StartMissionCompleteSequence(const FEOSStageResult& StageResult);

	// Boss Defeat Signal & Combat Lock
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool TriggerBossDefeatHandoff(const FString& BossID);

	// State Machine Controls
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool TransitionToState(EEOSNarrativeState NewState);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	EEOSNarrativeState GetCurrentState() const { return CurrentState; }

	// Step-by-Step Flow Controls
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool AdvanceToNextStep();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool SkipSequence();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool PauseSequence();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool ResumeSequence();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void SetPlaybackSpeed(float Speed);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	float GetPlaybackSpeed() const { return ActivePlaybackSpeed; }

	// Input & Context Preservation
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void CapturePreNarrativeInputContext(const FVector& Location, const FRotator& Rotation, bool bTargetLocked);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool RestorePreNarrativeInputContext();

	// Transition Profiles & Music Transitions
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void RegisterTransitionProfile(const FEOSNarrativeTransitionProfile& Profile);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	bool GetTransitionProfile(const FString& ProfileID, FEOSNarrativeTransitionProfile& OutProfile) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void RegisterMusicTransition(const FEOSMusicTransitionRecord& Record);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	bool GetMusicTransition(const FString& TransitionID, FEOSMusicTransitionRecord& OutTransition) const;

	// Idempotent Presentation & Commit Logic
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool PresentRewards(const TArray<FEOSRewardPresentationItem>& Rewards, bool bIsReplay = false);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool CommitWorldStateDelta(const FString& DeltaID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool UnlockStoryArchiveEntry(const FString& ArchiveID, const FString& SequenceID);

	// Transaction Safety & Crash Recovery
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool GetTransaction(const FString& TransactionID, FEOSNarrativeTransaction& OutTransaction) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	bool ResumePendingTransaction(const FString& TransactionID);

	// Settings & Accessibility
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void UpdateNarrativeSettings(const FEOSNarrativeSettings& NewSettings);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	FEOSNarrativeSettings GetNarrativeSettings() const { return Settings; }

	// Sequence Registry & Queries
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Narrative")
	void RegisterSequence(const FEOSMotionMangaSequenceRecord& Sequence);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	bool GetSequence(const FString& SequenceID, FEOSMotionMangaSequenceRecord& OutSequence) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	FString GetActiveSequenceID() const { return ActiveSequenceID; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	FString GetActiveWorldStateID() const { return CurrentWorldStateID; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	TArray<FString> GetPresentedRewardIDs() const { return GrantedRewardHistory; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Narrative")
	TArray<FString> GetUnlockedArchiveIDs() const { return UnlockedArchiveEntries; }

public:
	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|Narrative")
	FEOSNarrativeStateChangedDelegate OnNarrativeStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|Narrative")
	FEOSNarrativeSequenceCompletedDelegate OnNarrativeSequenceCompleted;

private:
	UPROPERTY()
	EEOSNarrativeState CurrentState = EEOSNarrativeState::None;

	UPROPERTY()
	FString ActiveSequenceID;

	UPROPERTY()
	FString ActiveTransactionID;

	UPROPERTY()
	FString ActiveStageID;

	UPROPERTY()
	FString ActiveWorldStateDeltaID;

	UPROPERTY()
	FString CurrentWorldStateID = "WORLD_STATE_KONOHA_NORMAL";

	UPROPERTY()
	float ActivePlaybackSpeed = 1.0f;

	UPROPERTY()
	bool bIsPaused = false;

	UPROPERTY()
	FEOSPreNarrativeInputContext SavedInputContext;

	UPROPERTY()
	FEOSNarrativeSettings Settings;

	UPROPERTY()
	TMap<FString, FEOSMotionMangaSequenceRecord> SequenceRegistry;

	UPROPERTY()
	TMap<FString, FEOSNarrativeTransitionProfile> TransitionProfiles;

	UPROPERTY()
	TMap<FString, FEOSMusicTransitionRecord> MusicTransitions;

	UPROPERTY()
	TMap<FString, FEOSNarrativeTransaction> TransactionLedger;

	UPROPERTY()
	TArray<FString> GrantedRewardHistory;

	UPROPERTY()
	TArray<FString> UnlockedArchiveEntries;
};
