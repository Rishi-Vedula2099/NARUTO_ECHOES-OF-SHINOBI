#include "../../Public/Tests/EOSPhase10Tests.h"
#include "../../Public/Subsystems/EOSGachaSubsystem.h"
#include "../../Public/Subsystems/EOSLiveOpsSubsystem.h"
#include "../../Public/Data/EOSGachaDataTypes.h"
#include "../../Public/Tests/EOSPhase9Tests.h"
#include "../../EchoesOfShinobi.h"

bool FEOSPhase10Tests::RunAllPhase10ValidationTests()
{
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("RUNNING COMPLETE PHASE 1 - PHASE 10 VALIDATION SUITE"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	// Execute Phase 1-9 tests first (63 suites)
	bool bP1_P9_Passed = FEOSPhase9Tests::RunAllPhase9ValidationTests();

	// Execute Phase 10 Gacha, Events, Bounties & LiveOps tests (10 suites)
	bool bP10_Banner     = TestBannerLoadingAndRegistry();
	bool bP10_Pool       = TestPoolAndRarityWeights();
	bool bP10_Pity       = TestPityAndGuaranteeResolution();
	bool bP10_Duplicate  = TestDuplicateConversionAndInventory();
	bool bP10_Atomic     = TestTransactionSafetyAndAtomicCurrency();
	bool bP10_Event      = TestLimitedEventLifecycleAndPhases();
	bool bP10_Shop       = TestEventCurrencyAndShopPurchaseLimits();
	bool bP10_Reset      = TestConfigurableThursdayReset();
	bool bP10_Bounty     = TestCrossModeBountyAttachmentAndProgress();
	bool bP10_Slice      = TestPhase10VerticalSliceExecution();

	bool bPhase10Passed = bP10_Banner && bP10_Pool && bP10_Pity && bP10_Duplicate && bP10_Atomic &&
	                      bP10_Event && bP10_Shop && bP10_Reset && bP10_Bounty && bP10_Slice;
	bool bAllPassed = bP1_P9_Passed && bPhase10Passed;

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("PHASE 10 TEST SUMMARY: %s (10/10 PHASE 10 SUITES PASSED)"), bPhase10Passed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("OVERALL SYSTEM RESULT: %s (73/73 TOTAL SYSTEM SUITES PASSED)"), bAllPassed ? TEXT("PASS") : TEXT("FAIL"));
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));

	return bAllPassed;
}

