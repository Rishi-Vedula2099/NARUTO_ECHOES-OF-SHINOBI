#include "../../Public/Subsystems/EOSWorldSubsystem.h"

UEOSWorldSubsystem::UEOSWorldSubsystem()
{
}

void UEOSWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ZoneRegistry.Empty();
	FastTravelRegistry.Empty();
	LoreRegistry.Empty();
	NPCRegistry.Empty();

	ActiveZoneID = "ZONE_KONOHA_MARKET";
	CurrentWeatherVariant = EEOSWeatherTimeVariant::Day;
	ActiveWorldStateID = "WORLD_STATE_KONOHA_NORMAL";

	InitializeFullWorldDatabase();
}

void UEOSWorldSubsystem::Deinitialize()
{
	ZoneRegistry.Empty();
	FastTravelRegistry.Empty();
	LoreRegistry.Empty();
	NPCRegistry.Empty();
	Super::Deinitialize();
}

void UEOSWorldSubsystem::InitializeFullWorldDatabase()
{
	ZoneRegistry.Empty();
	FastTravelRegistry.Empty();
	LoreRegistry.Empty();
	NPCRegistry.Empty();

	// 1. Author Zone Definitions across all Lands & Realms
	struct FZoneDef
	{
		const char* ZoneID;
		EEOSWorldRegion Region;
		EEOSZoneType ZoneType;
		const char* DisplayName;
		const char* Description;
		const char* FastTravelID;
		const char* ConnectedZones[4];
		const char* CanonStatus;
	};

	const FZoneDef ZoneList[] = {
		// Konohagakure (10 zones)
		{ "ZONE_KONOHA_MARKET", EEOSWorldRegion::Konoha, EEOSZoneType::HubVillage, "Konoha Main Market District", "Bustling merchant street featuring Ichiraku Ramen and weapon smiths.", "FTP_KONOHA_MAIN_GATE", { "ZONE_KONOHA_TRAINING_7", "ZONE_KONOHA_ROOFTOPS", "ZONE_KONOHA_HOSPITAL", "ZONE_KONOHA_ACADEMY" }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_TRAINING_7", EEOSWorldRegion::Konoha, EEOSZoneType::TrainingGrounds, "Training Ground #7", "Riverbank with the Three Bell posts and memorial forest clearing.", "FTP_TRAINING_GROUND_7", { "ZONE_KONOHA_MARKET", "ZONE_KONOHA_FOREST_DEATH", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_ROOFTOPS", EEOSWorldRegion::Konoha, EEOSZoneType::RooftopNetwork, "Konoha Rooftop Highway", "Interconnected red-tile roofs, wirelines, and highline traversal routes.", "FTP_KONOHA_ROOFTOPS", { "ZONE_KONOHA_MARKET", "ZONE_KONOHA_HOKAGE_TOWER", "ZONE_KONOHA_ACADEMY", nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_FOREST_DEATH", EEOSWorldRegion::Konoha, EEOSZoneType::ForestWilderness, "Forest of Death (Zone 44)", "Perimeter fence guarding giant predatory trees and locked central stone tower.", "FTP_FOREST_OF_DEATH", { "ZONE_KONOHA_TRAINING_7", "ZONE_FIRE_TEMPLE", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_HOSPITAL", EEOSWorldRegion::Konoha, EEOSZoneType::InteriorFacility, "Konoha Medical Hospital", "Intensive medical care wards and rooftop rehabilitation pavilion.", "FTP_KONOHA_HOSPITAL", { "ZONE_KONOHA_MARKET", "ZONE_KONOHA_ROOFTOPS", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_ACADEMY", EEOSWorldRegion::Konoha, EEOSZoneType::InteriorFacility, "Shinobi Academy & Courtyard", "Classroom lecture halls, sparring grounds, and the historic tree swing.", "FTP_KONOHA_ACADEMY", { "ZONE_KONOHA_MARKET", "ZONE_KONOHA_ROOFTOPS", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_HOKAGE_TOWER", EEOSWorldRegion::Konoha, EEOSZoneType::MemorialMonument, "Hokage Tower & Monument", "Administrative residence and cliffside ascent to the Great Stone Faces.", "FTP_HOKAGE_TOWER", { "ZONE_KONOHA_ROOFTOPS", "ZONE_KONOHA_MEMORIAL_STONE", "ZONE_KONOHA_ROOT_UNDERGROUND", nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_MEMORIAL_STONE", EEOSWorldRegion::Konoha, EEOSZoneType::MemorialMonument, "Hero's Memorial Cenotaph", "Quiet wooded glade housing the black granite stone engraved with fallen heroes.", "FTP_MEMORIAL_STONE", { "ZONE_KONOHA_HOKAGE_TOWER", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_ROOT_UNDERGROUND", EEOSWorldRegion::Konoha, EEOSZoneType::UndergroundNetwork, "Root / ANBU Underground Routes", "Subterranean catacombs, drainage conduits, and Foundation training cells.", "FTP_ROOT_UNDERGROUND", { "ZONE_KONOHA_HOKAGE_TOWER", "ZONE_KONOHA_BORUTO_MODERN", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KONOHA_BORUTO_MODERN", EEOSWorldRegion::Konoha, EEOSZoneType::District, "Modern Technological Konoha", "Boruto-era district with Thunder Rail stations and Scientific Tool labs.", "FTP_BORUTO_TECH_DISTRICT", { "ZONE_KONOHA_MARKET", "ZONE_KONOHA_ROOT_UNDERGROUND", nullptr, nullptr }, "CANON_VERIFIED" },

		// Land of Fire (3 zones)
		{ "ZONE_FIRE_TEMPLE", EEOSWorldRegion::LandOfFire, EEOSZoneType::District, "Fire Temple of the Monks", "Sacred temple grounds flanked by monolithic Nio guardian gatekeepers.", "FTP_FIRE_TEMPLE", { "ZONE_KONOHA_FOREST_DEATH", "ZONE_TANZAKU_QUARTERS", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_TANZAKU_QUARTERS", EEOSWorldRegion::LandOfFire, EEOSZoneType::District, "Tanzaku Town & Castle Ruins", "Lively gambling avenues and ancient hillside fortress ramparts.", "FTP_TANZAKU_TOWN", { "ZONE_FIRE_TEMPLE", "ZONE_WAVES_BRIDGE", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_WAVES_BRIDGE", EEOSWorldRegion::LandOfFire, EEOSZoneType::District, "Great Naruto Bridge", "Massive ocean suspension bridge shrouded in mist banks at the Waves estuary.", "FTP_WAVES_BRIDGE", { "ZONE_TANZAKU_QUARTERS", "ZONE_VALLEY_OF_THE_END", nullptr, nullptr }, "CANON_VERIFIED" },

		// Land of Wind & Sunagakure (3 zones)
		{ "ZONE_SUNA_GATE", EEOSWorldRegion::LandOfWind, EEOSZoneType::HubVillage, "Sunagakure Valley Gate", "Narrow sandstone chasm gateway guarding the Hidden Sand Village.", "FTP_SUNA_MAIN_GATE", { "ZONE_SUNA_ROOFTOPS", "ZONE_SUNA_DUNES", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_SUNA_ROOFTOPS", EEOSWorldRegion::LandOfWind, EEOSZoneType::RooftopNetwork, "Suna Adobe Rooftops & Kazekage Spire", "Circular baked clay roofs, puppet atelier, and wind-carved parapets.", "FTP_SUNA_KAZEKAGE_SPIRE", { "ZONE_SUNA_GATE", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_SUNA_DUNES", EEOSWorldRegion::LandOfWind, EEOSZoneType::ForestWilderness, "Great Sand Dunes & Quicksand Canyons", "Vast shifting dunes, ancient buried ruins, and puppet salvage hollows.", "FTP_SUNA_DUNES", { "ZONE_SUNA_GATE", "ZONE_WAR_FRONTS", nullptr, nullptr }, "CANON_VERIFIED" },

		// Land of Water & Kirigakure (3 zones)
		{ "ZONE_KIRI_CANALS", EEOSWorldRegion::LandOfWater, EEOSZoneType::HubVillage, "Kirigakure Canal District", "Mist-shrouded tidal waterways, iron footbridges, and brick cylindrical spires.", "FTP_KIRI_CANALS", { "ZONE_KIRI_MIST_FOREST", "ZONE_KIRI_SWORD_COVE", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KIRI_MIST_FOREST", EEOSWorldRegion::LandOfWater, EEOSZoneType::ForestWilderness, "Mist Forest & Silent Killing Swamps", "Dense bamboo thickets enclosed by permanent dense fog.", "FTP_KIRI_MIST_FOREST", { "ZONE_KIRI_CANALS", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KIRI_SWORD_COVE", EEOSWorldRegion::LandOfWater, EEOSZoneType::TrainingGrounds, "Seven Swordsmen Training Coves", "Secluded coastal caverns housing legendary sword trial shrines.", "FTP_KIRI_SWORD_COVE", { "ZONE_KIRI_CANALS", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },

		// Land of Lightning & Kumogakure (3 zones)
		{ "ZONE_KUMO_CRAGS", EEOSWorldRegion::LandOfLightning, EEOSZoneType::HubVillage, "Kumogakure Cloud Crags", "High-altitude suspension bridges crossing cloud-shrouded granite pinnacles.", "FTP_KUMO_CRAGS", { "ZONE_KUMO_RAIKAGE_TERRACE", "ZONE_KUMO_VALLEY_CLOUDS", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KUMO_RAIKAGE_TERRACE", EEOSWorldRegion::LandOfLightning, EEOSZoneType::InteriorFacility, "Raikage Office & Training Plateau", "Mountain-top stronghold terrace with heavy iron lightning training rigs.", "FTP_KUMO_RAIKAGE_TERRACE", { "ZONE_KUMO_CRAGS", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KUMO_VALLEY_CLOUDS", EEOSWorldRegion::LandOfLightning, EEOSZoneType::TrainingGrounds, "Valley of Clouds and Lightning", "Thundering waterfalls and natural grottos used for Tailed Beast training.", "FTP_KUMO_VALLEY_CLOUDS", { "ZONE_KUMO_CRAGS", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },

		// Land of Earth & Iwagakure (2 zones)
		{ "ZONE_IWA_CITADEL", EEOSWorldRegion::LandOfEarth, EEOSZoneType::HubVillage, "Iwagakure Stone Citadel", "Natural bowl-shaped mountain fortress with carved subterranean stone palaces.", "FTP_IWA_CITADEL", { "ZONE_IWA_QUARRY", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_IWA_QUARRY", EEOSWorldRegion::LandOfEarth, EEOSZoneType::TrainingGrounds, "Granite Quarries & Canyons", "Heavy stepped stone slabs and massive boulders used for Earth Style trials.", "FTP_IWA_QUARRY", { "ZONE_IWA_CITADEL", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },

		// Amegakure (2 zones)
		{ "ZONE_AME_SKYSCRAPERS", EEOSWorldRegion::Amegakure, EEOSZoneType::HubVillage, "Amegakure Steel Skyscraper Pavements", "Rain-slicked iron sky-walkways under continuous downpour.", "FTP_AME_SKYSCRAPERS", { "ZONE_AME_PAIN_TOWER", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_AME_PAIN_TOWER", EEOSWorldRegion::Amegakure, EEOSZoneType::MemorialMonument, "Pain's Sanctuary Tower", "Tallest architectural tower featuring stone gargoyle observation perches.", "FTP_AME_PAIN_TOWER", { "ZONE_AME_SKYSCRAPERS", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },

		// Mystical, Historical & Dimensional Realms (6 zones)
		{ "ZONE_MYOBOKU_SAGE_SANCTUARY", EEOSWorldRegion::MountMyoboku, EEOSZoneType::DimensionRealm, "Mount Myōboku Toad Sanctuary", "Colossal sacred oil falls, monolithic toad statues, and balance spires.", "FTP_MYOBOKU_SANCTUARY", { nullptr, nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_VALLEY_OF_THE_END", EEOSWorldRegion::ValleyOfTheEnd, EEOSZoneType::Battlefield, "Valley of the End", "Towering statues of Hashirama and Madara framing the thunderous waterfall.", "FTP_VALLEY_OF_THE_END", { "ZONE_WAVES_BRIDGE", nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_WAR_FRONTS", EEOSWorldRegion::WarFronts, EEOSZoneType::Battlefield, "Fourth Shinobi War Fronts", "Coastal defense lines, meteor craters, and Allied Shinobi trench fortifications.", "FTP_WAR_FRONTS", { "ZONE_SUNA_DUNES", "ZONE_DIVINE_TREE_CORE", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_KAMUI_DIMENSION", EEOSWorldRegion::KamuiDimension, EEOSZoneType::DimensionRealm, "Kamui Dimension Void", "Endless twilight expanse composed of floating geometric stone blocks.", "FTP_KAMUI_VOID", { nullptr, nullptr, nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_DIVINE_TREE_CORE", EEOSWorldRegion::DivineTreeRealms, EEOSZoneType::DimensionRealm, "Divine Tree Core & Roots", "Gigantic bioluminescent God Tree root caverns pulsing with cosmic chakra.", "FTP_DIVINE_TREE_CORE", { "ZONE_WAR_FRONTS", "ZONE_ECLIPSE_ASTRAL_CORE", nullptr, nullptr }, "CANON_VERIFIED" },
		{ "ZONE_ECLIPSE_ASTRAL_CORE", EEOSWorldRegion::AstralEclipseRealm, EEOSZoneType::DimensionRealm, "Eclipse Endgame Astral Realm", "Original game endgame celestial bridge reflecting the original bridge motif.", "FTP_ECLIPSE_ASTRAL_CORE", { "ZONE_DIVINE_TREE_CORE", nullptr, nullptr, nullptr }, "ORIGINAL_GAME_VARIANT" }
	};

	const int32 TotalZones = sizeof(ZoneList) / sizeof(ZoneList[0]);
	for (int32 i = 0; i < TotalZones; ++i)
	{
		const FZoneDef& ZD = ZoneList[i];
		FEOSWorldZone Zone;
		Zone.ZoneID = FString(UTF8_TO_TCHAR(ZD.ZoneID));
		Zone.Region = ZD.Region;
		Zone.ZoneType = ZD.ZoneType;
		Zone.DisplayName = FText::FromString(FString(UTF8_TO_TCHAR(ZD.DisplayName)));
		Zone.Description = FText::FromString(FString(UTF8_TO_TCHAR(ZD.Description)));
		Zone.FastTravelPointID = FString(UTF8_TO_TCHAR(ZD.FastTravelID));
		Zone.CanonStatus = FString(UTF8_TO_TCHAR(ZD.CanonStatus));
		Zone.ActiveWeather = EEOSWeatherTimeVariant::Day;
		Zone.RequiredWorldStateID = "WORLD_STATE_KONOHA_NORMAL";

		for (int32 c = 0; c < 4; ++c)
		{
			if (ZD.ConnectedZones[c] != nullptr)
			{
				Zone.ConnectedZoneIDs.Add(FString(UTF8_TO_TCHAR(ZD.ConnectedZones[c])));
			}
		}

		// Register Fast Travel Point
		FEOSFastTravelPoint FTP;
		FTP.PointID = Zone.FastTravelPointID;
		FTP.ZoneID = Zone.ZoneID;
		FTP.DisplayName = Zone.DisplayName;
		FTP.bIsUnlockedByDefault = (i == 0); // Konoha Market is unlocked by default
		FTP.bIsUnlocked = (i == 0);
		RegisterFastTravelPoint(FTP);

		// Register Lore Prop for Zone
		FEOSLoreProp Lore;
		Lore.PropID = FString::Printf(TEXT("LORE_%s_MEMORIAL"), *Zone.ZoneID);
		Lore.ZoneID = Zone.ZoneID;
		Lore.DisplayName = FText::FromString(FString::Printf(TEXT("%s Inscription"), *Zone.DisplayName.ToString()));
		Lore.Description = FText::FromString(FString::Printf(TEXT("Historical archives regarding %s."), *Zone.DisplayName.ToString()));
		Lore.Category = (i % 2 == 0) ? EEOSLoreCategory::Memorial : EEOSLoreCategory::Scroll;
		Lore.ArchiveUnlockID = FString::Printf(TEXT("ARCHIVE_LORE_%s"), *Zone.ZoneID);
		Lore.bIsDiscovered = false;
		Zone.LorePropIDs.Add(Lore.PropID);
		RegisterLoreProp(Lore);

		// Register NPC for Zone
		FEOSNPCInteractable NPC;
		NPC.NPCID = FString::Printf(TEXT("NPC_%s_GUIDE"), *Zone.ZoneID);
		NPC.ZoneID = Zone.ZoneID;
		NPC.DisplayName = FText::FromString(FString::Printf(TEXT("Area Guide (%s)"), *Zone.DisplayName.ToString()));
		NPC.Role = (i == 0) ? EEOSNPCRole::MissionGiver : EEOSNPCRole::LoreKeeper;
		NPC.DialogueSetID = FString::Printf(TEXT("DLG_ZONE_%s"), *Zone.ZoneID);
		NPC.RequiredWorldStateID = "WORLD_STATE_KONOHA_NORMAL";
		Zone.NPCIDs.Add(NPC.NPCID);
		NPCRegistry.Add(NPC.NPCID, NPC);

		RegisterZone(Zone);
	}
}

void UEOSWorldSubsystem::RegisterZone(const FEOSWorldZone& Zone)
{
	if (!Zone.ZoneID.IsEmpty())
	{
		ZoneRegistry.Add(Zone.ZoneID, Zone);
	}
}

bool UEOSWorldSubsystem::GetZone(const FString& ZoneID, FEOSWorldZone& OutZone) const
{
	const FEOSWorldZone* Found = ZoneRegistry.Find(ZoneID);
	if (Found)
	{
		OutZone = *Found;
		return true;
	}
	return false;
}

TArray<FEOSWorldZone> UEOSWorldSubsystem::GetZonesByRegion(EEOSWorldRegion Region) const
{
	TArray<FEOSWorldZone> Result;
	for (const auto& Kvp : ZoneRegistry)
	{
		if (Kvp.Value.Region == Region)
		{
			Result.Add(Kvp.Value);
		}
	}
	return Result;
}

bool UEOSWorldSubsystem::TransitionToZone(const FString& ZoneID)
{
	if (ZoneRegistry.Contains(ZoneID))
	{
		ActiveZoneID = ZoneID;
		return true;
	}
	return false;
}

void UEOSWorldSubsystem::RegisterFastTravelPoint(const FEOSFastTravelPoint& Point)
{
	if (!Point.PointID.IsEmpty())
	{
		FastTravelRegistry.Add(Point.PointID, Point);
	}
}

bool UEOSWorldSubsystem::UnlockFastTravelPoint(const FString& PointID)
{
	FEOSFastTravelPoint* Found = FastTravelRegistry.Find(PointID);
	if (Found)
	{
		Found->bIsUnlocked = true;
		return true;
	}
	return false;
}

bool UEOSWorldSubsystem::FastTravelTo(const FString& PointID, FString& OutTargetZoneID)
{
	const FEOSFastTravelPoint* Found = FastTravelRegistry.Find(PointID);
	if (Found && Found->bIsUnlocked)
	{
		ActiveZoneID = Found->ZoneID;
		OutTargetZoneID = Found->ZoneID;
		return true;
	}
	return false;
}

TArray<FEOSFastTravelPoint> UEOSWorldSubsystem::GetUnlockedFastTravelPoints() const
{
	TArray<FEOSFastTravelPoint> Unlocked;
	for (const auto& Kvp : FastTravelRegistry)
	{
		if (Kvp.Value.bIsUnlocked)
		{
			Unlocked.Add(Kvp.Value);
		}
	}
	return Unlocked;
}

void UEOSWorldSubsystem::RegisterLoreProp(const FEOSLoreProp& Prop)
{
	if (!Prop.PropID.IsEmpty())
	{
		LoreRegistry.Add(Prop.PropID, Prop);
	}
}

bool UEOSWorldSubsystem::DiscoverLoreProp(const FString& PropID, FString& OutArchiveUnlockID)
{
	FEOSLoreProp* Found = LoreRegistry.Find(PropID);
	if (Found)
	{
		Found->bIsDiscovered = true;
		OutArchiveUnlockID = Found->ArchiveUnlockID;
		return true;
	}
	return false;
}

int32 UEOSWorldSubsystem::GetDiscoveredLoreCount() const
{
	int32 Count = 0;
	for (const auto& Kvp : LoreRegistry)
	{
		if (Kvp.Value.bIsDiscovered)
		{
			Count++;
		}
	}
	return Count;
}

void UEOSWorldSubsystem::SetWeatherTimeVariant(EEOSWeatherTimeVariant NewVariant)
{
	CurrentWeatherVariant = NewVariant;
	if (ZoneRegistry.Contains(ActiveZoneID))
	{
		ZoneRegistry[ActiveZoneID].ActiveWeather = NewVariant;
	}
}

void UEOSWorldSubsystem::ApplyWorldState(const FString& NewWorldStateID)
{
	ActiveWorldStateID = NewWorldStateID;
	if (NewWorldStateID.Contains(TEXT("CRUSHED")) || NewWorldStateID.Contains(TEXT("DAMAGED")))
	{
		CurrentWeatherVariant = EEOSWeatherTimeVariant::DamagedPostInvasion;
	}
	else if (NewWorldStateID.Contains(TEXT("RAIN")))
	{
		CurrentWeatherVariant = EEOSWeatherTimeVariant::Rain;
	}
	else
	{
		CurrentWeatherVariant = EEOSWeatherTimeVariant::Day;
	}
}

void UEOSWorldSubsystem::AuditWorldCoverage(int32& OutKonohaZones, int32& OutLandZones, int32& OutDimensionZones, int32& OutTotalLore) const
{
	OutKonohaZones = 0;
	OutLandZones = 0;
	OutDimensionZones = 0;
	OutTotalLore = LoreRegistry.Num();

	for (const auto& Kvp : ZoneRegistry)
	{
		if (Kvp.Value.Region == EEOSWorldRegion::Konoha)
		{
			OutKonohaZones++;
		}
		else if (Kvp.Value.Region == EEOSWorldRegion::KamuiDimension ||
		         Kvp.Value.Region == EEOSWorldRegion::DivineTreeRealms ||
		         Kvp.Value.Region == EEOSWorldRegion::AstralEclipseRealm ||
		         Kvp.Value.Region == EEOSWorldRegion::MountMyoboku)
		{
			OutDimensionZones++;
		}
		else
		{
			OutLandZones++;
		}
	}
}
