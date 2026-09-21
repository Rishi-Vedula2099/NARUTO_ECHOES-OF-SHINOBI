#include "../../Public/Subsystems/EOSGameModeSubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSGameModeSubsystem::UEOSGameModeSubsystem()
{
}

void UEOSGameModeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ModeRegistry.Empty();
	ProgressRegistry.Empty();
	BountyRegistry.Empty();
	RunResultRegistry.Empty();
	CurrentState = EEOSModeState::Available;
	RunCounter = 0;

	InitializeFullModeDatabase();
}

void UEOSGameModeSubsystem::Deinitialize()
{
	ModeRegistry.Empty();
	ProgressRegistry.Empty();
	BountyRegistry.Empty();
	RunResultRegistry.Empty();
	Super::Deinitialize();
}

void UEOSGameModeSubsystem::InitializeFullModeDatabase()
{
	ModeRegistry.Empty();
	ProgressRegistry.Empty();
	BountyRegistry.Empty();
	RunResultRegistry.Empty();

	struct FModeDef
	{
		const char* ModeID;
		const char* DisplayName;
		const char* Description;
		EEOSGameModeCategory Category;
		EEOSGameModeType ModeType;
		int32 RecPower;
		float TimeLimit;
		const char* CanonStatus;
	};

	const FModeDef MasterModeList[] = {
		// STORY (4)
		{ "MODE_STORY_CAMPAIGN", "Story Campaign", "Main 25-arc narrative campaign spanning 500 core stages.", EEOSGameModeCategory::Story, EEOSGameModeType::StoryCampaign, 1000, 600.0f, "CANON_VERIFIED" },
		{ "MODE_SHINOBI_CHRONICLES", "Shinobi Chronicles", "Playable side stories exploring companion backstory and bond growth.", EEOSGameModeCategory::Story, EEOSGameModeType::ShinobiChronicles, 1200, 480.0f, "CANON_VERIFIED" },
		{ "MODE_LEGACY_ARCHIVE", "Legacy Archive", "Narrative preservation repository and cutscene replay hub.", EEOSGameModeCategory::Story, EEOSGameModeType::LegacyArchive, 0, 0.0f, "CANON_VERIFIED" },
		{ "MODE_MOTION_MANGA", "Motion Manga Presentation", "2.5D parallax manga cutscene playback following major boss victories.", EEOSGameModeCategory::Story, EEOSGameModeType::MotionManga, 0, 0.0f, "CANON_VERIFIED" },

		// TRAINING (9)
		{ "MODE_TRAINING_ARENA", "Free Training Arena", "Unrestricted combat sandbox with infinite chakra and dummy practice.", EEOSGameModeCategory::Training, EEOSGameModeType::TrainingArena, 0, 0.0f, "CANON_VERIFIED" },
		{ "MODE_GENIN_TRAINING", "Genin Fundamentals", "Core movement, basic attacks, guarding, and Kawarimi substitution.", EEOSGameModeCategory::Training, EEOSGameModeType::GeninTraining, 600, 300.0f, "CANON_VERIFIED" },
		{ "MODE_CHUNIN_TRAINING", "Chunin Tactical Combos", "Combo branching, perfect dodges, and 5-nature elemental interactions.", EEOSGameModeCategory::Training, EEOSGameModeType::ChuninTraining, 1200, 300.0f, "CANON_VERIFIED" },
		{ "MODE_JONIN_TRAINING", "Jonin Advanced Mastery", "Cancel windows, boss telegraph parsing, and support assist timing.", EEOSGameModeCategory::Training, EEOSGameModeType::JoninTraining, 2500, 300.0f, "CANON_VERIFIED" },
		{ "MODE_SAGE_TRAINING", "Sage Mode Balance Trials", "Nature energy gauge balancing atop needle spires.", EEOSGameModeCategory::Training, EEOSGameModeType::SageTraining, 3500, 180.0f, "CANON_VERIFIED" },
		{ "MODE_JUTSU_ARENA", "Jutsu Practice Arena", "Hand-sign execution trials, timing refinement, and mastery progression.", EEOSGameModeCategory::Training, EEOSGameModeType::JutsuArena, 800, 240.0f, "CANON_VERIFIED" },
		{ "MODE_CHAKRA_TRIALS", "Chakra Control Trials", "Precision chakra output regulation and overcharge avoidance.", EEOSGameModeCategory::Training, EEOSGameModeType::ChakraTrials, 1000, 180.0f, "CANON_VERIFIED" },
		{ "MODE_MENTOR_DOJO", "Mentor Dojo Challenges", "Character-specific trials assigned by legendary mentors.", EEOSGameModeCategory::Training, EEOSGameModeType::MentorDojo, 2000, 360.0f, "CANON_VERIFIED" },
		{ "MODE_BEAST_RESONANCE", "Tailed Beast Resonance Trials", "Jinchūriki chakra cloak stability and beast jutsu control.", EEOSGameModeCategory::Training, EEOSGameModeType::BeastResonance, 4000, 240.0f, "CANON_VERIFIED" },

		// CHALLENGE (10)
		{ "MODE_SHINOBI_TOWER", "Shinobi Tower Floor Climb", "Vertical 100-floor combat ascent with stacking environmental modifiers.", EEOSGameModeCategory::Challenge, EEOSGameModeType::ShinobiTower, 3000, 300.0f, "CANON_INSPIRED" },
		{ "MODE_SURVIVAL_WAVES", "Survival Enemy Waves", "Escalating waves of minions, elites, and mini-bosses in hazard arenas.", EEOSGameModeCategory::Challenge, EEOSGameModeType::SurvivalWaves, 2200, 420.0f, "CANON_INSPIRED" },
		{ "MODE_BOSS_RUSH", "Sequential Boss Rush", "Consecutive multi-phase boss battles with limited recovery items.", EEOSGameModeCategory::Challenge, EEOSGameModeType::BossRush, 4500, 600.0f, "CANON_INSPIRED" },
		{ "MODE_BOSS_REMATCH", "Adaptive Boss Rematch Memory", "Replaying story bosses enhanced with bounded adaptive AI memories.", EEOSGameModeCategory::Challenge, EEOSGameModeType::BossRematch, 1800, 360.0f, "CANON_VERIFIED" },
		{ "MODE_TIME_TRIALS", "Time Trial Speedrun", "Fixed traversal speedruns across Konoha rooftops and outer lands.", EEOSGameModeCategory::Challenge, EEOSGameModeType::TimeTrial, 1500, 180.0f, "CANON_INSPIRED" },
		{ "MODE_NINJA_RELAY", "Ninja Traversal Relay", "Continuous checkpoint parkour seamlessly linked to combat pockets.", EEOSGameModeCategory::Challenge, EEOSGameModeType::NinjaRelay, 1400, 240.0f, "CANON_INSPIRED" },
		{ "MODE_TEAM_SYNERGY", "Team Synergy Trials", "Trio team combat emphasizing bond assists and elemental combos.", EEOSGameModeCategory::Challenge, EEOSGameModeType::TeamSynergy, 2000, 300.0f, "CANON_VERIFIED" },
		{ "MODE_PERFECT_MISSION", "Flawless Perfect Mission", "High-skill trial requiring zero damage taken and maximum combo chains.", EEOSGameModeCategory::Challenge, EEOSGameModeType::PerfectMission, 3500, 240.0f, "CANON_INSPIRED" },
		{ "MODE_ROGUE_BOARD", "Rogue Mission Board", "Procedurally assembled mission templates with unpredictable bounties.", EEOSGameModeCategory::Challenge, EEOSGameModeType::RogueBoard, 1800, 360.0f, "ORIGINAL_GAME_VARIANT" },
		{ "MODE_DIMENSIONAL_RIFT", "Dimensional Rift Alternate Trial", "Original game mode featuring alternate space-time combat rules.", EEOSGameModeCategory::Challenge, EEOSGameModeType::DimensionalRift, 5000, 300.0f, "ORIGINAL_GAME_VARIANT" },

		// LIVE (5)
		{ "MODE_WEEKLY_BOUNTY", "Weekly Seeded Bounty Hunt", "Server-seeded Thursday 00:00 UTC target hunt with unique modifiers.", EEOSGameModeCategory::Live, EEOSGameModeType::WeeklyBounty, 1400, 360.0f, "CANON_VERIFIED" },
		{ "MODE_LIMITED_EVENTS", "Limited Character Release Event", "Special story arc and challenge missions for flagship characters.", EEOSGameModeCategory::Live, EEOSGameModeType::LimitedEvent, 2500, 480.0f, "CANON_INSPIRED" },
		{ "MODE_LIMITED_TRIALS", "Limited Character Test Trial", "Trial stage to experience a limited character's complete ability kit.", EEOSGameModeCategory::Live, EEOSGameModeType::LimitedTrial, 1000, 300.0f, "CANON_VERIFIED" },
		{ "MODE_VILLAGE_DEFENSE", "Village Invasion Defense", "Dynamic invasion wave defense affecting Konoha's world-state.", EEOSGameModeCategory::Live, EEOSGameModeType::VillageDefense, 3000, 420.0f, "CANON_VERIFIED" },
		{ "MODE_ROTATING_CHALLENGE", "Rotating Condition Challenge", "Daily rotating modifiers and specialized objective constraints.", EEOSGameModeCategory::Live, EEOSGameModeType::RotatingChallenge, 1600, 240.0f, "CANON_INSPIRED" },

		// WORLD (4)
		{ "MODE_WORLD_EXPLORATION", "Free-Roam World Exploration", "32 interconnected 3D zones with vertical traversal and lore props.", EEOSGameModeCategory::World, EEOSGameModeType::WorldExploration, 600, 0.0f, "CANON_VERIFIED" },
		{ "MODE_EXPEDITIONS", "Regional Expedition & Extraction", "Resource-gathering journeys through foreign lands and wilderness.", EEOSGameModeCategory::World, EEOSGameModeType::Expedition, 1500, 600.0f, "CANON_INSPIRED" },
		{ "MODE_SCROLL_HUNT", "Hidden Lore Scroll Hunt", "Open-world search for ancient ninjutsu scrolls and clan relics.", EEOSGameModeCategory::World, EEOSGameModeType::ScrollHunt, 800, 0.0f, "CANON_VERIFIED" },
		{ "MODE_WORLD_EVENTS", "Dynamic World Events", "Spontaneous village events and traveling merchant festivals.", EEOSGameModeCategory::World, EEOSGameModeType::WorldEvent, 1000, 300.0f, "CANON_INSPIRED" }
	};

	const int32 TotalModes = sizeof(MasterModeList) / sizeof(MasterModeList[0]);
	for (int32 i = 0; i < TotalModes; ++i)
	{
		const FModeDef& MD = MasterModeList[i];
		FEOSGameModeRecord Record;
		Record.ModeID = FString(UTF8_TO_TCHAR(MD.ModeID));
		Record.DisplayName = FText::FromString(FString(UTF8_TO_TCHAR(MD.DisplayName)));
		Record.Description = FText::FromString(FString(UTF8_TO_TCHAR(MD.Description)));
		Record.Category = MD.Category;
		Record.ModeType = MD.ModeType;
		Record.RecommendedPower = MD.RecPower;
		Record.MinimumPower = FMath::Max(600, MD.RecPower - 400);
		Record.TimeLimit = MD.TimeLimit;
		Record.CanonStatus = FString(UTF8_TO_TCHAR(MD.CanonStatus));
		Record.bEnabled = true;
		Record.bReplayable = true;
		Record.bScoreEnabled = true;

		// Default Primary Objective
		FEOSModeObjective PrimObj;
		PrimObj.ObjectiveID = FString::Printf(TEXT("OBJ_%s_PRIMARY"), *Record.ModeID);
		PrimObj.DisplayName = FText::FromString(TEXT("Complete Mode Target"));
		PrimObj.Description = FText::FromString(TEXT("Fulfill all designated combat and stage criteria."));
		PrimObj.TargetCount = 1;
		PrimObj.bIsOptional = false;
		PrimObj.ScoreBonus = 500;
		Record.Objectives.Add(PrimObj);

		// Optional Bonus Objective
		FEOSModeObjective OptObj;
		OptObj.ObjectiveID = FString::Printf(TEXT("OBJ_%s_OPTIONAL"), *Record.ModeID);
		OptObj.DisplayName = FText::FromString(TEXT("Perform 5 Perfect Dodges"));
		OptObj.Description = FText::FromString(TEXT("Evade incoming hostile strikes with precise timing."));
		OptObj.TargetCount = 5;
		OptObj.bIsOptional = true;
		OptObj.ScoreBonus = 250;
		Record.Objectives.Add(OptObj);

		// First Clear Reward
		FEOSModeRewardEntry FirstClear;
		FirstClear.RewardType = EEOSModeRewardType::CharacterEXP;
		FirstClear.ItemID = "RWD_EXP_LARGE";
		FirstClear.Quantity = 5000;
		FirstClear.bFirstClearOnly = true;
		Record.Rewards.Add(FirstClear);

		// Repeatable Reward
		FEOSModeRewardEntry RepeatClear;
		RepeatClear.RewardType = EEOSModeRewardType::Material;
		RepeatClear.ItemID = "MAT_CHAKRA_CRYSTAL";
		RepeatClear.Quantity = 10;
		RepeatClear.bFirstClearOnly = false;
		Record.Rewards.Add(RepeatClear);

		RegisterGameMode(Record);
	}

	// Author Active Weekly Bounty (Thursday 00:00 UTC Seed)
	FEOSBountyDefinition Bounty;
	Bounty.BountyID = "BOUNTY_WEEK_01_ZABUZA";
	Bounty.TargetID = "BOSS_ZABUZA";
	Bounty.DisplayName = FText::FromString("Weekly Bounty: Zabuza Momochi (Demon of the Mist)");
	Bounty.TargetDescription = FText::FromString("Neutralize Zabuza Momochi under extreme fog conditions on the Great Naruto Bridge.");
	Bounty.LocationID = "ZONE_WAVES_BRIDGE";
	Bounty.ActiveModifierID = "MOD_MIST_DENSITY";
	Bounty.BonusObjectiveID = "OBJ_PERFECT_DODGE_8";
	Bounty.RecommendedPower = 1400;
	Bounty.ResetSchedule = "Thursday 00:00 UTC";
	Bounty.bIsClaimedThisWeek = false;

	FEOSModeRewardEntry BountyReward1;
	BountyReward1.RewardType = EEOSModeRewardType::Material;
	BountyReward1.ItemID = "MAT_HIGH_GRADE_WATER_ESSENCE";
	BountyReward1.Quantity = 5;
	BountyReward1.bFirstClearOnly = true;
	Bounty.Rewards.Add(BountyReward1);

	FEOSModeRewardEntry BountyReward2;
	BountyReward2.RewardType = EEOSModeRewardType::Gear;
	BountyReward2.ItemID = "RECIPE_EXECUTIONER_BLADE";
	BountyReward2.Quantity = 1;
	BountyReward2.bFirstClearOnly = true;
	Bounty.Rewards.Add(BountyReward2);

	RegisterWeeklyBounty(Bounty);
}

