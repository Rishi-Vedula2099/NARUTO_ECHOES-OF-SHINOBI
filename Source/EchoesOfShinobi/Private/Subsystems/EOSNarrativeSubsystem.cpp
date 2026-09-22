#include "../../Public/Subsystems/EOSNarrativeSubsystem.h"
#include "../../EchoesOfShinobi.h"

UEOSNarrativeSubsystem::UEOSNarrativeSubsystem()
{
}

void UEOSNarrativeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultNarrativeDatabase();
	UE_LOG(LogEOSCore, Log, TEXT("UEOSNarrativeSubsystem Initialized. Master Narrative State Machine Ready."));
}

void UEOSNarrativeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSNarrativeSubsystem::InitializeDefaultNarrativeDatabase()
{
	// 1. Register Default HUD Transition Profiles
	FEOSNarrativeTransitionProfile DefaultProfile;
	DefaultProfile.ProfileID = TEXT("TRANS_PROFILE_DEFAULT");
	DefaultProfile.BossUIFadeTime = 0.10f;
	DefaultProfile.ObjectiveUIFadeTime = 0.25f;
	DefaultProfile.SkillUIFadeTime = 0.40f;
	DefaultProfile.MinimapFadeTime = 0.50f;
	DefaultProfile.FullHUDHiddenTime = 0.60f;
	DefaultProfile.NarrativeStartTime = 0.80f;
	DefaultProfile.bLetterboxEnabled = true;
	DefaultProfile.CameraBlendDuration = 1.0f;
	RegisterTransitionProfile(DefaultProfile);

	FEOSNarrativeTransitionProfile ClimaxProfile;
	ClimaxProfile.ProfileID = TEXT("TRANS_PROFILE_CLIMAX");
	ClimaxProfile.BossUIFadeTime = 0.05f;
	ClimaxProfile.ObjectiveUIFadeTime = 0.15f;
	ClimaxProfile.SkillUIFadeTime = 0.30f;
	ClimaxProfile.MinimapFadeTime = 0.40f;
	ClimaxProfile.FullHUDHiddenTime = 0.50f;
	ClimaxProfile.NarrativeStartTime = 0.70f;
	ClimaxProfile.bLetterboxEnabled = true;
	ClimaxProfile.CameraBlendDuration = 0.8f;
	RegisterTransitionProfile(ClimaxProfile);

	// 2. Register Default Audio & Music Transition Records
	FEOSMusicTransitionRecord BossTriumph;
	BossTriumph.TransitionID = TEXT("MUS_TRANS_BOSS_TO_TRIUMPH");
	BossTriumph.FromMusicState = TEXT("COMBAT_TRACK");
	BossTriumph.ToMusicState = TEXT("NARRATIVE_TRIUMPH");
	BossTriumph.FadeOutDuration = 0.3f;
	BossTriumph.CrossfadeDuration = 1.2f;
	BossTriumph.StingerID = TEXT("STINGER_BOSS_DEFEAT_01");
	BossTriumph.Priority = 100;
	RegisterMusicTransition(BossTriumph);

	FEOSMusicTransitionRecord BossMelancholy;
	BossMelancholy.TransitionID = TEXT("MUS_TRANS_BOSS_TO_MELANCHOLY");
	BossMelancholy.FromMusicState = TEXT("COMBAT_TRACK");
	BossMelancholy.ToMusicState = TEXT("NARRATIVE_MELANCHOLY");
	BossMelancholy.FadeOutDuration = 0.4f;
	BossMelancholy.CrossfadeDuration = 1.5f;
	BossMelancholy.StingerID = TEXT("STINGER_BOSS_DEFEAT_02");
	BossMelancholy.Priority = 110;
	RegisterMusicTransition(BossMelancholy);

	FEOSMusicTransitionRecord ArcEnd;
	ArcEnd.TransitionID = TEXT("MUS_TRANS_ARC_END");
	ArcEnd.FromMusicState = TEXT("COMBAT_TRACK");
	ArcEnd.ToMusicState = TEXT("NARRATIVE_EPIC_CLIMAX");
	ArcEnd.FadeOutDuration = 0.5f;
	ArcEnd.CrossfadeDuration = 2.0f;
	ArcEnd.StingerID = TEXT("STINGER_ARC_COMPLETE");
	ArcEnd.Priority = 200;
	RegisterMusicTransition(ArcEnd);

	// 3. Register Canonical Arc 01 Climax Sequence (Land of Waves Bridge Conclusion)
	FEOSMotionMangaSequenceRecord WaveClimax;
	WaveClimax.SequenceID = TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX");
	WaveClimax.ArcID = TEXT("ARC_01");
	WaveClimax.ChapterID = TEXT("CH_01_04");
	WaveClimax.StageID = TEXT("STG_01_04_05");
	WaveClimax.Title = FText::FromString(TEXT("The Bridge Called Peace"));
	WaveClimax.Subtitle = FText::FromString(TEXT("Naruto's Ninja Way"));
	WaveClimax.MusicID = TEXT("MUS_TRANS_BOSS_TO_MELANCHOLY");
	WaveClimax.ArchiveID = TEXT("ARCHIVE_MM_A01_C04");
	WaveClimax.WorldStateDeltaID = TEXT("DELTA_LAND_OF_WAVES_RESTORED");
	WaveClimax.FinalPanelID = TEXT("MM_A01_C04_P005_FINAL");

	// Panel 1: Falling snow over the Great Naruto Bridge
	FEOSMotionMangaPanel P1;
	P1.PanelID = TEXT("MM_A01_C04_P001");
	P1.SequenceID = WaveClimax.SequenceID;
	P1.PanelIndex = 0;
	P1.ImageAsset = TEXT("Tex_Manga_Wave_P001");
	P1.BackgroundAsset = TEXT("Tex_Manga_Wave_P001_BG");
	P1.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P001_Snow"));
	P1.MotionProfile = EEOSPanelMotionProfile::ZoomIn;
	P1.Duration = 4.0f;
	P1.HoldDuration = 1.0f;
	P1.DialogueID = TEXT("D_A01_C04_P001_01");
	WaveClimax.Panels.Add(P1);

	// Panel 2: Zabuza looking up through fading vision
	FEOSMotionMangaPanel P2;
	P2.PanelID = TEXT("MM_A01_C04_P002");
	P2.SequenceID = WaveClimax.SequenceID;
	P2.PanelIndex = 1;
	P2.ImageAsset = TEXT("Tex_Manga_Wave_P002");
	P2.BackgroundAsset = TEXT("Tex_Manga_Wave_P002_BG");
	P2.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P002_Zabuza"));
	P2.MotionProfile = EEOSPanelMotionProfile::PanLeft;
	P2.Duration = 4.5f;
	P2.DialogueID = TEXT("D_A01_C04_P002_01");
	WaveClimax.Panels.Add(P2);

	// Panel 3: Kakashi laying Zabuza beside Haku
	FEOSMotionMangaPanel P3;
	P3.PanelID = TEXT("MM_A01_C04_P003");
	P3.SequenceID = WaveClimax.SequenceID;
	P3.PanelIndex = 2;
	P3.ImageAsset = TEXT("Tex_Manga_Wave_P003");
	P3.BackgroundAsset = TEXT("Tex_Manga_Wave_P003_BG");
	P3.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P003_Kakashi_Zabuza"));
	P3.MotionProfile = EEOSPanelMotionProfile::DramaticHold;
	P3.Duration = 5.0f;
	P3.DialogueID = TEXT("D_A01_C04_P003_01");
	WaveClimax.Panels.Add(P3);

	// Panel 4: Naruto crying with tears on his face
	FEOSMotionMangaPanel P4;
	P4.PanelID = TEXT("MM_A01_C04_P004");
	P4.SequenceID = WaveClimax.SequenceID;
	P4.PanelIndex = 3;
	P4.ImageAsset = TEXT("Tex_Manga_Wave_P004");
	P4.BackgroundAsset = TEXT("Tex_Manga_Wave_P004_BG");
	P4.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P004_Naruto"));
	P4.MotionProfile = EEOSPanelMotionProfile::ZoomIn;
	P4.Duration = 4.0f;
	P4.DialogueID = TEXT("D_A01_C04_P004_01");
	WaveClimax.Panels.Add(P4);

	// Panel 5: Final Climax Panel - Sunrise over the bridge
	FEOSMotionMangaPanel P5;
	P5.PanelID = WaveClimax.FinalPanelID;
	P5.SequenceID = WaveClimax.SequenceID;
	P5.PanelIndex = 4;
	P5.ImageAsset = TEXT("Tex_Manga_Wave_P005_Final");
	P5.BackgroundAsset = TEXT("Tex_Manga_Wave_P005_Sunrise");
	P5.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P005_Team7"));
	P5.MotionProfile = EEOSPanelMotionProfile::DramaticHold;
	P5.Duration = 6.0f;
	P5.HoldDuration = 2.5f;
	P5.bIsFinalPanel = true;
	P5.ArchiveUnlockID = WaveClimax.ArchiveID;
	P5.DialogueID = TEXT("D_A01_C04_P005_01");
	WaveClimax.Panels.Add(P5);

	RegisterSequence(WaveClimax);
}

