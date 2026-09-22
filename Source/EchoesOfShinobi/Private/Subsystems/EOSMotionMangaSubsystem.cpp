#include "../../Public/Subsystems/EOSMotionMangaSubsystem.h"
#include "../../EchoesOfShinobi.h"

UEOSMotionMangaSubsystem::UEOSMotionMangaSubsystem()
{
}

void UEOSMotionMangaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultMangaDatabase();
	UE_LOG(LogEOSCore, Log, TEXT("UEOSMotionMangaSubsystem Initialized. 2.5D Parallax Engine & Panel Streaming Ready."));
}

void UEOSMotionMangaSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSMotionMangaSubsystem::InitializeDefaultMangaDatabase()
{
	// 1. Register Canonical Speaker Profiles
	RegisterSpeaker(TEXT("CHAR_NARUTO"), FText::FromString(TEXT("Naruto Uzumaki")), TEXT("PORTRAIT_NARUTO_CLASSIC"));
	RegisterSpeaker(TEXT("CHAR_SASUKE"), FText::FromString(TEXT("Sasuke Uchiha")), TEXT("PORTRAIT_SASUKE_CLASSIC"));
	RegisterSpeaker(TEXT("CHAR_KAKASHI"), FText::FromString(TEXT("Kakashi Hatake")), TEXT("PORTRAIT_KAKASHI_SHINOBI"));
	RegisterSpeaker(TEXT("CHAR_ZABUZA"), FText::FromString(TEXT("Zabuza Momochi")), TEXT("PORTRAIT_ZABUZA_DEMON"));
	RegisterSpeaker(TEXT("CHAR_HAKU"), FText::FromString(TEXT("Haku")), TEXT("PORTRAIT_HAKU_ICE"));
	RegisterSpeaker(TEXT("CHAR_TAZUNA"), FText::FromString(TEXT("Tazuna")), TEXT("PORTRAIT_TAZUNA_BUILDER"));
	RegisterSpeaker(TEXT("CHAR_OBITO"), FText::FromString(TEXT("Obito Uchiha")), TEXT("PORTRAIT_OBITO_MASKED"));
	RegisterSpeaker(TEXT("CHAR_MADARA"), FText::FromString(TEXT("Madara Uchiha")), TEXT("PORTRAIT_MADARA_LEGEND"));

	// 2. Register Canonical Dialogue Entries for Arc 01 Climax
	FEOSDialogueEntry D1;
	D1.DialogueID = TEXT("D_A01_C04_P001_01");
	D1.SpeakerID = TEXT("CHAR_ZABUZA");
	D1.SpeakerDisplayName = FText::FromString(TEXT("Zabuza Momochi"));
	D1.Text = FText::FromString(TEXT("Your words, kid... they cut deeper than any blade."));
	D1.LocalizationKey = TEXT("LOC_A01_C04_D001");
	D1.VoiceID = TEXT("VOX_A01_ZABUZA_01");
	D1.PortraitID = TEXT("PORTRAIT_ZABUZA_DEMON");
	D1.Emotion = TEXT("SORROW");
	RegisterDialogueEntry(D1);

	FEOSDialogueEntry D2;
	D2.DialogueID = TEXT("D_A01_C04_P002_01");
	D2.SpeakerID = TEXT("CHAR_ZABUZA");
	D2.SpeakerDisplayName = FText::FromString(TEXT("Zabuza Momochi"));
	D2.Text = FText::FromString(TEXT("If it were possible... I'd like to go to the same place as you, Haku."));
	D2.LocalizationKey = TEXT("LOC_A01_C04_D002");
	D2.VoiceID = TEXT("VOX_A01_ZABUZA_02");
	D2.PortraitID = TEXT("PORTRAIT_ZABUZA_DEMON");
	D2.Emotion = TEXT("REDEMPTION");
	RegisterDialogueEntry(D2);

	FEOSDialogueEntry D3;
	D3.DialogueID = TEXT("D_A01_C04_P003_01");
	D3.SpeakerID = TEXT("CHAR_KAKASHI");
	D3.SpeakerDisplayName = FText::FromString(TEXT("Kakashi Hatake"));
	D3.Text = FText::FromString(TEXT("A shinobi is not a tool to be discarded. They lived as true ninja to the very end."));
	D3.LocalizationKey = TEXT("LOC_A01_C04_D003");
	D3.VoiceID = TEXT("VOX_A01_KAKASHI_01");
	D3.PortraitID = TEXT("PORTRAIT_KAKASHI_SHINOBI");
	D3.Emotion = TEXT("SOLEMN");
	RegisterDialogueEntry(D3);

	FEOSDialogueEntry D4;
	D4.DialogueID = TEXT("D_A01_C04_P004_01");
	D4.SpeakerID = TEXT("CHAR_NARUTO");
	D4.SpeakerDisplayName = FText::FromString(TEXT("Naruto Uzumaki"));
	D4.Text = FText::FromString(TEXT("I've decided! I'm gonna forge my own ninja way—one with no regrets, dattebayo!"));
	D4.LocalizationKey = TEXT("LOC_A01_C04_D004");
	D4.VoiceID = TEXT("VOX_A01_NARUTO_01");
	D4.PortraitID = TEXT("PORTRAIT_NARUTO_DETERMINED");
	D4.Emotion = TEXT("DETERMINED");
	RegisterDialogueEntry(D4);

	FEOSDialogueEntry D5;
	D5.DialogueID = TEXT("D_A01_C04_P005_01");
	D5.SpeakerID = TEXT("CHAR_TAZUNA");
	D5.SpeakerDisplayName = FText::FromString(TEXT("Tazuna"));
	D5.Text = FText::FromString(TEXT("This bridge that brought us hope... it shall forever be known as the Great Naruto Bridge!"));
	D5.LocalizationKey = TEXT("LOC_A01_C04_D005");
	D5.VoiceID = TEXT("VOX_A01_TAZUNA_01");
	D5.PortraitID = TEXT("PORTRAIT_TAZUNA_BUILDER");
	D5.Emotion = TEXT("TRIUMPH");
	RegisterDialogueEntry(D5);

	// 3. Register Arc 01 Sequence Template
	FEOSMotionMangaSequenceRecord WaveClimax;
	WaveClimax.SequenceID = TEXT("MM_SEQ_A01_C04_ZABUZA_CLIMAX");
	WaveClimax.ArcID = TEXT("ARC_01");
	WaveClimax.ChapterID = TEXT("CH_01_04");
	WaveClimax.StageID = TEXT("STG_01_04_05");
	WaveClimax.Title = FText::FromString(TEXT("The Bridge Called Peace"));
	WaveClimax.Subtitle = FText::FromString(TEXT("Naruto's Ninja Way"));
	WaveClimax.ArchiveID = TEXT("ARCHIVE_MM_A01_C04");
	WaveClimax.FinalPanelID = TEXT("MM_A01_C04_P005_FINAL");

	FEOSMotionMangaPanel P1;
	P1.PanelID = TEXT("MM_A01_C04_P001");
	P1.SequenceID = WaveClimax.SequenceID;
	P1.PanelIndex = 0;
	P1.ImageAsset = TEXT("Tex_Manga_Wave_P001");
	P1.BackgroundAsset = TEXT("Tex_Manga_Wave_P001_BG");
	P1.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P001_Snow"));
	P1.MotionProfile = EEOSPanelMotionProfile::ZoomIn;
	P1.DialogueID = TEXT("D_A01_C04_P001_01");
	WaveClimax.Panels.Add(P1);

	FEOSMotionMangaPanel P2;
	P2.PanelID = TEXT("MM_A01_C04_P002");
	P2.SequenceID = WaveClimax.SequenceID;
	P2.PanelIndex = 1;
	P2.ImageAsset = TEXT("Tex_Manga_Wave_P002");
	P2.BackgroundAsset = TEXT("Tex_Manga_Wave_P002_BG");
	P2.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P002_Zabuza"));
	P2.MotionProfile = EEOSPanelMotionProfile::PanLeft;
	P2.DialogueID = TEXT("D_A01_C04_P002_01");
	WaveClimax.Panels.Add(P2);

	FEOSMotionMangaPanel P3;
	P3.PanelID = TEXT("MM_A01_C04_P003");
	P3.SequenceID = WaveClimax.SequenceID;
	P3.PanelIndex = 2;
	P3.ImageAsset = TEXT("Tex_Manga_Wave_P003");
	P3.BackgroundAsset = TEXT("Tex_Manga_Wave_P003_BG");
	P3.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P003_Kakashi_Zabuza"));
	P3.MotionProfile = EEOSPanelMotionProfile::DramaticHold;
	P3.DialogueID = TEXT("D_A01_C04_P003_01");
	WaveClimax.Panels.Add(P3);

	FEOSMotionMangaPanel P4;
	P4.PanelID = TEXT("MM_A01_C04_P004");
	P4.SequenceID = WaveClimax.SequenceID;
	P4.PanelIndex = 3;
	P4.ImageAsset = TEXT("Tex_Manga_Wave_P004");
	P4.BackgroundAsset = TEXT("Tex_Manga_Wave_P004_BG");
	P4.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P004_Naruto"));
	P4.MotionProfile = EEOSPanelMotionProfile::ZoomIn;
	P4.DialogueID = TEXT("D_A01_C04_P004_01");
	WaveClimax.Panels.Add(P4);

	FEOSMotionMangaPanel P5;
	P5.PanelID = WaveClimax.FinalPanelID;
	P5.SequenceID = WaveClimax.SequenceID;
	P5.PanelIndex = 4;
	P5.ImageAsset = TEXT("Tex_Manga_Wave_P005_Final");
	P5.BackgroundAsset = TEXT("Tex_Manga_Wave_P005_Sunrise");
	P5.CharacterLayerAssets.Add(TEXT("Tex_Manga_Wave_P005_Team7"));
	P5.MotionProfile = EEOSPanelMotionProfile::DramaticHold;
	P5.bIsFinalPanel = true;
	P5.DialogueID = TEXT("D_A01_C04_P005_01");
	WaveClimax.Panels.Add(P5);

	SequenceRegistry.Add(WaveClimax.SequenceID, WaveClimax);
}