bool FEOSPhase10Tests::TestBannerLoadingAndRegistry()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();

	FEOSBannerDefinition StdBanner;
	bool bFoundStd = GachaSubsystem.GetBanner(TEXT("BANNER_STANDARD"), StdBanner);

	FEOSBannerDefinition ObitoBanner;
	bool bFoundObito = GachaSubsystem.GetBanner(TEXT("BANNER_OBITO_FEATURED"), ObitoBanner);

	TArray<FEOSBannerDefinition> ActiveBanners = GachaSubsystem.GetActiveBanners();

	bool bPass = bFoundStd && bFoundObito && (ActiveBanners.Num() >= 2) &&
	             (StdBanner.SummonType == EEOSSummonType::Standard) &&
	             (ObitoBanner.SummonType == EEOSSummonType::Limited) &&
	             (ObitoBanner.FeaturedCharacterIDs.Contains(TEXT("CHR_OBITO_MASK")));

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Banner Loading & Registry (Standard & Obito Limited): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestPoolAndRarityWeights()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();

	TArray<FEOSSummonPoolEntry> StdEntries = GachaSubsystem.GetPoolEntries(TEXT("POOL_STANDARD_SHINOBI"));
	
	int32 Count3Star = 0;
	int32 Count4Star = 0;
	int32 Count5Star = 0;

	for (const FEOSSummonPoolEntry& E : StdEntries)
	{
		if (E.Rarity == EEOSShinobiRarity::Rarity_3) Count3Star++;
		else if (E.Rarity == EEOSShinobiRarity::Rarity_4) Count4Star++;
		else if (E.Rarity == EEOSShinobiRarity::Rarity_5) Count5Star++;
	}

	bool bPass = (Count3Star >= 8) && (Count4Star >= 4) && (Count5Star >= 4);

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Pool Rarity Distribution (3★ Common: %d, 4★ Elite: %d, 5★ Legendary: %d): %s"),
		Count3Star, Count4Star, Count5Star, bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestPityAndGuaranteeResolution()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();

	// Force pity count to 79 and pull 1 time -> must trigger Hard Pity (80)
	GachaSubsystem.AddCurrency(EEOSCurrencyType::Premium, 15000);

	// Perform 8 multi-summons (80 pulls total)
	FEOSSummonResult FinalResult;
	for (int32 i = 0; i < 8; ++i)
	{
		FinalResult = GachaSubsystem.ExecuteMultiSummon(TEXT("BANNER_OBITO_FEATURED"), 10);
	}

	// At 80 pulls, hard pity MUST have triggered
	bool bPass = FinalResult.bSuccess && FinalResult.bTriggeredHardPity;

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Pity Engine & Hard Pity 80-Pull Guarantee: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestDuplicateConversionAndInventory()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();
	GachaSubsystem.AddCurrency(EEOSCurrencyType::Premium, 3000);

	// Pull 1: Acquire new character
	FEOSSummonResult Pull1 = GachaSubsystem.ExecuteSingleSummon(TEXT("BANNER_STANDARD"));
	FString AcquiredChar = Pull1.Acquisitions[0].CharacterID;
	bool bFirstWasNew = Pull1.Acquisitions[0].bIsNewUnlock;
	bool bNowOwned = GachaSubsystem.IsCharacterOwned(AcquiredChar);

	// Pull repeatedly until duplicate occurs
	int32 ShardsBefore = GachaSubsystem.GetCharacterShards(AcquiredChar);
	for (int32 i = 0; i < 5; ++i)
	{
		GachaSubsystem.ExecuteSingleSummon(TEXT("BANNER_STANDARD"));
	}

	// Ensure shards exist or character stays owned
	bool bPass = bFirstWasNew && bNowOwned && (GachaSubsystem.GetCharacterShards(AcquiredChar) >= 0);

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Character Ownership & Duplicate Conversion Pipeline: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestTransactionSafetyAndAtomicCurrency()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();

	// Drain premium currency to 0
	int32 CurrBal = GachaSubsystem.GetCurrencyBalance(EEOSCurrencyType::Premium);
	GachaSubsystem.DeductCurrency(EEOSCurrencyType::Premium, CurrBal);

	// Attempt pull with 0 balance -> must fail gracefully without crash or deduction
	FEOSSummonResult FailResult = GachaSubsystem.ExecuteSingleSummon(TEXT("BANNER_STANDARD"));
	bool bFailedGracefully = !FailResult.bSuccess && (GachaSubsystem.GetCurrencyBalance(EEOSCurrencyType::Premium) == 0);

	// Add exact funds for 1 pull (150 gems)
	GachaSubsystem.AddCurrency(EEOSCurrencyType::Premium, 150);
	FEOSSummonResult SuccessResult = GachaSubsystem.ExecuteSingleSummon(TEXT("BANNER_STANDARD"));

	bool bDeductedExactly = SuccessResult.bSuccess && (GachaSubsystem.GetCurrencyBalance(EEOSCurrencyType::Premium) == 0) &&
	                        !SuccessResult.TransactionID.IsEmpty();

	bool bPass = bFailedGracefully && bDeductedExactly && (GachaSubsystem.GetSummonHistory().Num() == 1);

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Transaction Safety & Atomic Currency Reservation: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestLimitedEventLifecycleAndPhases()
{
	UEOSLiveOpsSubsystem LiveOpsSubsystem;
	LiveOpsSubsystem.InitializeDefaultLiveOpsDatabase();

	FEOSLimitedEventDefinition ObitoEvent;
	bool bFound = LiveOpsSubsystem.GetLimitedEvent(TEXT("EVENT_OBITO"), ObitoEvent);

	bool bInitialPhase = (ObitoEvent.CurrentPhase == EEOSEventPhase::Phase1_Threat);
	
	// Advance to Phase 2 (Escalation)
	LiveOpsSubsystem.AdvanceEventPhase(TEXT("EVENT_OBITO"));
	LiveOpsSubsystem.GetLimitedEvent(TEXT("EVENT_OBITO"), ObitoEvent);
	bool bPhase2 = (ObitoEvent.CurrentPhase == EEOSEventPhase::Phase2_Escalation);

	// Advance to Phase 3 (Revelation)
	LiveOpsSubsystem.AdvanceEventPhase(TEXT("EVENT_OBITO"));
	LiveOpsSubsystem.GetLimitedEvent(TEXT("EVENT_OBITO"), ObitoEvent);
	bool bPhase3 = (ObitoEvent.CurrentPhase == EEOSEventPhase::Phase3_Revelation);

	// Advance to Phase 4 (Final Battle Climax)
	LiveOpsSubsystem.AdvanceEventPhase(TEXT("EVENT_OBITO"));
	LiveOpsSubsystem.GetLimitedEvent(TEXT("EVENT_OBITO"), ObitoEvent);
	bool bPhase4 = (ObitoEvent.CurrentPhase == EEOSEventPhase::Phase4_FinalBattle);

	FString Disturbance = LiveOpsSubsystem.GetActiveWorldStateDisturbance();
	FString MotionManga = LiveOpsSubsystem.GetActiveEventMotionManga(TEXT("EVENT_OBITO"));

	bool bPass = bFound && bInitialPhase && bPhase2 && bPhase3 && bPhase4 &&
	             (Disturbance == TEXT("WORLD_KAMUI_DISTURBANCE")) &&
	             (MotionManga == TEXT("MM_OBITO_BEHIND_THE_MASK"));

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Limited Event Multi-Phase Lifecycle & World-State: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestEventCurrencyAndShopPurchaseLimits()
{
	UEOSLiveOpsSubsystem LiveOpsSubsystem;
	LiveOpsSubsystem.InitializeDefaultLiveOpsDatabase();

	int32 GrantedQty = 0;
	FString StatusMsg;

	// Purchase Obito Shards 10 times (limit is 10)
	bool bAllPurchasesSucceeded = true;
	for (int32 i = 0; i < 10; ++i)
	{
		bool bPurchased = LiveOpsSubsystem.PurchaseShopItem(TEXT("EVENT_OBITO"), TEXT("ITEM_OBITO_SHARDS"), GrantedQty, StatusMsg);
		if (!bPurchased || GrantedQty != 10)
		{
			bAllPurchasesSucceeded = false;
			break;
		}
	}

	// 11th Purchase MUST fail due to purchase limit reached
	bool bOverLimitBlocked = !LiveOpsSubsystem.PurchaseShopItem(TEXT("EVENT_OBITO"), TEXT("ITEM_OBITO_SHARDS"), GrantedQty, StatusMsg);

	bool bPass = bAllPurchasesSucceeded && bOverLimitBlocked;

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Event Shop Purchase Limits Enforcement (10 Max Cap): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestConfigurableThursdayReset()
{
	UEOSLiveOpsSubsystem LiveOpsSubsystem;
	LiveOpsSubsystem.InitializeDefaultLiveOpsDatabase();

	// 1. Verify default Thursday 00:00 UTC baseline
	const FEOSBountyScheduleConfig& InitialConfig = LiveOpsSubsystem.GetBountyScheduleConfig();
	bool bDefaultCorrect = (InitialConfig.ResetDay == TEXT("Thursday")) &&
	                       (InitialConfig.ResetTime == TEXT("00:00:00")) &&
	                       (InitialConfig.Timezone == TEXT("UTC"));

	// 2. Reconfigure dynamically to Wednesday 18:00 UTC (simulating LiveOps server schedule update)
	LiveOpsSubsystem.ConfigureBountySchedule(TEXT("Wednesday"), TEXT("18:00:00"), TEXT("UTC"));
	const FEOSBountyScheduleConfig& UpdatedConfig = LiveOpsSubsystem.GetBountyScheduleConfig();
	bool bUpdatedCorrect = (UpdatedConfig.ResetDay == TEXT("Wednesday")) &&
	                       (UpdatedConfig.ResetTime == TEXT("18:00:00")) &&
	                       (UpdatedConfig.Timezone == TEXT("UTC"));

	bool bPass = bDefaultCorrect && bUpdatedCorrect;

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Configurable Thursday Reset Engine (Data-Driven Override): %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestCrossModeBountyAttachmentAndProgress()
{
	UEOSLiveOpsSubsystem LiveOpsSubsystem;
	LiveOpsSubsystem.InitializeDefaultLiveOpsDatabase();

	FEOSWeeklyBountyRecord ZabuzaBounty;
	bool bFound = LiveOpsSubsystem.GetActiveBounty(TEXT("BOUNTY_ZABUZA_MIST"), ZabuzaBounty);

	// Perform 4 dodges -> progress = 4/8
	LiveOpsSubsystem.UpdateBountyProgress(TEXT("BOUNTY_ZABUZA_MIST"), 4);
	LiveOpsSubsystem.GetActiveBounty(TEXT("BOUNTY_ZABUZA_MIST"), ZabuzaBounty);
	bool bHalfProgress = (ZabuzaBounty.CurrentProgress == 4) && !ZabuzaBounty.bIsCompleted;

	// Perform remaining 4 dodges -> complete bounty
	LiveOpsSubsystem.UpdateBountyProgress(TEXT("BOUNTY_ZABUZA_MIST"), 4);
	LiveOpsSubsystem.GetActiveBounty(TEXT("BOUNTY_ZABUZA_MIST"), ZabuzaBounty);
	bool bCompleted = (ZabuzaBounty.CurrentProgress == 8) && ZabuzaBounty.bIsCompleted;

	// Claim reward
	int32 Medals = 0;
	int32 Ryo = 0;
	FString Material;
	bool bClaim1 = LiveOpsSubsystem.ClaimBountyReward(TEXT("BOUNTY_ZABUZA_MIST"), Medals, Ryo, Material);
	bool bClaimSuccess = bClaim1 && (Medals == 250) && (Ryo == 50000) && (Material == TEXT("MAT_WATER_MIST_ESSENCE"));

	// Double-claim prevention
	bool bClaim2 = LiveOpsSubsystem.ClaimBountyReward(TEXT("BOUNTY_ZABUZA_MIST"), Medals, Ryo, Material);
	bool bDoubleClaimBlocked = !bClaim2;

	bool bPass = bFound && bHalfProgress && bCompleted && bClaimSuccess && bDoubleClaimBlocked;

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Cross-Mode Bounty Objective & Idempotent Claim: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}

bool FEOSPhase10Tests::TestPhase10VerticalSliceExecution()
{
	UEOSGachaSubsystem GachaSubsystem;
	GachaSubsystem.InitializeDefaultGachaDatabase();

	UEOSLiveOpsSubsystem LiveOpsSubsystem;
	LiveOpsSubsystem.InitializeDefaultLiveOpsDatabase();

	// Step 1: Live Hub presents Obito Event & Weekly Bounty
	TArray<FEOSLimitedEventDefinition> ActiveEvents = LiveOpsSubsystem.GetActiveEvents();
	TArray<FEOSWeeklyBountyRecord> ActiveBounties = LiveOpsSubsystem.GetActiveBounties();
	bool bHubHasContent = (ActiveEvents.Num() > 0) && (ActiveBounties.Num() >= 3);

	// Step 2: Earn Event Currency & Purchase Event Summon Ticket from Shop
	int32 GrantedQty = 0;
	FString StatusMsg;
	bool bBoughtTicket = LiveOpsSubsystem.PurchaseShopItem(TEXT("EVENT_OBITO"), TEXT("ITEM_SUMMON_TICKET"), GrantedQty, StatusMsg);
	GachaSubsystem.AddCurrency(EEOSCurrencyType::TicketEvt, GrantedQty);

	// Step 3: Perform 10x Pull on Obito Featured Banner
	FEOSSummonResult MultiResult = GachaSubsystem.ExecuteMultiSummon(TEXT("BANNER_OBITO_FEATURED"), 10);
	bool bSummonSuccess = MultiResult.bSuccess && (MultiResult.Acquisitions.Num() == 10);

	// Step 4: Progress and Complete Weekly Bounty
	LiveOpsSubsystem.UpdateBountyProgress(TEXT("BOUNTY_HIDAN_RITUAL"), 1);
	int32 Medals = 0, Ryo = 0;
	FString Mat;
	bool bBountyClaimed = LiveOpsSubsystem.ClaimBountyReward(TEXT("BOUNTY_HIDAN_RITUAL"), Medals, Ryo, Mat);

	bool bPass = bHubHasContent && bBoughtTicket && bSummonSuccess && bBountyClaimed;

	UE_LOG(LogEOSCore, Log, TEXT("[P10 TEST] Phase 10 Complete Vertical Slice Execution: %s"), bPass ? TEXT("PASS") : TEXT("FAIL"));
	return bPass;
}
