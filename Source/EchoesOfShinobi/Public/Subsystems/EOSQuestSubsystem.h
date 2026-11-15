#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSStoryDataTypes.h"
#include "../Data/EOSNarrativeDataTypes.h"
#include "EOSSaveSubsystem.h"
#include "EOSQuestSubsystem.generated.h"

UENUM(BlueprintType)
enum class EEOSQuestState : uint8
{
	NotStarted    UMETA(DisplayName = "Not Started"),
	Active        UMETA(DisplayName = "Active In Progress"),
	ObjectivesMet UMETA(DisplayName = "Objectives Met (Ready for Turn-in)"),
	Completed     UMETA(DisplayName = "Completed"),
	Failed        UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSQuestObjectiveProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsMandatory = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsCompleted = false;
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSDialogueChoiceRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString ChoiceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString ActionTag;
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNPCConversation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString SpeakerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FEOSDialogueChoiceRecord> Choices;
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSQuestDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestGiverNPCID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString AssociatedStageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FEOSQuestObjectiveProgress> DefaultObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 RewardRyo = 2500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 RewardEXP = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString WorldStateDeltaOnComplete;
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSActiveQuestRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EEOSQuestState State = EEOSQuestState::NotStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FEOSQuestObjectiveProgress> Objectives;
};

/**
 * Reusable Quest Subsystem orchestrating the Hinata Outskirts quest lifecycle,
 * NPC dialogue branching, objective evaluation, mission turn-in, reward handoff,
 * and save state synchronization.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void InitializeDefaultQuests();

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	bool RegisterQuestDefinition(const FEOSQuestDefinition& QuestDef);

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	bool StartQuestDialogue(const FString& NPCID, FEOSNPCConversation& OutConversation);

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	bool AcceptQuest(const FString& QuestID);

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	bool UpdateObjectiveProgress(const FString& QuestID, const FString& ObjectiveID, bool bCompleted);

	UFUNCTION(BlueprintPure, Category = "EOS|Quest")
	bool IsQuestReadyForCompletion(const FString& QuestID) const;

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	bool CompleteQuest(const FString& QuestID, FEOSStageResult& OutStageResult, TArray<FEOSRewardPresentationItem>& OutRewards);

	UFUNCTION(BlueprintPure, Category = "EOS|Quest")
	EEOSQuestState GetQuestStatus(const FString& QuestID) const;

	UFUNCTION(BlueprintPure, Category = "EOS|Quest")
	bool GetActiveQuest(const FString& QuestID, FEOSActiveQuestRecord& OutRecord) const;

	UFUNCTION(BlueprintPure, Category = "EOS|Quest")
	const TArray<FString>& GetCompletedQuestIDs() const { return CompletedQuestIDs; }

	// Persistence synchronization
	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	void PopulateSaveData(FEOSPlayerSaveData& OutSaveData) const;

	UFUNCTION(BlueprintCallable, Category = "EOS|Quest")
	void RestoreFromSaveData(const FEOSPlayerSaveData& InSaveData);

private:
	TMap<FString, FEOSQuestDefinition> QuestRegistry;
	TMap<FString, FEOSActiveQuestRecord> ActiveQuests;
	TArray<FString> CompletedQuestIDs;
};

