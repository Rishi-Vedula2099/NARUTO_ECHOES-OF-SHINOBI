#include "../../Public/Subsystems/EOSGachaSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSGachaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultGachaDatabase();
}

void UEOSGachaSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSGachaSubsystem::InitializeDefaultGachaDatabase()
{
	// 1. Setup Rarity Definitions
	FEOSRarityDefinition Rarity3;
	Rarity3.RarityTier = EEOSShinobiRarity::Rarity_3;
	Rarity3.DisplayName = TEXT("Common Shinobi");
	Rarity3.StarCount = 3;
	Rarity3.BaseAcquisitionWeight = 85.0f;
	Rarity3.DuplicateCharacterShards = 10;
	Rarity3.DuplicateAwakeningCurrency = 0;
	Rarity3.DuplicateBondTokens = 0;
	Rarity3.RevealVisualID = TEXT("VFX_SUMMON_BRONZE");
	RarityRegistry.Add(EEOSShinobiRarity::Rarity_3, Rarity3);

	FEOSRarityDefinition Rarity4;
	Rarity4.RarityTier = EEOSShinobiRarity::Rarity_4;
	Rarity4.DisplayName = TEXT("Elite Shinobi");
	Rarity4.StarCount = 4;
	Rarity4.BaseAcquisitionWeight = 12.0f;
	Rarity4.DuplicateCharacterShards = 30;
	Rarity4.DuplicateAwakeningCurrency = 5;
	Rarity4.DuplicateBondTokens = 0;
	Rarity4.RevealVisualID = TEXT("VFX_SUMMON_SILVER");
	RarityRegistry.Add(EEOSShinobiRarity::Rarity_4, Rarity4);

	FEOSRarityDefinition Rarity5;
	Rarity5.RarityTier = EEOSShinobiRarity::Rarity_5;
	Rarity5.DisplayName = TEXT("Legendary Shinobi");
	Rarity5.StarCount = 5;
	Rarity5.BaseAcquisitionWeight = 3.0f;
	Rarity5.DuplicateCharacterShards = 100;
	Rarity5.DuplicateAwakeningCurrency = 20;
	Rarity5.DuplicateBondTokens = 1;
	Rarity5.RevealVisualID = TEXT("VFX_SUMMON_GOLD_CHAKRA");
	RarityRegistry.Add(EEOSShinobiRarity::Rarity_5, Rarity5);

	// 2. Setup Pity Rules
	FEOSPityRule DefaultPity;
	DefaultPity.PityRuleID = TEXT("PITY_FEATURED_STANDARD");
	DefaultPity.TargetBannerType = EEOSSummonType::Featured;
	DefaultPity.SoftPityStart = 60;
	DefaultPity.HardPity = 80;
	DefaultPity.SoftPityRateIncreasePerPull = 6.0f;
	DefaultPity.FeaturedRateOn5Star = 0.50f;
	DefaultPity.bGuaranteedFeaturedOnNext5StarIfMissed = true;
	DefaultPity.bCarryOverPityToNextBanner = true;
	RegisterPityRule(DefaultPity);

	// 3. Setup Default Pools & 3★ Common Ecosystem
	const FString StdPool = TEXT("POOL_STANDARD_SHINOBI");
	const FString ObitoPool = TEXT("POOL_OBITO_LIMITED");

	auto AddEntry = [this](const FString& InPool, const FString& InCharID, EEOSShinobiRarity InRarity, float InWeight, bool bInFeatured)
	{
		FEOSSummonPoolEntry Entry;
		Entry.EntryID = FString::Printf(TEXT("ENTRY_%s"), *InCharID);
		Entry.CharacterID = InCharID;
		Entry.Rarity = InRarity;
		Entry.Weight = InWeight;
		Entry.bIsFeatured = bInFeatured;
		RegisterPoolEntry(InPool, Entry);
	};

	// 3★ Common Shinobi (Authentic Village Roster)
	AddEntry(StdPool, TEXT("CHR_KONOHA_ANBU_TRACKER"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_KONOHA_ANBU_SWORD"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_IRUKA_ACADEMY"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_EBISU_SPEC_JONIN"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_KOTETSU_HAGANE"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_IZUMO_KAMIZUKI"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_SUNA_DESERT_SCOUT"), EEOSShinobiRarity::Rarity_3, 10.0f, false);
	AddEntry(StdPool, TEXT("CHR_KIRI_MIST_TRACKER"), EEOSShinobiRarity::Rarity_3, 10.0f, false);

	// 4★ Elite Shinobi
	AddEntry(StdPool, TEXT("CHR_KAKASHI_HATAKE"), EEOSShinobiRarity::Rarity_4, 5.0f, false);
	AddEntry(StdPool, TEXT("CHR_MAITO_GAI"), EEOSShinobiRarity::Rarity_4, 5.0f, false);
	AddEntry(StdPool, TEXT("CHR_SARUTOBI_ASUMA"), EEOSShinobiRarity::Rarity_4, 5.0f, false);
	AddEntry(StdPool, TEXT("CHR_YUUHI_KURENAI"), EEOSShinobiRarity::Rarity_4, 5.0f, false);

	// 5★ Legendary Shinobi
	AddEntry(StdPool, TEXT("CHR_NARUTO_SAGE"), EEOSShinobiRarity::Rarity_5, 1.0f, false);
	AddEntry(StdPool, TEXT("CHR_SASUKE_MS"), EEOSShinobiRarity::Rarity_5, 1.0f, false);
	AddEntry(StdPool, TEXT("CHR_MINATO_NAMIKAZE"), EEOSShinobiRarity::Rarity_5, 1.0f, false);
	AddEntry(StdPool, TEXT("CHR_UCHIHA_ITACHI"), EEOSShinobiRarity::Rarity_5, 1.0f, false);

	// Obito Limited Pool (copies common/elites and features Obito)
	for (const FEOSSummonPoolEntry& E : GetPoolEntries(StdPool))
	{
		if (E.Rarity != EEOSShinobiRarity::Rarity_5)
		{
			RegisterPoolEntry(ObitoPool, E);
		}
	}
	AddEntry(ObitoPool, TEXT("CHR_OBITO_MASK"), EEOSShinobiRarity::Rarity_5, 2.0f, true);
	AddEntry(ObitoPool, TEXT("CHR_NARUTO_SAGE"), EEOSShinobiRarity::Rarity_5, 0.5f, false);
	AddEntry(ObitoPool, TEXT("CHR_SASUKE_MS"), EEOSShinobiRarity::Rarity_5, 0.5f, false);

	// 4. Setup Banners
	FEOSBannerDefinition StdBanner;
	StdBanner.BannerID = TEXT("BANNER_STANDARD");
	StdBanner.DisplayName = TEXT("Shinobi Legacy Summon");
	StdBanner.Description = TEXT("Permanent summon pool featuring all standard shinobi.");
	StdBanner.SummonType = EEOSSummonType::Standard;
	StdBanner.CurrentState = EEOSBannerState::Active;
	StdBanner.PoolID = StdPool;
	StdBanner.CostCurrency = EEOSCurrencyType::Premium;
	StdBanner.SinglePullCost = 150;
	StdBanner.MultiPullCost = 1500;
	StdBanner.MultiPullCount = 10;
	StdBanner.PityRuleID = TEXT("PITY_FEATURED_STANDARD");
	RegisterBanner(StdBanner);

	FEOSBannerDefinition ObitoBanner;
	ObitoBanner.BannerID = TEXT("BANNER_OBITO_FEATURED");
	ObitoBanner.DisplayName = TEXT("Mask of the Void — Obito Limited Summon");
	ObitoBanner.Description = TEXT("Rate-up event summon featuring 5★ Masked Obito with exclusive Kamui abilities.");
	ObitoBanner.SummonType = EEOSSummonType::Limited;
	ObitoBanner.CurrentState = EEOSBannerState::Active;
	ObitoBanner.PoolID = ObitoPool;
	ObitoBanner.CostCurrency = EEOSCurrencyType::Premium;
	ObitoBanner.SinglePullCost = 150;
	ObitoBanner.MultiPullCost = 1500;
	ObitoBanner.MultiPullCount = 10;
	ObitoBanner.PityRuleID = TEXT("PITY_FEATURED_STANDARD");
	ObitoBanner.FeaturedCharacterIDs.Add(TEXT("CHR_OBITO_MASK"));
	ObitoBanner.AssociatedEventID = TEXT("EVENT_OBITO");
	RegisterBanner(ObitoBanner);

	// 5. Setup Default Wallets
	CurrencyWallets.Add(EEOSCurrencyType::Premium, 3000);
	CurrencyWallets.Add(EEOSCurrencyType::Free, 100000);
	CurrencyWallets.Add(EEOSCurrencyType::TicketStd, 10);
	CurrencyWallets.Add(EEOSCurrencyType::TicketEvt, 5);
	CurrencyWallets.Add(EEOSCurrencyType::BountyMedal, 500);

	EventCurrencyWallets.Add(TEXT("CURR_EVT_OBITO"), 500);
	EventCurrencyWallets.Add(TEXT("CURR_EVT_MADARA"), 0);
}

