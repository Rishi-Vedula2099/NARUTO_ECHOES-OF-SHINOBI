#include "../../Public/Subsystems/EOSLiveOpsSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSLiveOpsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultLiveOpsDatabase();
}

void UEOSLiveOpsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSLiveOpsSubsystem::InitializeDefaultLiveOpsDatabase()
{
	// 1. Configure Weekly Bounty Baseline (Configurable Thursday 00:00 UTC)
	BountyScheduleConfig.ResetDay = TEXT("Thursday");
	BountyScheduleConfig.ResetTime = TEXT("00:00:00");
	BountyScheduleConfig.Timezone = TEXT("UTC");
	BountyScheduleConfig.ActiveBountyCount = 3;
	BountyScheduleConfig.bAllowCrossModeAttachment = true;

	// 2. Setup 4 Planned Major Event Packages
	FEOSLimitedEventDefinition ObitoEvent;
	ObitoEvent.EventID = TEXT("EVENT_OBITO");
	ObitoEvent.DisplayName = TEXT("Mask of the Void — Obito's Spatial Incursion");
	ObitoEvent.Description = TEXT("Kamui spatial rifts disturb the Land of Fire. Investigate the masked threat and enter the Kamui Void.");
	ObitoEvent.EventType = EEOSEventType::StoryEvent;
	ObitoEvent.CurrentPhase = EEOSEventPhase::Phase1_Threat;
	ObitoEvent.AssociatedBannerID = TEXT("BANNER_OBITO_FEATURED");
	ObitoEvent.EventCurrencyID = TEXT("CURR_EVT_OBITO");
	ObitoEvent.EventBossID = TEXT("BOSS_OBITO_MASK");
	ObitoEvent.MotionMangaID = TEXT("MM_OBITO_BEHIND_THE_MASK");
	ObitoEvent.WorldStateDisturbanceID = TEXT("WORLD_KAMUI_DISTURBANCE");
	ObitoEvent.EventShopID = TEXT("SHOP_OBITO_KAMUI");
	ObitoEvent.StartTimeUTC = TEXT("2026-09-01T00:00:00Z");
	ObitoEvent.EndTimeUTC = TEXT("2026-09-25T00:00:00Z");
	ObitoEvent.ClaimEndTimeUTC = TEXT("2026-10-02T00:00:00Z");
	ObitoEvent.bIsActive = true;

	// Populate Obito Shop Items
	FEOSShopItemDefinition Item1;
	Item1.ItemID = TEXT("ITEM_OBITO_SHARDS");
	Item1.DisplayName = TEXT("Obito Character Fragments");
	Item1.RewardType = TEXT("CharacterShards");
	Item1.Quantity = 10;
	Item1.Cost = 150;
	Item1.PurchaseLimit = 10;
	Item1.CurrentPurchases = 0;
	ObitoEvent.ShopItems.Add(Item1);

	FEOSShopItemDefinition Item2;
	Item2.ItemID = TEXT("ITEM_SUMMON_TICKET");
	Item2.DisplayName = TEXT("Limited Event Summon Ticket");
	Item2.RewardType = TEXT("SummonTicket");
	Item2.Quantity = 1;
	Item2.Cost = 100;
	Item2.PurchaseLimit = 5;
	Item2.CurrentPurchases = 0;
	ObitoEvent.ShopItems.Add(Item2);

	FEOSShopItemDefinition Item3;
	Item3.ItemID = TEXT("ITEM_KAMUI_ESSENCE");
	Item3.DisplayName = TEXT("Kamui Awakening Essence");
	Item3.RewardType = TEXT("AwakeningMaterial");
	Item3.Quantity = 5;
	Item3.Cost = 80;
	Item3.PurchaseLimit = 10;
	Item3.CurrentPurchases = 0;
	ObitoEvent.ShopItems.Add(Item3);

	RegisterLimitedEvent(ObitoEvent);

	// Juubi Madara Event
	FEOSLimitedEventDefinition MadaraEvent;
	MadaraEvent.EventID = TEXT("EVENT_JUUBI_MADARA");
	MadaraEvent.DisplayName = TEXT("The Celestial Awakening — Juubi Madara Calamity");
	MadaraEvent.Description = TEXT("Ten-Tails descent upon the Allied Shinobi battlefield. Encounter Limbo clone mechanics.");
	MadaraEvent.EventType = EEOSEventType::BossEvent;
	MadaraEvent.CurrentPhase = EEOSEventPhase::Phase1_Threat;
	MadaraEvent.AssociatedBannerID = TEXT("BANNER_MADARA_JUUBI");
	MadaraEvent.EventCurrencyID = TEXT("CURR_EVT_MADARA");
	MadaraEvent.EventBossID = TEXT("BOSS_JUUBI_MADARA");
	MadaraEvent.MotionMangaID = TEXT("MM_MADARA_CELESTIAL");
	MadaraEvent.WorldStateDisturbanceID = TEXT("WORLD_GOD_TREE_CALAMITY");
	MadaraEvent.EventShopID = TEXT("SHOP_MADARA_JUUBI");
	MadaraEvent.bIsActive = false; // Scheduled
	RegisterLimitedEvent(MadaraEvent);

	// Indra Event
	FEOSLimitedEventDefinition IndraEvent;
	IndraEvent.EventID = TEXT("EVENT_INDRA");
	IndraEvent.DisplayName = TEXT("The Pure Spark of Lightning — Indra Ōtsutsuki Trials");
	IndraEvent.Description = TEXT("Precision ocular mastery and lightning style Susanoo combat challenges.");
	IndraEvent.EventType = EEOSEventType::CharacterEvent;
	IndraEvent.CurrentPhase = EEOSEventPhase::Phase1_Threat;
	IndraEvent.EventCurrencyID = TEXT("CURR_EVT_INDRA");
	IndraEvent.EventBossID = TEXT("BOSS_INDRA_OTSUTSUKI");
	IndraEvent.bIsActive = false;
	RegisterLimitedEvent(IndraEvent);

	// Ashura Event
	FEOSLimitedEventDefinition AshuraEvent;
	AshuraEvent.EventID = TEXT("EVENT_ASHURA");
	AshuraEvent.DisplayName = TEXT("The Inherited Will of Fire — Ashura Ōtsutsuki Trials");
	AshuraEvent.Description = TEXT("Six Paths team synergy and supportive chakra mastery gauntlet.");
	AshuraEvent.EventType = EEOSEventType::CharacterEvent;
	AshuraEvent.CurrentPhase = EEOSEventPhase::Phase1_Threat;
	AshuraEvent.EventCurrencyID = TEXT("CURR_EVT_ASHURA");
	AshuraEvent.EventBossID = TEXT("BOSS_ASHURA_OTSUTSUKI");
	AshuraEvent.bIsActive = false;
	RegisterLimitedEvent(AshuraEvent);

	// 3. Generate Baseline Weekly Bounties (Seed: 20260901)
	GenerateWeeklyBounties(20260901);
}

