#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSGachaDataTypes.h"
#include "EOSLiveOpsSubsystem.generated.h"

/**
 * Reusable Runtime LiveOps, Events, and Weekly Bounty Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSLiveOpsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates all default events, bounties, and LiveOps schedule */
	void InitializeDefaultLiveOpsDatabase();

	// -------------------------------------------------------------
	// Limited Event Management
	// -------------------------------------------------------------
	bool RegisterLimitedEvent(const FEOSLimitedEventDefinition& EventDef);
	bool GetLimitedEvent(const FString& EventID, FEOSLimitedEventDefinition& OutEvent) const;
	TArray<FEOSLimitedEventDefinition> GetActiveEvents() const;
	bool AdvanceEventPhase(const FString& EventID);
	bool SetEventActiveState(const FString& EventID, bool bIsActive);

	// -------------------------------------------------------------
	// Event Shop Pipeline
	// -------------------------------------------------------------
	bool PurchaseShopItem(const FString& EventID, const FString& ItemID, int32& OutGrantedQty, FString& OutStatusMessage);
	TArray<FEOSShopItemDefinition> GetEventShopItems(const FString& EventID) const;

	// -------------------------------------------------------------
	// Configurable Weekly Bounty System (Thursday 00:00 UTC Baseline)
	// -------------------------------------------------------------
	const FEOSBountyScheduleConfig& GetBountyScheduleConfig() const { return BountyScheduleConfig; }
	void ConfigureBountySchedule(const FString& NewResetDay, const FString& NewResetTime, const FString& NewTimezone);
	
	/** Seeded deterministic generation of weekly bounties */
	void GenerateWeeklyBounties(int32 Seed);
	bool GetActiveBounty(const FString& BountyID, FEOSWeeklyBountyRecord& OutBounty) const;
	TArray<FEOSWeeklyBountyRecord> GetActiveBounties() const;

	/** Objective progress & cross-mode completion */
	bool UpdateBountyProgress(const FString& BountyID, int32 Increment);
	bool ClaimBountyReward(const FString& BountyID, int32& OutMedals, int32& OutRyo, FString& OutMaterial);

	// -------------------------------------------------------------
	// Event World-State & Narrative Links
	// -------------------------------------------------------------
	FString GetActiveWorldStateDisturbance() const;
	FString GetActiveEventMotionManga(const FString& EventID) const;

private:
	/** Registered event packages */
	TMap<FString, FEOSLimitedEventDefinition> EventRegistry;

	/** Weekly bounty schedule and active pool */
	FEOSBountyScheduleConfig BountyScheduleConfig;
	TMap<FString, FEOSWeeklyBountyRecord> ActiveWeeklyBounties;

	/** Active player bounty completion records */
	TMap<FString, int32> BountyCompletionHistory;
};