void UEOSGameModeSubsystem::RegisterGameMode(const FEOSGameModeRecord& Record)
{
	if (!Record.ModeID.IsEmpty())
	{
		ModeRegistry.Add(Record.ModeID, Record);

		// Initialize progress tracking if not present
		if (!ProgressRegistry.Contains(Record.ModeID))
		{
			FEOSModeProgress Progress;
			Progress.ModeID = Record.ModeID;
			Progress.bFirstClearClaimed = false;
			Progress.TotalClears = 0;
			ProgressRegistry.Add(Record.ModeID, Progress);
		}
	}
}

bool UEOSGameModeSubsystem::GetGameMode(const FString& ModeID, FEOSGameModeRecord& OutRecord) const
{
	const FEOSGameModeRecord* Found = ModeRegistry.Find(ModeID);
	if (Found)
	{
		OutRecord = *Found;
		return true;
	}
	return false;
}

TArray<FEOSGameModeRecord> UEOSGameModeSubsystem::GetModesByCategory(EEOSGameModeCategory Category) const
{
	TArray<FEOSGameModeRecord> Results;
	for (const auto& Kvp : ModeRegistry)
	{
		if (Kvp.Value.Category == Category)
		{
			Results.Add(Kvp.Value);
		}
	}
	return Results;
}

