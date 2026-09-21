#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSGachaDataTypes.h"
#include "EOSGachaSubsystem.generated.h"

/**
 * Reusable Runtime Gacha & Acquisition Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSGachaSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates all default banners, pools, and rarity configurations */
	void InitializeDefaultGachaDatabase();

	// -------------------------------------------------------------
	// Banner Registry
	// -------------------------------------------------------------
	bool RegisterBanner(const FEOSBannerDefinition& BannerDef);
	bool GetBanner(const FString& BannerID, FEOSBannerDefinition& OutBanner) const;
	TArray<FEOSBannerDefinition> GetActiveBanners() const;

	// -------------------------------------------------------------
	// Pool & Rarity Management
	// -------------------------------------------------------------
	bool RegisterPoolEntry(const FString& PoolID, const FEOSSummonPoolEntry& Entry);
	TArray<FEOSSummonPoolEntry> GetPoolEntries(const FString& PoolID) const;
	bool RegisterPityRule(const FEOSPityRule& Rule);
	bool GetPityRule(const FString& RuleID, FEOSPityRule& OutRule) const;

	// -------------------------------------------------------------
	// Currency & Wallet Management
	// -------------------------------------------------------------
	int32 GetCurrencyBalance(EEOSCurrencyType CurrencyType) const;
	void AddCurrency(EEOSCurrencyType CurrencyType, int32 Amount);
	bool DeductCurrency(EEOSCurrencyType CurrencyType, int32 Amount);
	int32 GetEventCurrencyBalance(const FString& EventCurrencyID) const;
	void AddEventCurrency(const FString& EventCurrencyID, int32 Amount);
	bool DeductEventCurrency(const FString& EventCurrencyID, int32 Amount);

	// -------------------------------------------------------------
	// Pity Tracking & State
	// -------------------------------------------------------------
	int32 GetPityCount(const FString& BannerID) const;
	bool HasGuaranteedFeatured(const FString& BannerID) const;
	void ResetPity(const FString& BannerID);

	// -------------------------------------------------------------
	// Summon Execution Pipeline (Atomic & Transaction-Safe)
	// -------------------------------------------------------------
	FEOSSummonResult ExecuteSingleSummon(const FString& BannerID);
	FEOSSummonResult ExecuteMultiSummon(const FString& BannerID, int32 PullCount = 10);

	// -------------------------------------------------------------
	// Collection & Inventory Query
	// -------------------------------------------------------------
	bool IsCharacterOwned(const FString& CharacterID) const;
	int32 GetCharacterShards(const FString& CharacterID) const;
	const TArray<FEOSSummonResult>& GetSummonHistory() const { return SummonHistory; }

private:
	/** Resolves a single pull within a banner's active pool applying current pity */
	FEOSAcquisitionResult ResolveSinglePullInternal(const FEOSBannerDefinition& Banner, int32& InOutPity, bool& InOutGuaranteeFeatured, bool& OutTriggeredHardPity);

	/** Internal registries */
	TMap<FString, FEOSBannerDefinition> BannerRegistry;
	TMap<FString, TArray<FEOSSummonPoolEntry>> PoolRegistry;
	TMap<FString, FEOSPityRule> PityRuleRegistry;
	TMap<EEOSShinobiRarity, FEOSRarityDefinition> RarityRegistry;

	/** Player runtime inventory & wallet */
	TMap<EEOSCurrencyType, int32> CurrencyWallets;
	TMap<FString, int32> EventCurrencyWallets;
	TSet<FString> OwnedCharacters;
	TMap<FString, int32> CharacterShardInventory;
	int32 AwakeningCurrencyBalance = 0;
	int32 BondTokenBalance = 0;

	/** Banner pity state tracking */
	TMap<FString, int32> BannerPityCounters;
	TMap<FString, bool> BannerFeaturedGuarantees;

	/** Transaction audit trail */
	TArray<FEOSSummonResult> SummonHistory;
	int32 TransactionCounter = 1000;
};
