#pragma once

#include "CoreMinimal.h"

/**
 * Automated Verification Suites for Phase 10:
 * Gacha, Summons, Banners, Rarity, Pity, Currencies, Limited Events, Weekly Bounties & LiveOps
 */
class ECHOESOFSHINOBI_API FEOSPhase10Tests
{
public:
	static bool RunAllPhase10ValidationTests();

	// 10 Dedicated Unit / Integration Tests
	static bool TestBannerLoadingAndRegistry();
	static bool TestPoolAndRarityWeights();
	static bool TestPityAndGuaranteeResolution();
	static bool TestDuplicateConversionAndInventory();
	static bool TestTransactionSafetyAndAtomicCurrency();
	static bool TestLimitedEventLifecycleAndPhases();
	static bool TestEventCurrencyAndShopPurchaseLimits();
	static bool TestConfigurableThursdayReset();
	static bool TestCrossModeBountyAttachmentAndProgress();
	static bool TestPhase10VerticalSliceExecution();
};