bool UEOSGameModeSubsystem::StartModeRun(const FString& ModeID, FString& OutRunID)
{
	if (!ModeRegistry.Contains(ModeID))
	{
		return false;
	}

	RunCounter++;
	ActiveRunID = FString::Printf(TEXT("RUN_%s_%05d"), *ModeID, RunCounter);
	ActiveModeID = ModeID;
	CurrentState = EEOSModeState::Active;
	OutRunID = ActiveRunID;

	return true;
}

bool UEOSGameModeSubsystem::PauseModeRun()
{
	if (CurrentState == EEOSModeState::Active)
	{
		CurrentState = EEOSModeState::Paused;
		return true;
	}
	return false;
}

bool UEOSGameModeSubsystem::ResumeModeRun()
{
	if (CurrentState == EEOSModeState::Paused)
	{
		CurrentState = EEOSModeState::Active;
		return true;
	}
	return false;
}

bool UEOSGameModeSubsystem::CompleteModeRun(const FString& RunID, float ClearTime, int32 DamageTaken, int32 MaxCombo, int32 PerfectDodges, FEOSModeRunResult& OutResult)
{
	if (CurrentState != EEOSModeState::Active && CurrentState != EEOSModeState::Paused)
	{
		return false;
	}

	OutResult.RunID = RunID;
	OutResult.ModeID = ActiveModeID;
	OutResult.bIsCleared = true;
	OutResult.ClearTime = ClearTime;
	OutResult.DamageTaken = DamageTaken;
	OutResult.MaxCombo = MaxCombo;
	OutResult.PerfectDodges = PerfectDodges;
	OutResult.CompletedObjectiveCount = 2; // Primary + Optional completed in successful run

	FString Grade;
	OutResult.FinalScore = CalculateRunScore(ClearTime, DamageTaken, MaxCombo, PerfectDodges, OutResult.CompletedObjectiveCount, Grade);
	OutResult.CombatGrade = Grade;

	// Update persistent progress
	if (FEOSModeProgress* Prog = ProgressRegistry.Find(ActiveModeID))
	{
		Prog->TotalClears++;
		if (OutResult.FinalScore > Prog->HighestScore)
		{
			Prog->HighestScore = OutResult.FinalScore;
		}
		if (ClearTime < Prog->BestTime)
		{
			Prog->BestTime = ClearTime;
		}
	}

	RunResultRegistry.Add(RunID, OutResult);
	CurrentState = EEOSModeState::RewardPending;
	return true;
}

