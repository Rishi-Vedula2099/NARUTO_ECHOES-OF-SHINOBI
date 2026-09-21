#include "../../Public/Subsystems/EOSShinobiIntelSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSShinobiIntelSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultIntelDatabase();
}

void UEOSShinobiIntelSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSShinobiIntelSubsystem::InitializeDefaultIntelDatabase()
{
	// 1. Character Intel
	FEOSShinobiIntelRecord NarutoIntel;
	NarutoIntel.IntelID = TEXT("INTEL_CHR_NARUTO");
	NarutoIntel.Category = EEOSIntelCategory::Character;
	NarutoIntel.Title = TEXT("Naruto Uzumaki");
	NarutoIntel.DiscoveryState = EEOSDiscoveryState::FullyRevealed;
	NarutoIntel.EncounterCount = 50;
	NarutoIntel.ElementAffinity = TEXT("Wind Nature");
	NarutoIntel.LoreSummary = TEXT("Nine-Tails Jinchūriki, student of Jiraiya, and destined savior of the shinobi world.");
	NarutoIntel.CrossLinkedEntityIDs = {
		TEXT("INTEL_CLAN_UZUMAKI"),
		TEXT("INTEL_VILLAGE_KONOHA"),
		TEXT("INTEL_CHR_JIRAIYA"),
		TEXT("INTEL_CHR_SASUKE"),
		TEXT("INTEL_JUT_RASENGAN"),
		TEXT("INTEL_FORM_SAGE"),
		TEXT("INTEL_BEAST_KURAMA")
	};
	RegisterIntelRecord(NarutoIntel);

	FEOSShinobiIntelRecord SasukeIntel;
	SasukeIntel.IntelID = TEXT("INTEL_CHR_SASUKE");
	SasukeIntel.Category = EEOSIntelCategory::Character;
	SasukeIntel.Title = TEXT("Sasuke Uchiha");
	SasukeIntel.DiscoveryState = EEOSDiscoveryState::FullyRevealed;
	SasukeIntel.EncounterCount = 45;
	SasukeIntel.ElementAffinity = TEXT("Fire / Lightning Nature");
	SasukeIntel.LoreSummary = TEXT("Sole survivor of the Uchiha Clan massacre, wielding the Sharingan and Chidori.");
	SasukeIntel.CrossLinkedEntityIDs = {
		TEXT("INTEL_CLAN_UCHIHA"),
		TEXT("INTEL_VILLAGE_KONOHA"),
		TEXT("INTEL_CHR_NARUTO"),
		TEXT("INTEL_CHR_ITACHI"),
		TEXT("INTEL_JUT_CHIDORI")
	};
	RegisterIntelRecord(SasukeIntel);

	// 2. Boss Intel (Adaptive AI Observations)
	FEOSShinobiIntelRecord ObitoBossIntel;
	ObitoBossIntel.IntelID = TEXT("INTEL_BOSS_OBITO");
	ObitoBossIntel.Category = EEOSIntelCategory::Boss;
	ObitoBossIntel.Title = TEXT("Masked Obito (Adaptive Boss)");
	ObitoBossIntel.DiscoveryState = EEOSDiscoveryState::PartiallyRevealed;
	ObitoBossIntel.EncounterCount = 3;
	ObitoBossIntel.ElementAffinity = TEXT("Fire / Space-Time");
	ObitoBossIntel.WeaknessDescription = TEXT("Materialization Vulnerability: 0.75s strike window immediately following space-time reappearance.");
	ObitoBossIntel.ObservedBehaviorNotes = TEXT("Phases through physical and ninjutsu attacks; teleports behind player on jutsu windups.");
	ObitoBossIntel.LoreSummary = TEXT("The mastermind pulling the strings of the Akatsuki from within the shadows.");
	ObitoBossIntel.CrossLinkedEntityIDs = {
		TEXT("INTEL_CLAN_UCHIHA"),
		TEXT("INTEL_CHR_KAKASHI"),
		TEXT("INTEL_REALM_KAMUI")
	};
	RegisterIntelRecord(ObitoBossIntel);

	FEOSShinobiIntelRecord ZabuzaBossIntel;
	ZabuzaBossIntel.IntelID = TEXT("INTEL_BOSS_ZABUZA");
	ZabuzaBossIntel.Category = EEOSIntelCategory::Boss;
	ZabuzaBossIntel.Title = TEXT("Zabuza Momochi (Demon of the Mist)");
	ZabuzaBossIntel.DiscoveryState = EEOSDiscoveryState::FullyRevealed;
	ZabuzaBossIntel.EncounterCount = 12;
	ZabuzaBossIntel.ElementAffinity = TEXT("Water Nature");
	ZabuzaBossIntel.WeaknessDescription = TEXT("Sensory Disruption: Sonic and Wind dispersal jutsus penetrate his Silent Killing fog.");
	ZabuzaBossIntel.ObservedBehaviorNotes = TEXT("Strikes with water clones from blind spots during dense mist activation.");
	ZabuzaBossIntel.CrossLinkedEntityIDs = {
		TEXT("INTEL_VILLAGE_KIRI"),
		TEXT("INTEL_WEAPON_KUBIKIRIBOCHO")
	};
	RegisterIntelRecord(ZabuzaBossIntel);

	// 3. Lore & World Records
	FEOSShinobiIntelRecord WillOfFire;
	WillOfFire.IntelID = TEXT("INTEL_LORE_WILL_OF_FIRE");
	WillOfFire.Category = EEOSIntelCategory::Lore;
	WillOfFire.Title = TEXT("The Will of Fire");
	WillOfFire.DiscoveryState = EEOSDiscoveryState::FullyRevealed;
	WillOfFire.LoreSummary = TEXT("The spiritual ideal passed down from the First Hokage that all Konoha villagers are family.");
	WillOfFire.CrossLinkedEntityIDs = {
		TEXT("INTEL_VILLAGE_KONOHA"),
		TEXT("INTEL_CHR_NARUTO")
	};
	RegisterIntelRecord(WillOfFire);

	FEOSShinobiIntelRecord KamuiVoid;
	KamuiVoid.IntelID = TEXT("INTEL_REALM_KAMUI");
	KamuiVoid.Category = EEOSIntelCategory::Location;
	KamuiVoid.Title = TEXT("Kamui Dimension (Space-Time Void)");
	KamuiVoid.DiscoveryState = EEOSDiscoveryState::Discovered;
	KamuiVoid.LoreSummary = TEXT("An infinite dimension comprised of suspended concrete monolith blocks, accessible only via Mangekyo Sharingan.");
	KamuiVoid.CrossLinkedEntityIDs = {
		TEXT("INTEL_BOSS_OBITO"),
		TEXT("INTEL_CHR_KAKASHI")
	};
	RegisterIntelRecord(KamuiVoid);
}

