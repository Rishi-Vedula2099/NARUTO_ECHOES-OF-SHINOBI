#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSMetaProgressionDataTypes.h"
#include "EOSCollectionSubsystem.generated.h"

/**
 * Reusable Runtime Shinobi Collection, Archive & Profile Meta Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSCollectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates initial entries across all 11 collection categories */
	void InitializeDefaultCollectionDatabase();

	// -------------------------------------------------------------
	// Collection Entry Registry & States
	// -------------------------------------------------------------
	bool RegisterCollectionEntry(const FEOSCollectionEntryRecord& Entry);
	bool GetCollectionEntry(const FString& EntryID, FEOSCollectionEntryRecord& OutEntry) const;
	TArray<FEOSCollectionEntryRecord> GetEntriesByCategory(EEOSCollectionCategory Category) const;

	bool SetEntryOwned(const FString& EntryID, bool bOwned);
	bool SetEntryMastered(const FString& EntryID, bool bMastered);

	// -------------------------------------------------------------
	// Aggregate Progress & Account Profile Metrics
	// -------------------------------------------------------------
	float GetCategoryCompletionPercentage(EEOSCollectionCategory Category) const;
	float GetGlobalArchiveCompletionPercentage() const;
	int32 GetArchiveAccountLevel() const;

	// -------------------------------------------------------------
	// Milestone Rewards Pipeline
	// -------------------------------------------------------------
	int32 EvaluateMilestoneRewards(int32& OutGrantedGems, FString& OutUnlockedTitle);

private:
	TMap<FString, FEOSCollectionEntryRecord> CollectionRegistry;
	int32 LastClaimedMilestoneTier = 0; // 0=None, 1=25%, 2=50%, 3=75%, 4=100%
};