bool UEOSNarrativeSubsystem::StartMissionCompleteSequence(const FEOSStageResult& StageResult)
{
	if (!StageResult.bIsSuccess)
	{
		UE_LOG(LogEOSCore, Warning, TEXT("[Narrative] Cannot start sequence: Stage Result is not SUCCESS."));
		return false;
	}

	ActiveStageID = StageResult.StageID;
	ActiveSequenceID = StageResult.NarrativeSequenceID.IsEmpty() ? TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX") : StageResult.NarrativeSequenceID;
	ActiveWorldStateDeltaID = StageResult.WorldStateDeltaID;
	ActiveTransactionID = StageResult.TransactionID.IsEmpty() ? FString::Printf(TEXT("TX_%s_%s"), *ActiveStageID, *ActiveSequenceID) : StageResult.TransactionID;

	// Initialize or load transaction
	FEOSNarrativeTransaction Tx;
	Tx.TransactionID = ActiveTransactionID;
	Tx.StageID = ActiveStageID;
	Tx.SequenceID = ActiveSequenceID;
	Tx.bStageResultLocked = true;
	Tx.bNarrativeStarted = true;
	TransactionLedger.Add(ActiveTransactionID, Tx);

	TransitionToState(EEOSNarrativeState::MissionComplete);
	return true;
}