bool UEOSGameModeSubsystem::FailModeRun(const FString& RunID, const FString& FailureReason)
{
	CurrentState = EEOSModeState::Failed;
	return true;
}

bool UEOSGameModeSubsystem::EvaluateObjective(const FString& ObjectiveID, int32 ProgressCount)
{
	if (FEOSGameModeRecord* Record = ModeRegistry.Find(ActiveModeID))
	{
		for (FEOSModeObjective& Obj : Record->Objectives)
		{
			if (Obj.ObjectiveID == ObjectiveID)
			{
				Obj.CurrentCount = ProgressCount;
				Obj.bIsCompleted = (Obj.CurrentCount >= Obj.TargetCount);
				return true;
			}
		}
	}
	return false;
}

int32 UEOSGameModeSubsystem::CalculateRunScore(float ClearTime, int32 DamageTaken, int32 MaxCombo, int32 PerfectDodges, int32 CompletedObjectives, FString& OutGrade) const
{
	int32 BaseScore = 1000;
	int32 ObjectiveBonus = CompletedObjectives * 250;
	int32 CombatScore = (MaxCombo * 15) + (PerfectDodges * 50);
	int32 TimeBonus = FMath::Max(0, static_cast<int32>((300.0f - ClearTime) * 10.0f));
	int32 DamagePenalty = static_cast<int32>(DamageTaken * 0.25f);

	int32 Total = FMath::Max(100, BaseScore + ObjectiveBonus + CombatScore + TimeBonus - DamagePenalty);

	if (Total >= 2000)
	{
		OutGrade = "S";
	}
	else if (Total >= 1500)
	{
		OutGrade = "A";
	}
	else if (Total >= 1000)
	{
		OutGrade = "B";
	}
	else
	{
		OutGrade = "C";
	}

	return Total;
}

