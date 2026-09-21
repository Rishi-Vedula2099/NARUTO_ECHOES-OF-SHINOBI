#include "../../Public/Subsystems/EOSCollectionSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSCollectionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultCollectionDatabase();
}

void UEOSCollectionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSCollectionSubsystem::InitializeDefaultCollectionDatabase()
{
	auto AddEntry = [this](const FString& ID, EEOSCollectionCategory Cat, const FString& Name, bool bOwned, bool bMastered)
	{
		FEOSCollectionEntryRecord Entry;
		Entry.EntryID = ID;
		Entry.Category = Cat;
		Entry.DisplayName = Name;
		Entry.bIsOwned = bOwned;
		Entry.bIsDiscovered = true;
		Entry.bIsMastered = bMastered;
		RegisterCollectionEntry(Entry);
	};

	// 1. Characters
	AddEntry(TEXT("COL_CHR_NARUTO"), EEOSCollectionCategory::Characters, TEXT("Naruto Uzumaki"), true, true);
	AddEntry(TEXT("COL_CHR_SASUKE"), EEOSCollectionCategory::Characters, TEXT("Sasuke Uchiha"), true, true);
	AddEntry(TEXT("COL_CHR_KAKASHI"), EEOSCollectionCategory::Characters, TEXT("Kakashi Hatake"), true, false);
	AddEntry(TEXT("COL_CHR_OBITO"), EEOSCollectionCategory::Characters, TEXT("Masked Obito"), false, false);

	// 2. Forms
	AddEntry(TEXT("COL_FORM_SAGE_MODE"), EEOSCollectionCategory::Forms, TEXT("Toad Sage Mode"), true, true);
	AddEntry(TEXT("COL_FORM_CURSE_MARK_2"), EEOSCollectionCategory::Forms, TEXT("Heaven Curse Mark Stage 2"), true, false);

	// 3. Jutsu
	AddEntry(TEXT("COL_JUT_RASENGAN"), EEOSCollectionCategory::Jutsu, TEXT("Rasengan (Spiraling Sphere)"), true, true);
	AddEntry(TEXT("COL_JUT_CHIDORI"), EEOSCollectionCategory::Jutsu, TEXT("Chidori (One Thousand Birds)"), true, true);

	// 4. Weapons
	AddEntry(TEXT("COL_WEP_KUSANAGI"), EEOSCollectionCategory::Weapons, TEXT("Sword of Kusanagi"), true, false);
	AddEntry(TEXT("COL_WEP_SAMEHADA"), EEOSCollectionCategory::Weapons, TEXT("Samehada Greatsword"), false, false);

	// 5. Artifacts
	AddEntry(TEXT("COL_ART_SAGE_URN"), EEOSCollectionCategory::Artifacts, TEXT("Sacred Toad Oil Urn"), true, false);
	AddEntry(TEXT("COL_ART_UCHIHA_FAN"), EEOSCollectionCategory::Artifacts, TEXT("Uchiha Crest Flame Talisman"), true, false);

	// 6. Gear Sets
	AddEntry(TEXT("COL_SET_UCHIHA"), EEOSCollectionCategory::GearSets, TEXT("Uchiha Legacy (4-Piece)"), true, false);
	AddEntry(TEXT("COL_SET_WILL_OF_FIRE"), EEOSCollectionCategory::GearSets, TEXT("Will of Fire (4-Piece)"), true, false);

	// 7. Lore
	AddEntry(TEXT("COL_LORE_WILL_OF_FIRE"), EEOSCollectionCategory::Lore, TEXT("The Will of Fire Chronicle"), true, true);

	// 8. Enemies
	AddEntry(TEXT("COL_ENM_WHITE_ZETSU"), EEOSCollectionCategory::Enemies, TEXT("White Zetsu Clone Vanguard"), true, false);

	// 9. Bosses
	AddEntry(TEXT("COL_BOSS_ZABUZA"), EEOSCollectionCategory::Bosses, TEXT("Zabuza Momochi Rematch"), true, true);
	AddEntry(TEXT("COL_BOSS_OBITO"), EEOSCollectionCategory::Bosses, TEXT("Masked Obito Adaptive Boss"), false, false);

	// 10. World Locations
	AddEntry(TEXT("COL_LOC_KONOHA_MARKET"), EEOSCollectionCategory::WorldLocations, TEXT("Konoha Central Market"), true, true);
	AddEntry(TEXT("COL_LOC_VALLEY_OF_END"), EEOSCollectionCategory::WorldLocations, TEXT("Valley of the End Chasm"), true, true);

	// 11. Motion Manga
	AddEntry(TEXT("COL_MM_LAND_OF_WAVES"), EEOSCollectionCategory::MotionManga, TEXT("Climax on the Great Naruto Bridge"), true, true);
	AddEntry(TEXT("COL_MM_OBITO_UNMASKED"), EEOSCollectionCategory::MotionManga, TEXT("Behind the Mask (Kamui Climax)"), false, false);
}