bool UEOSNarrativeSubsystem::TriggerBossDefeatHandoff(const FString& BossID)
{
	TransitionToState(EEOSNarrativeState::BossDefeated);
	UE_LOG(LogEOSCore, Log, TEXT("[Narrative] Boss Defeated: %s. Combat lock engaged."), *BossID);
	return true;
}

bool UEOSNarrativeSubsystem::TransitionToState(EEOSNarrativeState NewState)
{
	if (CurrentState == NewState)
	{
		return true;
	}

	CurrentState = NewState;
	if (OnNarrativeStateChanged.IsBound())
	{
		OnNarrativeStateChanged.Broadcast(CurrentState);
	}
	return true;
}

bool UEOSNarrativeSubsystem::AdvanceToNextStep()
{
	switch (CurrentState)
	{
	case EEOSNarrativeState::BossDefeated:
		return TransitionToState(EEOSNarrativeState::MissionComplete);

	case EEOSNarrativeState::MissionComplete:
		return TransitionToState(EEOSNarrativeState::HUDFade);

	case EEOSNarrativeState::HUDFade:
		return TransitionToState(EEOSNarrativeState::MusicTransition);

	case EEOSNarrativeState::MusicTransition:
		return TransitionToState(EEOSNarrativeState::NarrativeLoading);

	case EEOSNarrativeState::NarrativeLoading:
		return TransitionToState(EEOSNarrativeState::MotionMangaStart);

	case EEOSNarrativeState::MotionMangaStart:
		return TransitionToState(EEOSNarrativeState::PanelActive);

	case EEOSNarrativeState::PanelActive:
		// Advance to final panel
		return TransitionToState(EEOSNarrativeState::FinalPanel);

	case EEOSNarrativeState::FinalPanel:
		return TransitionToState(EEOSNarrativeState::ArcComplete);

	case EEOSNarrativeState::ChapterComplete:
	case EEOSNarrativeState::ArcComplete:
		return TransitionToState(EEOSNarrativeState::RewardPresentation);

	case EEOSNarrativeState::RewardPresentation:
		return TransitionToState(EEOSNarrativeState::ArchiveUpdate);

	case EEOSNarrativeState::ArchiveUpdate:
		return TransitionToState(EEOSNarrativeState::WorldStateUpdate);

	case EEOSNarrativeState::WorldStateUpdate:
		return TransitionToState(EEOSNarrativeState::NarrativeComplete);

	case EEOSNarrativeState::NarrativeComplete:
	{
		// Mark transaction fully completed
		if (TransactionLedger.Contains(ActiveTransactionID))
		{
			TransactionLedger[ActiveTransactionID].bNarrativeCompleted = true;
			TransactionLedger[ActiveTransactionID].bCompleted = true;
		}

		if (OnNarrativeSequenceCompleted.IsBound())
		{
			OnNarrativeSequenceCompleted.Broadcast(ActiveSequenceID);
		}
		return TransitionToState(EEOSNarrativeState::ReturnToGame);
	}

	case EEOSNarrativeState::ReturnToGame:
		RestorePreNarrativeInputContext();
		return TransitionToState(EEOSNarrativeState::None);

	default:
		break;
	}

	return false;
}