bool UEOSMotionMangaSubsystem::LoadSequence(const FString& SequenceID)
{
	if (!SequenceRegistry.Contains(SequenceID))
	{
		UE_LOG(LogEOSCore, Warning, TEXT("[MotionManga] Sequence not found: %s"), *SequenceID);
		return false;
	}

	ActiveSequence = SequenceRegistry[SequenceID];
	CurrentPanelIndex = 0;
	bIsPlaying = false;

	// Preload first panel assets
	if (ActiveSequence.Panels.Num() > 0)
	{
		LoadedAssetCache.Add(ActiveSequence.Panels[0].ImageAsset);
		LoadedAssetCache.Add(ActiveSequence.Panels[0].BackgroundAsset);
	}
	if (ActiveSequence.Panels.Num() > 1)
	{
		LoadedAssetCache.Add(ActiveSequence.Panels[1].ImageAsset);
	}

	return true;
}

bool UEOSMotionMangaSubsystem::StartPlayback()
{
	if (ActiveSequence.Panels.Num() == 0)
	{
		return false;
	}

	bIsPlaying = true;
	CurrentPanelIndex = 0;

	if (OnPanelChanged.IsBound())
	{
		OnPanelChanged.Broadcast(ActiveSequence.SequenceID, CurrentPanelIndex);
	}

	return true;
}