bool UEOSCollectionSubsystem::RegisterCollectionEntry(const FEOSCollectionEntryRecord& Entry)
{
	if (Entry.EntryID.IsEmpty()) return false;
	CollectionRegistry.Add(Entry.EntryID, Entry);
	return true;
}

bool UEOSCollectionSubsystem::GetCollectionEntry(const FString& EntryID, FEOSCollectionEntryRecord& OutEntry) const
{
	if (const FEOSCollectionEntryRecord* Found = CollectionRegistry.Find(EntryID))
	{
		OutEntry = *Found;
		return true;
	}
	return false;
}

TArray<FEOSCollectionEntryRecord> UEOSCollectionSubsystem::GetEntriesByCategory(EEOSCollectionCategory Category) const
{
	TArray<FEOSCollectionEntryRecord> Result;
	for (const auto& Pair : CollectionRegistry)
	{
		if (Pair.Value.Category == Category)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

bool UEOSCollectionSubsystem::SetEntryOwned(const FString& EntryID, bool bOwned)
{
	if (FEOSCollectionEntryRecord* Found = CollectionRegistry.Find(EntryID))
	{
		Found->bIsOwned = bOwned;
		Found->bIsDiscovered = true;
		return true;
	}
	return false;
}

bool UEOSCollectionSubsystem::SetEntryMastered(const FString& EntryID, bool bMastered)
{
	if (FEOSCollectionEntryRecord* Found = CollectionRegistry.Find(EntryID))
	{
		Found->bIsMastered = bMastered;
		if (bMastered) Found->bIsOwned = true;
		return true;
	}
	return false;
}

float UEOSCollectionSubsystem::GetCategoryCompletionPercentage(EEOSCollectionCategory Category) const
{
	int32 Total = 0;
	int32 Owned = 0;
	for (const auto& Pair : CollectionRegistry)
	{
		if (Pair.Value.Category == Category)
		{
			Total++;
			if (Pair.Value.bIsOwned) Owned++;
		}
	}
	return Total > 0 ? ((float)Owned / (float)Total) * 100.0f : 0.0f;
}

float UEOSCollectionSubsystem::GetGlobalArchiveCompletionPercentage() const
{
	int32 Total = CollectionRegistry.Num();
	int32 Owned = 0;
	for (const auto& Pair : CollectionRegistry)
	{
		if (Pair.Value.bIsOwned) Owned++;
	}
	return Total > 0 ? ((float)Owned / (float)Total) * 100.0f : 0.0f;
}

int32 UEOSCollectionSubsystem::GetArchiveAccountLevel() const
{
	int32 MasteredCount = 0;
	for (const auto& Pair : CollectionRegistry)
	{
		if (Pair.Value.bIsMastered) MasteredCount++;
	}
	return 1 + (MasteredCount / 2); // Level up every 2 mastered entries
}

int32 UEOSCollectionSubsystem::EvaluateMilestoneRewards(int32& OutGrantedGems, FString& OutUnlockedTitle)
{
	float GlobalComp = GetGlobalArchiveCompletionPercentage();
	int32 CurrentTier = 0;
	if (GlobalComp >= 100.0f) CurrentTier = 4;
	else if (GlobalComp >= 75.0f) CurrentTier = 3;
	else if (GlobalComp >= 50.0f) CurrentTier = 2;
	else if (GlobalComp >= 25.0f) CurrentTier = 1;

	if (CurrentTier > LastClaimedMilestoneTier)
	{
		int32 TiersGained = CurrentTier - LastClaimedMilestoneTier;
		LastClaimedMilestoneTier = CurrentTier;
		OutGrantedGems = TiersGained * 500;
		OutUnlockedTitle = FString::Printf(TEXT("Shinobi Grand Archivist Tier %d"), CurrentTier);
		return CurrentTier;
	}

	OutGrantedGems = 0;
	OutUnlockedTitle = TEXT("");
	return LastClaimedMilestoneTier;
}