bool UEOSNarrativeSubsystem::SkipSequence()
{
	if (CurrentState == EEOSNarrativeState::None || CurrentState == EEOSNarrativeState::ReturnToGame)
	{
		return false;
	}

	// Guaranteed idempotent finalize: rewards, archive, and world-state MUST still be committed!
	UnlockStoryArchiveEntry(TEXT("ARCHIVE_") + ActiveSequenceID, ActiveSequenceID);
	CommitWorldStateDelta(ActiveWorldStateDeltaID);

	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		TransactionLedger[ActiveTransactionID].bNarrativeCompleted = true;
		TransactionLedger[ActiveTransactionID].bCompleted = true;
	}

	TransitionToState(EEOSNarrativeState::Skipped);
	RestorePreNarrativeInputContext();
	TransitionToState(EEOSNarrativeState::ReturnToGame);
	return true;
}

bool UEOSNarrativeSubsystem::PauseSequence()
{
	bIsPaused = true;
	return true;
}

bool UEOSNarrativeSubsystem::ResumeSequence()
{
	bIsPaused = false;
	return true;
}

void UEOSNarrativeSubsystem::SetPlaybackSpeed(float Speed)
{
	ActivePlaybackSpeed = FMath::Clamp(Speed, 0.5f, 2.0f);
}

void UEOSNarrativeSubsystem::CapturePreNarrativeInputContext(const FVector& Location, const FRotator& Rotation, bool bTargetLocked)
{
	SavedInputContext.SavedPlayerLocation = Location;
	SavedInputContext.SavedCameraRotation = Rotation;
	SavedInputContext.bWasTargetLocked = bTargetLocked;
	SavedInputContext.bInputWasLocked = true;
}

bool UEOSNarrativeSubsystem::RestorePreNarrativeInputContext()
{
	SavedInputContext.bInputWasLocked = false;
	return true;
}

void UEOSNarrativeSubsystem::RegisterTransitionProfile(const FEOSNarrativeTransitionProfile& Profile)
{
	TransitionProfiles.Add(Profile.ProfileID, Profile);
}

bool UEOSNarrativeSubsystem::GetTransitionProfile(const FString& ProfileID, FEOSNarrativeTransitionProfile& OutProfile) const
{
	if (const FEOSNarrativeTransitionProfile* Found = TransitionProfiles.Find(ProfileID))
	{
		OutProfile = *Found;
		return true;
	}
	return false;
}

void UEOSNarrativeSubsystem::RegisterMusicTransition(const FEOSMusicTransitionRecord& Record)
{
	MusicTransitions.Add(Record.TransitionID, Record);
}

bool UEOSNarrativeSubsystem::GetMusicTransition(const FString& TransitionID, FEOSMusicTransitionRecord& OutTransition) const
{
	if (const FEOSMusicTransitionRecord* Found = MusicTransitions.Find(TransitionID))
	{
		OutTransition = *Found;
		return true;
	}
	return false;
}