bool UEOSLiveOpsSubsystem::RegisterLimitedEvent(const FEOSLimitedEventDefinition& EventDef)
{
	if (EventDef.EventID.IsEmpty())
	{
		return false;
	}
	EventRegistry.Add(EventDef.EventID, EventDef);
	return true;
}

bool UEOSLiveOpsSubsystem::GetLimitedEvent(const FString& EventID, FEOSLimitedEventDefinition& OutEvent) const
{
	if (const FEOSLimitedEventDefinition* Found = EventRegistry.Find(EventID))
	{
		OutEvent = *Found;
		return true;
	}
	return false;
}

TArray<FEOSLimitedEventDefinition> UEOSLiveOpsSubsystem::GetActiveEvents() const
{
	TArray<FEOSLimitedEventDefinition> Result;
	for (const auto& Pair : EventRegistry)
	{
		if (Pair.Value.bIsActive)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

bool UEOSLiveOpsSubsystem::AdvanceEventPhase(const FString& EventID)
{
	if (FEOSLimitedEventDefinition* Found = EventRegistry.Find(EventID))
	{
		switch (Found->CurrentPhase)
		{
		case EEOSEventPhase::Phase1_Threat:
			Found->CurrentPhase = EEOSEventPhase::Phase2_Escalation;
			return true;
		case EEOSEventPhase::Phase2_Escalation:
			Found->CurrentPhase = EEOSEventPhase::Phase3_Revelation;
			return true;
		case EEOSEventPhase::Phase3_Revelation:
			Found->CurrentPhase = EEOSEventPhase::Phase4_FinalBattle;
			return true;
		case EEOSEventPhase::Phase4_FinalBattle:
			return false; // Already at climax
		}
	}
	return false;
}

bool UEOSLiveOpsSubsystem::SetEventActiveState(const FString& EventID, bool bIsActive)
{
	if (FEOSLimitedEventDefinition* Found = EventRegistry.Find(EventID))
	{
		Found->bIsActive = bIsActive;
		return true;
	}
	return false;
}

bool UEOSLiveOpsSubsystem::PurchaseShopItem(const FString& EventID, const FString& ItemID, int32& OutGrantedQty, FString& OutStatusMessage)
{
	FEOSLimitedEventDefinition* FoundEvent = EventRegistry.Find(EventID);
	if (!FoundEvent)
	{
		OutStatusMessage = TEXT("Event not found.");
		return false;
	}

	for (FEOSShopItemDefinition& Item : FoundEvent->ShopItems)
	{
		if (Item.ItemID == ItemID)
		{
			if (Item.CurrentPurchases >= Item.PurchaseLimit)
			{
				OutStatusMessage = TEXT("Purchase limit reached for this item.");
				return false;
			}

			Item.CurrentPurchases++;
			OutGrantedQty = Item.Quantity;
			OutStatusMessage = FString::Printf(TEXT("Successfully purchased %s (x%d)."), *Item.DisplayName, Item.Quantity);
			return true;
		}
	}

	OutStatusMessage = TEXT("Item not found in event shop.");
	return false;
}

TArray<FEOSShopItemDefinition> UEOSLiveOpsSubsystem::GetEventShopItems(const FString& EventID) const
{
	if (const FEOSLimitedEventDefinition* Found = EventRegistry.Find(EventID))
	{
		return Found->ShopItems;
	}
	return TArray<FEOSShopItemDefinition>();
}

void UEOSLiveOpsSubsystem::ConfigureBountySchedule(const FString& NewResetDay, const FString& NewResetTime, const FString& NewTimezone)
{
	if (!NewResetDay.IsEmpty()) BountyScheduleConfig.ResetDay = NewResetDay;
	if (!NewResetTime.IsEmpty()) BountyScheduleConfig.ResetTime = NewResetTime;
	if (!NewTimezone.IsEmpty()) BountyScheduleConfig.Timezone = NewTimezone;
}

void UEOSLiveOpsSubsystem::GenerateWeeklyBounties(int32 Seed)
{
	ActiveWeeklyBounties.Empty();

	// Bounty 1: Zabuza Momochi (Mist Ambush)
	FEOSWeeklyBountyRecord B1;
	B1.BountyID = TEXT("BOUNTY_ZABUZA_MIST");
	B1.TargetBossName = TEXT("Zabuza Momochi");
	B1.TargetType = EEOSBountyTargetType::Boss;
	B1.LocationModeID = TEXT("MODE_STORY_CAMPAIGN");
	B1.ObjectiveDescription = TEXT("Execute 8 Perfect Dodges during mist ambush");
	B1.RequiredCount = 8;
	B1.CurrentProgress = 0;
	B1.ModifierDescription = TEXT("Mist Density 80% / Enemy Movement Speed +25%");
	B1.BountyMedalReward = 250;
	B1.RyoReward = 50000;
	B1.SpecialMaterialDrop = TEXT("MAT_WATER_MIST_ESSENCE");
	B1.bIsCompleted = false;
	B1.bRewardClaimed = false;
	ActiveWeeklyBounties.Add(B1.BountyID, B1);

	// Bounty 2: Hidan (Jashin Ritual)
	FEOSWeeklyBountyRecord B2;
	B2.BountyID = TEXT("BOUNTY_HIDAN_RITUAL");
	B2.TargetBossName = TEXT("Hidan");
	B2.TargetType = EEOSBountyTargetType::Boss;
	B2.LocationModeID = TEXT("MODE_SURVIVAL_WAVES");
	B2.ObjectiveDescription = TEXT("Break blood ritual circle within 30 seconds");
	B2.RequiredCount = 1;
	B2.CurrentProgress = 0;
	B2.ModifierDescription = TEXT("No Healing Potions / Bleed Status Tick 2% HP/s");
	B2.BountyMedalReward = 250;
	B2.RyoReward = 50000;
	B2.SpecialMaterialDrop = TEXT("MAT_CURSE_BLOOD_VIAL");
	B2.bIsCompleted = false;
	B2.bRewardClaimed = false;
	ActiveWeeklyBounties.Add(B2.BountyID, B2);

	// Bounty 3: Deidara (Airborne Detonation)
	FEOSWeeklyBountyRecord B3;
	B3.BountyID = TEXT("BOUNTY_DEIDARA_EXPLOSIVE");
	B3.TargetBossName = TEXT("Deidara");
	B3.TargetType = EEOSBountyTargetType::Boss;
	B3.LocationModeID = TEXT("MODE_SHINOBI_TOWER");
	B3.ObjectiveDescription = TEXT("Clear floor without taking ground mine explosion damage");
	B3.RequiredCount = 1;
	B3.CurrentProgress = 0;
	B3.ModifierDescription = TEXT("Airborne Threat / Micro-Clay Detonation Traps");
	B3.BountyMedalReward = 250;
	B3.RyoReward = 50000;
	B3.SpecialMaterialDrop = TEXT("MAT_EXPLOSIVE_CLAY_POWDER");
	B3.bIsCompleted = false;
	B3.bRewardClaimed = false;
	ActiveWeeklyBounties.Add(B3.BountyID, B3);
}

bool UEOSLiveOpsSubsystem::GetActiveBounty(const FString& BountyID, FEOSWeeklyBountyRecord& OutBounty) const
{
	if (const FEOSWeeklyBountyRecord* Found = ActiveWeeklyBounties.Find(BountyID))
	{
		OutBounty = *Found;
		return true;
	}
	return false;
}

TArray<FEOSWeeklyBountyRecord> UEOSLiveOpsSubsystem::GetActiveBounties() const
{
	TArray<FEOSWeeklyBountyRecord> Result;
	for (const auto& Pair : ActiveWeeklyBounties)
	{
		Result.Add(Pair.Value);
	}
	return Result;
}

bool UEOSLiveOpsSubsystem::UpdateBountyProgress(const FString& BountyID, int32 Increment)
{
	if (FEOSWeeklyBountyRecord* Found = ActiveWeeklyBounties.Find(BountyID))
	{
		if (Found->bIsCompleted) return false;

		Found->CurrentProgress += Increment;
		if (Found->CurrentProgress >= Found->RequiredCount)
		{
			Found->CurrentProgress = Found->RequiredCount;
			Found->bIsCompleted = true;
		}
		return true;
	}
	return false;
}

bool UEOSLiveOpsSubsystem::ClaimBountyReward(const FString& BountyID, int32& OutMedals, int32& OutRyo, FString& OutMaterial)
{
	if (FEOSWeeklyBountyRecord* Found = ActiveWeeklyBounties.Find(BountyID))
	{
		if (Found->bIsCompleted && !Found->bRewardClaimed)
		{
			Found->bRewardClaimed = true;
			OutMedals = Found->BountyMedalReward;
			OutRyo = Found->RyoReward;
			OutMaterial = Found->SpecialMaterialDrop;
			BountyCompletionHistory.FindOrAdd(BountyID)++;
			return true;
		}
	}
	return false;
}

FString UEOSLiveOpsSubsystem::GetActiveWorldStateDisturbance() const
{
	for (const auto& Pair : EventRegistry)
	{
		if (Pair.Value.bIsActive && !Pair.Value.WorldStateDisturbanceID.IsEmpty())
		{
			return Pair.Value.WorldStateDisturbanceID;
		}
	}
	return TEXT("WORLD_NORMAL");
}

FString UEOSLiveOpsSubsystem::GetActiveEventMotionManga(const FString& EventID) const
{
	if (const FEOSLimitedEventDefinition* Found = EventRegistry.Find(EventID))
	{
		return Found->MotionMangaID;
	}
	return FString();
}
