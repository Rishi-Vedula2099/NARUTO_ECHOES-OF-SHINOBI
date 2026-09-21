#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSMetaProgressionDataTypes.h"
#include "EOSShinobiIntelSubsystem.generated.h"

/**
 * Reusable Runtime Shinobi Intel & Knowledge Graph Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSShinobiIntelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates canon character, enemy, boss, and lore intel records */
	void InitializeDefaultIntelDatabase();

	// -------------------------------------------------------------
	// Intel Registry & Discovery Tracking
	// -------------------------------------------------------------
	bool RegisterIntelRecord(const FEOSShinobiIntelRecord& Record);
	bool GetIntelRecord(const FString& IntelID, FEOSShinobiIntelRecord& OutRecord) const;
	TArray<FEOSShinobiIntelRecord> GetIntelByCategory(EEOSIntelCategory Category) const;

	bool LogEncounter(const FString& IntelID);
	bool SetDiscoveryState(const FString& IntelID, EEOSDiscoveryState State);

	// -------------------------------------------------------------
	// Knowledge Graph Navigation
	// -------------------------------------------------------------
	TArray<FString> GetCrossLinkedEntities(const FString& IntelID) const;
	bool AddCrossLink(const FString& IntelIDA, const FString& IntelIDB);

	// -------------------------------------------------------------
	// Metrics & Research
	// -------------------------------------------------------------
	int32 GetTotalDiscoveredCount() const;
	float GetCategoryResearchPercentage(EEOSIntelCategory Category) const;

private:
	TMap<FString, FEOSShinobiIntelRecord> IntelRegistry;
};