bool UEOSGameModeSubsystem::ClaimRunRewards(const FString& RunID, TArray<FEOSModeRewardEntry>& OutGrantedRewards)
{
	FEOSModeRunResult* RunRes = RunResultRegistry.Find(RunID);
	if (!RunRes || CurrentState != EEOSModeState::RewardPending)
	{
		return false;
	}

	FEOSGameModeRecord* Record = ModeRegistry.Find(RunRes->ModeID);
	FEOSModeProgress* Prog = ProgressRegistry.Find(RunRes->ModeID);

	if (!Record || !Prog)
	{
		return false;
	}

	OutGrantedRewards.Empty();
	for (const FEOSModeRewardEntry& Entry : Record->Rewards)
	{
		// Idempotent first-clear check
		if (Entry.bFirstClearOnly)
		{
			if (!Prog->bFirstClearClaimed)
			{
				OutGrantedRewards.Add(Entry);
			}
		}
		else
		{
			OutGrantedRewards.Add(Entry);
		}
	}

	Prog->bFirstClearClaimed = true;
	RunRes->GrantedRewards = OutGrantedRewards;
	CurrentState = EEOSModeState::RewardClaimed;

	return true;
}

bool UEOSGameModeSubsystem::GetModeProgress(const FString& ModeID, FEOSModeProgress& OutProgress) const
{
	const FEOSModeProgress* Found = ProgressRegistry.Find(ModeID);
	if (Found)
	{
		OutProgress = *Found;
		return true;
	}
	return false;
}