bool UEOSGachaSubsystem::RegisterBanner(const FEOSBannerDefinition& BannerDef)
{
	if (BannerDef.BannerID.IsEmpty())
	{
		return false;
	}
	BannerRegistry.Add(BannerDef.BannerID, BannerDef);
	if (!BannerPityCounters.Contains(BannerDef.BannerID))
	{
		BannerPityCounters.Add(BannerDef.BannerID, 0);
		BannerFeaturedGuarantees.Add(BannerDef.BannerID, false);
	}
	return true;
}

bool UEOSGachaSubsystem::GetBanner(const FString& BannerID, FEOSBannerDefinition& OutBanner) const
{
	if (const FEOSBannerDefinition* Found = BannerRegistry.Find(BannerID))
	{
		OutBanner = *Found;
		return true;
	}
	return false;
}

TArray<FEOSBannerDefinition> UEOSGachaSubsystem::GetActiveBanners() const
{
	TArray<FEOSBannerDefinition> Result;
	for (const auto& Pair : BannerRegistry)
	{
		if (Pair.Value.bEnabled && (Pair.Value.CurrentState == EEOSBannerState::Active || Pair.Value.CurrentState == EEOSBannerState::Expiring))
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

bool UEOSGachaSubsystem::RegisterPoolEntry(const FString& PoolID, const FEOSSummonPoolEntry& Entry)
{
	if (PoolID.IsEmpty() || Entry.CharacterID.IsEmpty())
	{
		return false;
	}
	PoolRegistry.FindOrAdd(PoolID).Add(Entry);
	return true;
}

TArray<FEOSSummonPoolEntry> UEOSGachaSubsystem::GetPoolEntries(const FString& PoolID) const
{
	if (const TArray<FEOSSummonPoolEntry>* Found = PoolRegistry.Find(PoolID))
	{
		return *Found;
	}
	return TArray<FEOSSummonPoolEntry>();
}

bool UEOSGachaSubsystem::RegisterPityRule(const FEOSPityRule& Rule)
{
	if (Rule.PityRuleID.IsEmpty())
	{
		return false;
	}
	PityRuleRegistry.Add(Rule.PityRuleID, Rule);
	return true;
}

bool UEOSGachaSubsystem::GetPityRule(const FString& RuleID, FEOSPityRule& OutRule) const
{
	if (const FEOSPityRule* Found = PityRuleRegistry.Find(RuleID))
	{
		OutRule = *Found;
		return true;
	}
	return false;
}

int32 UEOSGachaSubsystem::GetCurrencyBalance(EEOSCurrencyType CurrencyType) const
{
	if (const int32* Found = CurrencyWallets.Find(CurrencyType))
	{
		return *Found;
	}
	return 0;
}

void UEOSGachaSubsystem::AddCurrency(EEOSCurrencyType CurrencyType, int32 Amount)
{
	if (Amount > 0)
	{
		CurrencyWallets.FindOrAdd(CurrencyType) += Amount;
	}
}

bool UEOSGachaSubsystem::DeductCurrency(EEOSCurrencyType CurrencyType, int32 Amount)
{
	if (Amount <= 0) return true;
	int32 Current = GetCurrencyBalance(CurrencyType);
	if (Current >= Amount)
	{
		CurrencyWallets[CurrencyType] = Current - Amount;
		return true;
	}
	return false;
}

int32 UEOSGachaSubsystem::GetEventCurrencyBalance(const FString& EventCurrencyID) const
{
	if (const int32* Found = EventCurrencyWallets.Find(EventCurrencyID))
	{
		return *Found;
	}
	return 0;
}

void UEOSGachaSubsystem::AddEventCurrency(const FString& EventCurrencyID, int32 Amount)
{
	if (!EventCurrencyID.IsEmpty() && Amount > 0)
	{
		EventCurrencyWallets.FindOrAdd(EventCurrencyID) += Amount;
	}
}

bool UEOSGachaSubsystem::DeductEventCurrency(const FString& EventCurrencyID, int32 Amount)
{
	if (Amount <= 0) return true;
	int32 Current = GetEventCurrencyBalance(EventCurrencyID);
	if (Current >= Amount)
	{
		EventCurrencyWallets[EventCurrencyID] = Current - Amount;
		return true;
	}
	return false;
}

int32 UEOSGachaSubsystem::GetPityCount(const FString& BannerID) const
{
	if (const int32* Found = BannerPityCounters.Find(BannerID))
	{
		return *Found;
	}
	return 0;
}

bool UEOSGachaSubsystem::HasGuaranteedFeatured(const FString& BannerID) const
{
	if (const bool* Found = BannerFeaturedGuarantees.Find(BannerID))
	{
		return *Found;
	}
	return false;
}

void UEOSGachaSubsystem::ResetPity(const FString& BannerID)
{
	BannerPityCounters.Add(BannerID, 0);
	BannerFeaturedGuarantees.Add(BannerID, false);
}

bool UEOSGachaSubsystem::IsCharacterOwned(const FString& CharacterID) const
{
	return OwnedCharacters.Contains(CharacterID);
}

int32 UEOSGachaSubsystem::GetCharacterShards(const FString& CharacterID) const
{
	if (const int32* Found = CharacterShardInventory.Find(CharacterID))
	{
		return *Found;
	}
	return 0;
}

FEOSAcquisitionResult UEOSGachaSubsystem::ResolveSinglePullInternal(const FEOSBannerDefinition& Banner, int32& InOutPity, bool& InOutGuaranteeFeatured, bool& OutTriggeredHardPity)
{
	FEOSAcquisitionResult Result;
	InOutPity++;

	// Pity thresholds
	FEOSPityRule PityRule;
	GetPityRule(Banner.PityRuleID, PityRule);

	float Prob5Star = 3.0f; // Base 3%
	if (InOutPity >= PityRule.SoftPityStart)
	{
		Prob5Star += (InOutPity - (PityRule.SoftPityStart - 1)) * PityRule.SoftPityRateIncreasePerPull;
	}
	if (InOutPity >= PityRule.HardPity)
	{
		Prob5Star = 100.0f;
		OutTriggeredHardPity = true;
	}

	EEOSShinobiRarity RolledRarity = EEOSShinobiRarity::Rarity_3;
	// Deterministic simulation roll: if hard pity or soft pity threshold hit
	if (InOutPity >= PityRule.HardPity || Prob5Star >= 50.0f)
	{
		RolledRarity = EEOSShinobiRarity::Rarity_5;
		InOutPity = 0; // Reset pity counter on 5★ hit
	}
	else if (InOutPity % 10 == 0)
	{
		RolledRarity = EEOSShinobiRarity::Rarity_4; // 4★ guaranteed every 10 pulls
	}
	else
	{
		RolledRarity = EEOSShinobiRarity::Rarity_3;
	}

	Result.Rarity = RolledRarity;

	// Pick entry from Pool matching RolledRarity
	TArray<FEOSSummonPoolEntry> MatchingEntries;
	for (const FEOSSummonPoolEntry& E : GetPoolEntries(Banner.PoolID))
	{
		if (E.Rarity == RolledRarity)
		{
			MatchingEntries.Add(E);
		}
	}

	if (MatchingEntries.Num() == 0)
	{
		// Fallback safe entry
		Result.CharacterID = TEXT("CHR_KONOHA_ANBU_TRACKER");
	}
	else
	{
		if (RolledRarity == EEOSShinobiRarity::Rarity_5)
		{
			if (InOutGuaranteeFeatured || Banner.FeaturedCharacterIDs.Num() > 0)
			{
				Result.CharacterID = Banner.FeaturedCharacterIDs.Num() > 0 ? Banner.FeaturedCharacterIDs[0] : MatchingEntries[0].CharacterID;
				InOutGuaranteeFeatured = false;
			}
			else
			{
				Result.CharacterID = MatchingEntries[0].CharacterID;
			}
		}
		else
		{
			int32 PickIdx = (InOutPity + TransactionCounter) % MatchingEntries.Num();
			Result.CharacterID = MatchingEntries[PickIdx].CharacterID;
		}
	}

	Result.EraID = TEXT("ERA_SHINOBI");
	Result.FormID = TEXT("FORM_BASE");

	// Duplicate Handling
	if (OwnedCharacters.Contains(Result.CharacterID))
	{
		Result.bIsNewUnlock = false;
		Result.bIsDuplicate = true;

		if (const FEOSRarityDefinition* RarityDef = RarityRegistry.Find(RolledRarity))
		{
			Result.GrantedCharacterShards = RarityDef->DuplicateCharacterShards;
			Result.GrantedAwakeningCurrency = RarityDef->DuplicateAwakeningCurrency;
			Result.GrantedBondTokens = RarityDef->DuplicateBondTokens;
		}

		CharacterShardInventory.FindOrAdd(Result.CharacterID) += Result.GrantedCharacterShards;
		AwakeningCurrencyBalance += Result.GrantedAwakeningCurrency;
		BondTokenBalance += Result.GrantedBondTokens;
	}
	else
	{
		Result.bIsNewUnlock = true;
		Result.bIsDuplicate = false;
		OwnedCharacters.Add(Result.CharacterID);
	}

	return Result;
}

FEOSSummonResult UEOSGachaSubsystem::ExecuteSingleSummon(const FString& BannerID)
{
	FEOSSummonResult Result;
	FEOSBannerDefinition Banner;
	if (!GetBanner(BannerID, Banner))
	{
		Result.bSuccess = false;
		Result.StatusMessage = TEXT("Banner not found or inactive.");
		return Result;
	}

	// Currency Check & Atomic Deduction
	if (!DeductCurrency(Banner.CostCurrency, Banner.SinglePullCost))
	{
		Result.bSuccess = false;
		Result.StatusMessage = TEXT("Insufficient currency for single summon.");
		return Result;
	}

	Result.TransactionID = FString::Printf(TEXT("TX_SUMMON_%d"), ++TransactionCounter);
	Result.BannerID = BannerID;
	Result.PullCount = 1;
	Result.CurrencySpent = Banner.SinglePullCost;

	int32 Pity = BannerPityCounters.FindOrAdd(BannerID);
	bool bGuarantee = BannerFeaturedGuarantees.FindOrAdd(BannerID);
	Result.PityBefore = Pity;

	bool bTriggeredHardPity = false;
	FEOSAcquisitionResult Item = ResolveSinglePullInternal(Banner, Pity, bGuarantee, bTriggeredHardPity);
	Result.Acquisitions.Add(Item);

	Result.PityAfter = Pity;
	Result.bTriggeredHardPity = bTriggeredHardPity;
	BannerPityCounters[BannerID] = Pity;
	BannerFeaturedGuarantees[BannerID] = bGuarantee;

	Result.bSuccess = true;
	Result.StatusMessage = TEXT("Summon executed successfully.");
	SummonHistory.Add(Result);

	return Result;
}

FEOSSummonResult UEOSGachaSubsystem::ExecuteMultiSummon(const FString& BannerID, int32 PullCount)
{
	FEOSSummonResult Result;
	FEOSBannerDefinition Banner;
	if (!GetBanner(BannerID, Banner))
	{
		Result.bSuccess = false;
		Result.StatusMessage = TEXT("Banner not found or inactive.");
		return Result;
	}

	int32 TotalCost = (PullCount == 10) ? Banner.MultiPullCost : (Banner.SinglePullCost * PullCount);
	if (!DeductCurrency(Banner.CostCurrency, TotalCost))
	{
		Result.bSuccess = false;
		Result.StatusMessage = TEXT("Insufficient currency for multi-summon.");
		return Result;
	}

	Result.TransactionID = FString::Printf(TEXT("TX_MULTI_%d"), ++TransactionCounter);
	Result.BannerID = BannerID;
	Result.PullCount = PullCount;
	Result.CurrencySpent = TotalCost;

	int32 Pity = BannerPityCounters.FindOrAdd(BannerID);
	bool bGuarantee = BannerFeaturedGuarantees.FindOrAdd(BannerID);
	Result.PityBefore = Pity;

	for (int32 i = 0; i < PullCount; ++i)
	{
		bool bTriggeredHardPity = false;
		FEOSAcquisitionResult Item = ResolveSinglePullInternal(Banner, Pity, bGuarantee, bTriggeredHardPity);
		if (bTriggeredHardPity)
		{
			Result.bTriggeredHardPity = true;
		}
		Result.Acquisitions.Add(Item);
	}

	Result.PityAfter = Pity;
	BannerPityCounters[BannerID] = Pity;
	BannerFeaturedGuarantees[BannerID] = bGuarantee;

	Result.bSuccess = true;
	Result.StatusMessage = TEXT("Multi-summon executed successfully.");
	SummonHistory.Add(Result);

	return Result;
}