bool UEOSMotionMangaSubsystem::AdvancePanel()
{
	if (!bIsPlaying || ActiveSequence.Panels.Num() == 0)
	{
		return false;
	}

	if (CurrentPanelIndex + 1 < ActiveSequence.Panels.Num())
	{
		CurrentPanelIndex++;
		ReleasePreviousPanel();
		PreloadNextPanel();

		if (OnPanelChanged.IsBound())
		{
			OnPanelChanged.Broadcast(ActiveSequence.SequenceID, CurrentPanelIndex);
		}

		if (IsActivePanelFinal() && OnFinalPanelReached.IsBound())
		{
			OnFinalPanelReached.Broadcast(ActiveSequence.SequenceID);
		}

		return true;
	}

	return false;
}

bool UEOSMotionMangaSubsystem::GetActivePanel(FEOSMotionMangaPanel& OutPanel) const
{
	if (ActiveSequence.Panels.IsValidIndex(CurrentPanelIndex))
	{
		OutPanel = ActiveSequence.Panels[CurrentPanelIndex];
		return true;
	}
	return false;
}

bool UEOSMotionMangaSubsystem::IsActivePanelFinal() const
{
	if (ActiveSequence.Panels.IsValidIndex(CurrentPanelIndex))
	{
		return ActiveSequence.Panels[CurrentPanelIndex].bIsFinalPanel;
	}
	return false;
}

FVector2D UEOSMotionMangaSubsystem::CalculateLayerParallaxOffset(const FVector2D& CameraDelta, float LayerDepthRatio, bool bReducedMotion) const
{
	const float MotionScale = bReducedMotion ? 0.3f : 1.0f;
	return CameraDelta * LayerDepthRatio * MotionScale;
}