void UEOSGameModeSubsystem::RegisterWeeklyBounty(const FEOSBountyDefinition& Bounty)
{
	if (!Bounty.BountyID.IsEmpty())
	{
		BountyRegistry.Add(Bounty.BountyID, Bounty);
	}
}

bool UEOSGameModeSubsystem::GetActiveWeeklyBounty(FEOSBountyDefinition& OutBounty) const
{
	const FEOSBountyDefinition* Found = BountyRegistry.Find(ActiveBountyID);
	if (Found)
	{
		OutBounty = *Found;
		return true;
	}
	return false;
}

bool UEOSGameModeSubsystem::ClaimBountyReward(const FString& BountyID, TArray<FEOSModeRewardEntry>& OutRewards)
{
	FEOSBountyDefinition* Found = BountyRegistry.Find(BountyID);
	if (Found && !Found->bIsClaimedThisWeek)
	{
		Found->bIsClaimedThisWeek = true;
		OutRewards = Found->Rewards;
		return true;
	}
	return false;
}

bool UEOSGameModeSubsystem::LoadBossRematchMemory(const FString& BossID, FString& OutMemoryProfileID) const
{
	if (BossID == "BOSS_ZABUZA")
	{
		OutMemoryProfileID = "MEMORY_BOSS_ZABUZA_REMATCH";
		return true;
	}
	else if (BossID == "BOSS_PAIN")
	{
		OutMemoryProfileID = "MEMORY_BOSS_PAIN_REMATCH";
		return true;
	}
	else if (BossID == "BOSS_MADARA")
	{
		OutMemoryProfileID = "MEMORY_BOSS_MADARA_REMATCH";
		return true;
	}

	OutMemoryProfileID = FString::Printf(TEXT("MEMORY_%s_DEFAULT"), *BossID);
	return true;
}

void UEOSGameModeSubsystem::AuditGameModeCoverage(int32& OutStoryCount, int32& OutTrainingCount, int32& OutChallengeCount, int32& OutLiveCount, int32& OutWorldCount) const
{
	OutStoryCount = 0;
	OutTrainingCount = 0;
	OutChallengeCount = 0;
	OutLiveCount = 0;
	OutWorldCount = 0;

	for (const auto& Kvp : ModeRegistry)
	{
		switch (Kvp.Value.Category)
		{
		case EEOSGameModeCategory::Story:
			OutStoryCount++;
			break;
		case EEOSGameModeCategory::Training:
			OutTrainingCount++;
			break;
		case EEOSGameModeCategory::Challenge:
			OutChallengeCount++;
			break;
		case EEOSGameModeCategory::Live:
			OutLiveCount++;
			break;
		case EEOSGameModeCategory::World:
			OutWorldCount++;
			break;
		}
	}
}
