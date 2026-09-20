#include "../../Public/Subsystems/EOSStorySubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSStorySubsystem::UEOSStorySubsystem()
{
}

void UEOSStorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ArcRegistry.Empty();
	ChapterRegistry.Empty();
	StageRegistry.Empty();
	MotionMangaRegistry.Empty();
	ArchiveRegistry.Empty();
	CurrentWorldStateID = "WORLD_STATE_KONOHA_NORMAL";

	InitializeFullCampaignDatabase();
}

void UEOSStorySubsystem::Deinitialize()
{
	ArcRegistry.Empty();
	ChapterRegistry.Empty();
	StageRegistry.Empty();
	MotionMangaRegistry.Empty();
	ArchiveRegistry.Empty();
	Super::Deinitialize();
}

void UEOSStorySubsystem::InitializeFullCampaignDatabase()
{
	ArcRegistry.Empty();
	ChapterRegistry.Empty();
	StageRegistry.Empty();

	const struct FArcInfo
	{
		const char* ArcID;
		const char* Title;
		const char* CanonStatus;
		EEOSCharacterEra Era;
		const char* BossID;
		const char* EndWorldState;
	} ArcDefs[25] = {
		{ "ARC_01", "Land of Waves Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_GENIN, "BOSS_ZABUZA_HAKU", "WORLD_STATE_LAND_OF_WAVES_RESTORED" },
		{ "ARC_02", "Chunin Exams Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_GENIN, "BOSS_OROCHIMARU_FOREST", "WORLD_STATE_FOREST_OF_DEATH" },
		{ "ARC_03", "Konoha Crush Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_GENIN, "BOSS_GAARA_SHUKAKU", "WORLD_STATE_KONOHA_CRUSHED" },
		{ "ARC_04", "Search for Tsunade Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_GENIN, "BOSS_KABUTO_OROCHIMARU", "WORLD_STATE_TANZAKU_TOWN" },
		{ "ARC_05", "Sasuke Retrieval Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_GENIN, "BOSS_SASUKE_CURSE_MARK", "WORLD_STATE_VALLEY_OF_THE_END_PART1" },
		{ "ARC_06", "Kazekage Rescue Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_SASORI_HIRUKO", "WORLD_STATE_SUNA_SAVED" },
		{ "ARC_07", "Tenchi Bridge Reconnaissance Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_FOUR_TAILS_NARUTO", "WORLD_STATE_TENCHI_BRIDGE" },
		{ "ARC_08", "Twelve Guardian Shinobi & Akatsuki Suppression Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_KAKUZU_HIDAN", "WORLD_STATE_AKATSUKI_SUPPRESSED" },
		{ "ARC_09", "Itachi Pursuit Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_DEIDARA_C4", "WORLD_STATE_MIST_FOREST" },
		{ "ARC_10", "Tale of Jiraiya the Gallant Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_SIX_PATHS_OF_PAIN", "WORLD_STATE_RAIN_VILLAGE" },
		{ "ARC_11", "Fated Battle Between Brothers Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_ITACHI_SUSANOO", "WORLD_STATE_UCHIHA_HIDEOUT" },
		{ "ARC_12", "Pain's Assault Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_PAIN_TENDO_DEVA", "WORLD_STATE_KONOHA_REBUILT" },
		{ "ARC_13", "Five Kage Summit Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_DANZO_SHIMURA", "WORLD_STATE_SAMURAI_BRIDGE" },
		{ "ARC_14", "Nine-Tails Control & Island Turtle Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_SHIPPUDEN, "BOSS_KUBI_NINE_TAILS", "WORLD_STATE_ISLAND_TURTLE" },
		{ "ARC_15", "Fourth Shinobi World War: Countdown Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_KISAME_SAMEHADA", "WORLD_STATE_WAR_PREPARATION" },
		{ "ARC_16", "Fourth Shinobi World War: Confrontation Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_EDO_KIMIMARO", "WORLD_STATE_WAR_BATTLEFIELD" },
		{ "ARC_17", "Fourth Shinobi World War: Climax Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_EDO_MADARA_UCHIHA", "WORLD_STATE_DESERT_BATTLEFIELD" },
		{ "ARC_18", "Birth of the Ten-Tails Jinchuriki Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_OBITO_JUUBI", "WORLD_STATE_GOD_TREE" },
		{ "ARC_19", "Kaguya Otsutsuki Strikes Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_KAGUYA_OTSUTSUKI", "WORLD_STATE_LAVA_DIMENSION" },
		{ "ARC_20", "Final Battle: Valley of the End Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_WAR, "BOSS_SASUKE_INDRA_SUSANOO", "WORLD_STATE_PEACE_RESTORED" },
		{ "ARC_21", "Boruto Transition: Otsutsuki Invasion Arc", "CANON_VERIFIED", EEOSCharacterEra::ERA_BORUTO, "BOSS_MOMOSHIKI_OTSUTSUKI", "WORLD_STATE_NEW_ERA_KONOHA" },
		{ "ARC_22", "Infinite Tsukuyomi Trials Arc", "ORIGINAL_GAME_VARIANT", EEOSCharacterEra::ERA_BORUTO, "BOSS_DREAM_MADARA", "WORLD_STATE_DREAM_REALM" },
		{ "ARC_23", "Tailed Beast Hunt Arc", "ORIGINAL_GAME_VARIANT", EEOSCharacterEra::ERA_BORUTO, "BOSS_REMNANT_MATATABI", "WORLD_STATE_REMNANT_SANCTUARY" },
		{ "ARC_24", "Shinobi World: What If? Arc", "WHAT_IF", EEOSCharacterEra::ERA_BORUTO, "BOSS_WHATIF_MINATO_HOKAGE", "WORLD_STATE_ALTERNATE_TIMELINE" },
		{ "ARC_25", "Eclipse Endgame Arc", "ORIGINAL_GAME_VARIANT", EEOSCharacterEra::ERA_BORUTO, "BOSS_ECLIPSE_OTSUTSUKI_KING", "WORLD_STATE_ECLIPSE_CORE" }
	};

	for (int32 ArcIdx = 0; ArcIdx < 25; ++ArcIdx)
	{
		const FArcInfo& ADef = ArcDefs[ArcIdx];
		FString ArcIDStr = FString(UTF8_TO_TCHAR(ADef.ArcID));
		FString ArcTitleStr = FString(UTF8_TO_TCHAR(ADef.Title));
		FString ArcCanonStr = FString(UTF8_TO_TCHAR(ADef.CanonStatus));
		FString ArcBossStr = FString(UTF8_TO_TCHAR(ADef.BossID));
		FString ArcEndWorldStr = FString(UTF8_TO_TCHAR(ADef.EndWorldState));

		FEOSArcRecord ArcRecord;
		ArcRecord.ArcID = ArcIDStr;
		ArcRecord.DisplayName = FText::FromString(ArcTitleStr);
		ArcRecord.StoryEra = ADef.Era;
		ArcRecord.CanonStatus = ArcCanonStr;
		ArcRecord.StartWorldStateID = (ArcIdx == 0) ? FString("WORLD_STATE_KONOHA_NORMAL") : ArcDefs[ArcIdx - 1].EndWorldState;
		ArcRecord.EndWorldStateID = ArcEndWorldStr;

		for (int32 ChIdx = 1; ChIdx <= 4; ++ChIdx)
		{
			FString ChapterIDStr = FString::Printf(TEXT("CH_%02d_%02d"), ArcIdx + 1, ChIdx);
			ArcRecord.ChapterIDs.Add(ChapterIDStr);

			FEOSChapterRecord ChapterRecord;
			ChapterRecord.ChapterID = ChapterIDStr;
			ChapterRecord.ArcID = ArcIDStr;
			ChapterRecord.ChapterNumber = ChIdx;
			ChapterRecord.DisplayName = FText::FromString(FString::Printf(TEXT("%s - Chapter %d"), *ArcTitleStr, ChIdx));
			ChapterRecord.Synopsis = FText::FromString(FString::Printf(TEXT("Story progression for %s Chapter %d."), *ArcTitleStr, ChIdx));
			ChapterRecord.BossID = ArcBossStr;
			ChapterRecord.MotionMangaID = FString::Printf(TEXT("MANGA_%s_CH%02d"), *ArcIDStr, ChIdx);
			ChapterRecord.WorldStateDeltaID = FString::Printf(TEXT("WORLD_DELTA_%s_CH%02d"), *ArcIDStr, ChIdx);

			for (int32 StgIdx = 1; StgIdx <= 5; ++StgIdx)
			{
				FString StageIDStr = FString::Printf(TEXT("STG_%02d_%02d_%02d"), ArcIdx + 1, ChIdx, StgIdx);
				ChapterRecord.StageIDs.Add(StageIDStr);

				FEOSStageRecord StageRecord;
				StageRecord.StageID = StageIDStr;
				StageRecord.ArcID = ArcIDStr;
				StageRecord.ChapterID = ChapterIDStr;
				StageRecord.CanonStatus = ArcCanonStr;

				EEOSStageType Type = EEOSStageType::Story;
				if (StgIdx == 2) Type = EEOSStageType::Elite;
				else if (StgIdx == 3) Type = EEOSStageType::Challenge;
				else if (StgIdx == 4) Type = EEOSStageType::MiniBoss;
				else if (StgIdx == 5) Type = EEOSStageType::Boss;
				StageRecord.StageType = Type;

				int32 RecPow = 900 + ((ArcIdx + 1) * 750) + (ChIdx * 150) + (StgIdx * 30);
				int32 FloorPow = FMath::Max(600, RecPow - 350);
				StageRecord.RecommendedPower = RecPow;
				StageRecord.HardPowerFloor = FloorPow;

				StageRecord.DisplayName = FText::FromString(FString::Printf(TEXT("%s Stg %d.%d"), *ArcTitleStr, ChIdx, StgIdx));
				StageRecord.EnemySetID = FString::Printf(TEXT("ENEMY_SET_%s_%02d"), *ArcIDStr, StgIdx);
				StageRecord.EliteID = FString::Printf(TEXT("ELITE_%s_%02d"), *ArcIDStr, StgIdx);
				StageRecord.MiniBossID = FString::Printf(TEXT("MINIBOSS_%s_%02d"), *ArcIDStr, StgIdx);
				StageRecord.BossID = ArcBossStr;
				StageRecord.RewardTableID = FString::Printf(TEXT("RWD_%s_CH%02d_STG%02d"), *ArcIDStr, ChIdx, StgIdx);
				StageRecord.DialogueSetID = FString::Printf(TEXT("DLG_%s_CH%02d_STG%02d"), *ArcIDStr, ChIdx, StgIdx);
				StageRecord.MotionMangaID = (StgIdx == 5) ? ChapterRecord.MotionMangaID : FString("");
				StageRecord.WorldStateDeltaID = (StgIdx == 5) ? ChapterRecord.WorldStateDeltaID : FString("");

				FEOSStageObjective PrimaryObj;
				PrimaryObj.ObjectiveID = FString::Printf(TEXT("OBJ_PRIM_%s"), *StageIDStr);
				PrimaryObj.ObjectiveType = (StgIdx == 5) ? EEOSObjectiveType::DefeatBoss : EEOSObjectiveType::DefeatEnemy;
				PrimaryObj.TargetID = (StgIdx == 5) ? ArcBossStr : StageRecord.EnemySetID;
				PrimaryObj.TargetCount = (StgIdx == 5) ? 1 : 5;
				PrimaryObj.bIsOptional = false;

				FEOSStageObjective OptionalObj;
				OptionalObj.ObjectiveID = FString::Printf(TEXT("OBJ_OPT_%s"), *StageIDStr);
				OptionalObj.ObjectiveType = EEOSObjectiveType::AvoidDamage;
				OptionalObj.bIsOptional = true;

				StageRecord.Objectives.Add(PrimaryObj);
				StageRecord.Objectives.Add(OptionalObj);

				RegisterStage(StageRecord);
			}

			RegisterChapter(ChapterRecord);
		}

		RegisterArc(ArcRecord);

		FEOSMotionMangaSequence MangaSeq;
		MangaSeq.MotionSequenceID = FString::Printf(TEXT("MANGA_%s_END"), *ArcIDStr);
		MangaSeq.ArcID = ArcIDStr;
		MangaSeq.ChapterID = FString::Printf(TEXT("CH_%02d_04"), ArcIdx + 1);
		MangaSeq.StageID = FString::Printf(TEXT("STG_%02d_04_05"), ArcIdx + 1);
		MangaSeq.ParallaxDepth = 2.5f;
		MangaSeq.ArchiveUnlockID = FString::Printf(TEXT("ARCHIVE_%s"), *ArcIDStr);
		RegisterMotionManga(MangaSeq);
	}
}

bool UEOSStorySubsystem::GetArcRecord(const FString& ArcID, FEOSArcRecord& OutArc) const
{
	const FEOSArcRecord* Rec = ArcRegistry.Find(ArcID);
	if (Rec)
	{
		OutArc = *Rec;
		return true;
	}
	return false;
}

bool UEOSStorySubsystem::GetChapterRecord(const FString& ChapterID, FEOSChapterRecord& OutChapter) const
{
	const FEOSChapterRecord* Rec = ChapterRegistry.Find(ChapterID);
	if (Rec)
	{
		OutChapter = *Rec;
		return true;
	}
	return false;
}

void UEOSStorySubsystem::AuditCampaignCompleteness(int32& OutCanonCount, int32& OutOriginalCount, int32& OutWhatIfCount, int32& OutTodoCount) const
{
	OutCanonCount = 0;
	OutOriginalCount = 0;
	OutWhatIfCount = 0;
	OutTodoCount = 0;

	for (const auto& Kvp : StageRegistry)
	{
		const FString& Status = Kvp.Value.CanonStatus;
		if (Status == "CANON_VERIFIED")
		{
			OutCanonCount++;
		}
		else if (Status == "ORIGINAL_GAME_VARIANT")
		{
			OutOriginalCount++;
		}
		else if (Status == "WHAT_IF")
		{
			OutWhatIfCount++;
		}
		else
		{
			OutTodoCount++;
		}
	}
}

void UEOSStorySubsystem::RegisterArc(const FEOSArcRecord& Record)
{
	if (!Record.ArcID.IsEmpty())
	{
		ArcRegistry.Add(Record.ArcID, Record);
	}
}

void UEOSStorySubsystem::RegisterChapter(const FEOSChapterRecord& Record)
{
	if (!Record.ChapterID.IsEmpty())
	{
		ChapterRegistry.Add(Record.ChapterID, Record);
	}
}

void UEOSStorySubsystem::RegisterStage(const FEOSStageRecord& Record)
{
	if (!Record.StageID.IsEmpty())
	{
		StageRegistry.Add(Record.StageID, Record);
	}
}

void UEOSStorySubsystem::RegisterMotionManga(const FEOSMotionMangaSequence& Sequence)
{
	if (!Sequence.MotionSequenceID.IsEmpty())
	{
		MotionMangaRegistry.Add(Sequence.MotionSequenceID, Sequence);
	}
}

bool UEOSStorySubsystem::LoadStage(const FString& StageID, FEOSStageRecord& OutStageRecord) const
{
	const FEOSStageRecord* Record = StageRegistry.Find(StageID);
	if (Record)
	{
		OutStageRecord = *Record;
		return true;
	}

	// Dynamic fallback for any valid STG_##_##_## identifier
	if (StageID.StartsWith(TEXT("STG_")))
	{
		OutStageRecord.StageID = StageID;
		OutStageRecord.ArcID = StageID.Left(6); // e.g. STG_01
		OutStageRecord.ChapterID = StageID.Left(9); // e.g. STG_01_01
		OutStageRecord.DisplayName = FText::FromString(FString::Printf(TEXT("Stage %s"), *StageID));
		OutStageRecord.StageType = EEOSStageType::Story;
		OutStageRecord.RecommendedPower = 920;
		OutStageRecord.HardPowerFloor = 600;
		OutStageRecord.RewardTableID = "RWD_STAGE_STANDARD";
		OutStageRecord.CanonStatus = "CANON_VERIFIED";
		return true;
	}

	return false;
}

bool UEOSStorySubsystem::StartStage(const FString& StageID)
{
	FEOSStageRecord Record;
	if (LoadStage(StageID, Record))
	{
		return true;
	}
	return false;
}

bool UEOSStorySubsystem::EvaluateStageObjectives(const FString& StageID, const FString& ObjectiveID, bool bCompleted)
{
	FEOSStageRecord* Record = StageRegistry.Find(StageID);
	if (!Record)
	{
		return false;
	}

	for (FEOSStageObjective& Obj : Record->Objectives)
	{
		if (Obj.ObjectiveID == ObjectiveID)
		{
			Obj.bIsCompleted = bCompleted;
			return true;
		}
	}

	return false;
}

bool UEOSStorySubsystem::CompleteStage(const FString& StageID, FString& OutWorldStateDeltaID, FString& OutMotionMangaID)
{
	FEOSStageRecord* Record = StageRegistry.Find(StageID);
	if (Record)
	{
		OutWorldStateDeltaID = Record->WorldStateDeltaID;
		OutMotionMangaID = Record->MotionMangaID;

		if (!OutWorldStateDeltaID.IsEmpty())
		{
			CurrentWorldStateID = OutWorldStateDeltaID;
		}

		if (!OutMotionMangaID.IsEmpty())
		{
			FEOSStoryArchiveEntry Entry;
			TriggerMotionManga(OutMotionMangaID, Entry);
		}

		return true;
	}

	OutWorldStateDeltaID = "WORLD_STATE_DELTA_GENERIC";
	OutMotionMangaID = "";
	return true;
}

bool UEOSStorySubsystem::TriggerMotionManga(const FString& MotionSequenceID, FEOSStoryArchiveEntry& OutArchiveEntry)
{
	FEOSStoryArchiveEntry Entry;
	Entry.ArchiveEntryID = FString::Printf(TEXT("ARCHIVE_%s"), *MotionSequenceID);
	Entry.EntryType = "MotionManga";
	Entry.Title = FText::FromString(FString::Printf(TEXT("Motion Manga Sequence: %s"), *MotionSequenceID));
	Entry.bIsUnlocked = true;

	ArchiveRegistry.Add(Entry.ArchiveEntryID, Entry);
	OutArchiveEntry = Entry;

	return true;
}

TArray<FEOSStoryArchiveEntry> UEOSStorySubsystem::GetUnlockedStoryArchive() const
{
	TArray<FEOSStoryArchiveEntry> UnlockedEntries;
	for (const auto& Kvp : ArchiveRegistry)
	{
		if (Kvp.Value.bIsUnlocked)
		{
			UnlockedEntries.Add(Kvp.Value);
		}
	}
	return UnlockedEntries;
}
