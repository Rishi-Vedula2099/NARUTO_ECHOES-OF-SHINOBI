#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSStoryDataTypes.h"
#include "EOSStorySubsystem.generated.h"

/**
 * GameInstanceSubsystem managing data-driven Arc, Chapter, and Stage loading (Arcs 01-25),
 * runtime objective tracking, world-state deltas, Motion Manga playback, and Story Archive unlocks.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSStorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSStorySubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Registration Functions
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void RegisterArc(const FEOSArcRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void RegisterChapter(const FEOSChapterRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void RegisterStage(const FEOSStageRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void RegisterMotionManga(const FEOSMotionMangaSequence& Sequence);

	// Data-Driven Stage Loading & Execution
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	bool LoadStage(const FString& StageID, FEOSStageRecord& OutStageRecord) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	bool StartStage(const FString& StageID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	bool EvaluateStageObjectives(const FString& StageID, const FString& ObjectiveID, bool bCompleted);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	bool CompleteStage(const FString& StageID, FString& OutWorldStateDeltaID, FString& OutMotionMangaID);

	// Full Campaign Database Initialization & Query
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void InitializeFullCampaignDatabase();

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	int32 GetTotalRegisteredArcCount() const { return ArcRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	int32 GetTotalRegisteredChapterCount() const { return ChapterRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	int32 GetTotalRegisteredStageCount() const { return StageRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	bool GetArcRecord(const FString& ArcID, FEOSArcRecord& OutArc) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	bool GetChapterRecord(const FString& ChapterID, FEOSChapterRecord& OutChapter) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	void AuditCampaignCompleteness(int32& OutCanonCount, int32& OutOriginalCount, int32& OutWhatIfCount, int32& OutTodoCount) const;

	// Motion Manga & Story Archive Unlocks
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Story")
	bool TriggerMotionManga(const FString& MotionSequenceID, FEOSStoryArchiveEntry& OutArchiveEntry);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	FString GetActiveWorldState() const { return CurrentWorldStateID; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Story")
	TArray<FEOSStoryArchiveEntry> GetUnlockedStoryArchive() const;

private:
	UPROPERTY()
	TMap<FString, FEOSArcRecord> ArcRegistry;

	UPROPERTY()
	TMap<FString, FEOSChapterRecord> ChapterRegistry;

	UPROPERTY()
	TMap<FString, FEOSStageRecord> StageRegistry;

	UPROPERTY()
	TMap<FString, FEOSMotionMangaSequence> MotionMangaRegistry;

	UPROPERTY()
	TMap<FString, FEOSStoryArchiveEntry> ArchiveRegistry;

	UPROPERTY()
	FString CurrentWorldStateID = "WORLD_STATE_KONOHA_NORMAL";
};