bool UEOSNarrativeSubsystem::PresentRewards(const TArray<FEOSRewardPresentationItem>& Rewards, bool bIsReplay)
{
	// Idempotency check: if replay or already granted in this transaction, suppress re-granting
	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		if (TransactionLedger[ActiveTransactionID].bRewardGranted)
		{
			UE_LOG(LogEOSCore, Log, TEXT("[Narrative] Rewards already granted for transaction %s. Suppressing duplicate grant."), *ActiveTransactionID);
			return true;
		}
	}

	if (bIsReplay)
	{
		UE_LOG(LogEOSCore, Log, TEXT("[Narrative] Replay mode: Stage completion rewards suppressed."));
		return true;
	}

	for (const FEOSRewardPresentationItem& Item : Rewards)
	{
		GrantedRewardHistory.Add(Item.RewardID);
	}

	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		TransactionLedger[ActiveTransactionID].bRewardGranted = true;
	}

	return true;
}

bool UEOSNarrativeSubsystem::CommitWorldStateDelta(const FString& DeltaID)
{
	if (DeltaID.IsEmpty())
	{
		return true;
	}

	// Idempotency check: verify this transaction hasn't already committed this delta
	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		if (TransactionLedger[ActiveTransactionID].bWorldStateCommitted)
		{
			UE_LOG(LogEOSCore, Log, TEXT("[Narrative] World state delta already committed for transaction %s. Skipping duplicate mutation."), *ActiveTransactionID);
			return true;
		}
	}

	CurrentWorldStateID = DeltaID;

	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		TransactionLedger[ActiveTransactionID].bWorldStateCommitted = true;
	}

	UE_LOG(LogEOSCore, Log, TEXT("[Narrative] World state delta committed: %s"), *DeltaID);
	return true;
}

bool UEOSNarrativeSubsystem::UnlockStoryArchiveEntry(const FString& ArchiveID, const FString& SequenceID)
{
	if (ArchiveID.IsEmpty())
	{
		return false;
	}

	UnlockedArchiveEntries.AddUnique(ArchiveID);

	if (TransactionLedger.Contains(ActiveTransactionID))
	{
		TransactionLedger[ActiveTransactionID].bArchiveCommitted = true;
	}

	UE_LOG(LogEOSCore, Log, TEXT("[Narrative] Story Archive unlocked: %s for Sequence %s"), *ArchiveID, *SequenceID);
	return true;
}

bool UEOSNarrativeSubsystem::GetTransaction(const FString& TransactionID, FEOSNarrativeTransaction& OutTransaction) const
{
	if (const FEOSNarrativeTransaction* Found = TransactionLedger.Find(TransactionID))
	{
		OutTransaction = *Found;
		return true;
	}
	return false;
}

bool UEOSNarrativeSubsystem::ResumePendingTransaction(const FString& TransactionID)
{
	if (!TransactionLedger.Contains(TransactionID))
	{
		return false;
	}

	FEOSNarrativeTransaction& Tx = TransactionLedger[TransactionID];
	if (Tx.bCompleted)
	{
		return true;
	}

	ActiveTransactionID = TransactionID;
	ActiveStageID = Tx.StageID;
	ActiveSequenceID = Tx.SequenceID;

	// Resume from last uncommitted milestone
	if (!Tx.bRewardGranted)
	{
		TransitionToState(EEOSNarrativeState::RewardPresentation);
	}
	else if (!Tx.bArchiveCommitted)
	{
		TransitionToState(EEOSNarrativeState::ArchiveUpdate);
	}
	else if (!Tx.bWorldStateCommitted)
	{
		TransitionToState(EEOSNarrativeState::WorldStateUpdate);
	}
	else
	{
		TransitionToState(EEOSNarrativeState::NarrativeComplete);
	}

	return true;
}

void UEOSNarrativeSubsystem::UpdateNarrativeSettings(const FEOSNarrativeSettings& NewSettings)
{
	Settings = NewSettings;
}

void UEOSNarrativeSubsystem::RegisterSequence(const FEOSMotionMangaSequenceRecord& Sequence)
{
	SequenceRegistry.Add(Sequence.SequenceID, Sequence);
}

bool UEOSNarrativeSubsystem::GetSequence(const FString& SequenceID, FEOSMotionMangaSequenceRecord& OutSequence) const
{
	if (const FEOSMotionMangaSequenceRecord* Found = SequenceRegistry.Find(SequenceID))
	{
		OutSequence = *Found;
		return true;
	}
	return false;
}