float UEOSMotionMangaSubsystem::EvaluateCameraScale(EEOSPanelMotionProfile Profile, float NormalizedTime, bool bReducedMotion) const
{
	const float Dampening = bReducedMotion ? 0.25f : 1.0f;
	const float ClampedTime = FMath::Clamp(NormalizedTime, 0.0f, 1.0f);

	switch (Profile)
	{
	case EEOSPanelMotionProfile::ZoomIn:
		return 1.0f + (0.08f * ClampedTime * Dampening);

	case EEOSPanelMotionProfile::ZoomOut:
		return 1.08f - (0.08f * ClampedTime * Dampening);

	case EEOSPanelMotionProfile::Impact:
		return 1.0f + (0.12f * (1.0f - ClampedTime) * Dampening);

	case EEOSPanelMotionProfile::DramaticHold:
		return 1.02f + (0.01f * ClampedTime * Dampening);

	default:
		break;
	}

	return 1.0f;
}

FVector2D UEOSMotionMangaSubsystem::EvaluateCameraOffset(EEOSPanelMotionProfile Profile, float NormalizedTime, bool bReducedMotion) const
{
	const float Dampening = bReducedMotion ? 0.25f : 1.0f;
	const float ClampedTime = FMath::Clamp(NormalizedTime, 0.0f, 1.0f);

	switch (Profile)
	{
	case EEOSPanelMotionProfile::PanLeft:
		return FVector2D(-40.0f * ClampedTime * Dampening, 0.0f);

	case EEOSPanelMotionProfile::PanRight:
		return FVector2D(40.0f * ClampedTime * Dampening, 0.0f);

	case EEOSPanelMotionProfile::Impact:
		return FVector2D(FMath::Sin(ClampedTime * 20.0f) * 8.0f * Dampening, FMath::Cos(ClampedTime * 20.0f) * 6.0f * Dampening);

	default:
		break;
	}

	return FVector2D::ZeroVector;
}

void UEOSMotionMangaSubsystem::RegisterDialogueEntry(const FEOSDialogueEntry& Entry)
{
	DialogueRegistry.Add(Entry.DialogueID, Entry);
}

bool UEOSMotionMangaSubsystem::GetDialogueEntry(const FString& DialogueID, FEOSDialogueEntry& OutEntry) const
{
	if (const FEOSDialogueEntry* Found = DialogueRegistry.Find(DialogueID))
	{
		OutEntry = *Found;
		return true;
	}
	return false;
}

void UEOSMotionMangaSubsystem::RegisterSpeaker(const FString& SpeakerID, const FText& DisplayName, const FString& DefaultPortrait)
{
	SpeakerNames.Add(SpeakerID, DisplayName);
	SpeakerPortraits.Add(SpeakerID, DefaultPortrait);
}

bool UEOSMotionMangaSubsystem::ResolveSpeaker(const FString& SpeakerID, FText& OutDisplayName, FString& OutPortrait) const
{
	bool bFound = false;
	if (const FText* Name = SpeakerNames.Find(SpeakerID))
	{
		OutDisplayName = *Name;
		bFound = true;
	}
	if (const FString* Portrait = SpeakerPortraits.Find(SpeakerID))
	{
		OutPortrait = *Portrait;
		bFound = true;
	}
	return bFound;
}

void UEOSMotionMangaSubsystem::PreloadNextPanel()
{
	int32 NextIndex = CurrentPanelIndex + 1;
	if (ActiveSequence.Panels.IsValidIndex(NextIndex))
	{
		LoadedAssetCache.Add(ActiveSequence.Panels[NextIndex].ImageAsset);
		LoadedAssetCache.Add(ActiveSequence.Panels[NextIndex].BackgroundAsset);
	}
}

void UEOSMotionMangaSubsystem::ReleasePreviousPanel()
{
	int32 PrevIndex = CurrentPanelIndex - 2;
	if (ActiveSequence.Panels.IsValidIndex(PrevIndex))
	{
		LoadedAssetCache.Remove(ActiveSequence.Panels[PrevIndex].ImageAsset);
		LoadedAssetCache.Remove(ActiveSequence.Panels[PrevIndex].BackgroundAsset);
	}
}

bool UEOSMotionMangaSubsystem::IsAssetLoaded(const FString& AssetPath) const
{
	return LoadedAssetCache.Contains(AssetPath);
}

FString UEOSMotionMangaSubsystem::ResolvePanelAssetWithFallback(const FString& DesiredAsset) const
{
	if (DesiredAsset.IsEmpty())
	{
		return TEXT("Tex_Manga_Fallback_Composite");
	}
	return DesiredAsset;
}