bool UEOSShinobiIntelSubsystem::RegisterIntelRecord(const FEOSShinobiIntelRecord& Record)
{
	if (Record.IntelID.IsEmpty()) return false;
	IntelRegistry.Add(Record.IntelID, Record);
	return true;
}

bool UEOSShinobiIntelSubsystem::GetIntelRecord(const FString& IntelID, FEOSShinobiIntelRecord& OutRecord) const
{
	if (const FEOSShinobiIntelRecord* Found = IntelRegistry.Find(IntelID))
	{
		OutRecord = *Found;
		return true;
	}
	return false;
}

TArray<FEOSShinobiIntelRecord> UEOSShinobiIntelSubsystem::GetIntelByCategory(EEOSIntelCategory Category) const
{
	TArray<FEOSShinobiIntelRecord> Result;
	for (const auto& Pair : IntelRegistry)
	{
		if (Pair.Value.Category == Category)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

bool UEOSShinobiIntelSubsystem::LogEncounter(const FString& IntelID)
{
	if (FEOSShinobiIntelRecord* Found = IntelRegistry.Find(IntelID))
	{
		Found->EncounterCount++;
		if (Found->DiscoveryState == EEOSDiscoveryState::Locked)
		{
			Found->DiscoveryState = EEOSDiscoveryState::Discovered;
		}
		else if (Found->DiscoveryState == EEOSDiscoveryState::Discovered && Found->EncounterCount >= 3)
		{
			Found->DiscoveryState = EEOSDiscoveryState::PartiallyRevealed;
		}
		else if (Found->DiscoveryState == EEOSDiscoveryState::PartiallyRevealed && Found->EncounterCount >= 5)
		{
			Found->DiscoveryState = EEOSDiscoveryState::FullyRevealed;
		}
		return true;
	}
	return false;
}

bool UEOSShinobiIntelSubsystem::SetDiscoveryState(const FString& IntelID, EEOSDiscoveryState State)
{
	if (FEOSShinobiIntelRecord* Found = IntelRegistry.Find(IntelID))
	{
		Found->DiscoveryState = State;
		return true;
	}
	return false;
}

TArray<FString> UEOSShinobiIntelSubsystem::GetCrossLinkedEntities(const FString& IntelID) const
{
	if (const FEOSShinobiIntelRecord* Found = IntelRegistry.Find(IntelID))
	{
		return Found->CrossLinkedEntityIDs;
	}
	return TArray<FString>();
}

bool UEOSShinobiIntelSubsystem::AddCrossLink(const FString& IntelIDA, const FString& IntelIDB)
{
	FEOSShinobiIntelRecord* FoundA = IntelRegistry.Find(IntelIDA);
	FEOSShinobiIntelRecord* FoundB = IntelRegistry.Find(IntelIDB);
	if (FoundA && FoundB)
	{
		FoundA->CrossLinkedEntityIDs.AddUnique(IntelIDB);
		FoundB->CrossLinkedEntityIDs.AddUnique(IntelIDA);
		return true;
	}
	return false;
}

int32 UEOSShinobiIntelSubsystem::GetTotalDiscoveredCount() const
{
	int32 Count = 0;
	for (const auto& Pair : IntelRegistry)
	{
		if (Pair.Value.DiscoveryState != EEOSDiscoveryState::Locked)
		{
			Count++;
		}
	}
	return Count;
}

float UEOSShinobiIntelSubsystem::GetCategoryResearchPercentage(EEOSIntelCategory Category) const
{
	int32 Total = 0;
	int32 FullyRevealed = 0;
	for (const auto& Pair : IntelRegistry)
	{
		if (Pair.Value.Category == Category)
		{
			Total++;
			if (Pair.Value.DiscoveryState == EEOSDiscoveryState::FullyRevealed ||
			    Pair.Value.DiscoveryState == EEOSDiscoveryState::Mastered)
			{
				FullyRevealed++;
			}
		}
	}
	return Total > 0 ? ((float)FullyRevealed / (float)Total) * 100.0f : 0.0f;
}
