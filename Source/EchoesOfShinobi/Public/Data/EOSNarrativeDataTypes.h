#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSNarrativeDataTypes.generated.h"

/**
 * Deterministic Master Narrative State Machine States (24 states)
 */
UENUM(BlueprintType)
enum class EEOSNarrativeState : uint8
{
	None                  UMETA(DisplayName = "None / Idle"),
	MissionIntro          UMETA(DisplayName = "Mission Intro Briefing"),
	MissionActive         UMETA(DisplayName = "Mission Active Gameplay"),
	BossIntro             UMETA(DisplayName = "Boss Introduction Cutscene"),
	BossActive            UMETA(DisplayName = "Boss Active Combat"),
	BossDefeated          UMETA(DisplayName = "Boss Defeated Transition"),
	MissionComplete       UMETA(DisplayName = "Mission Complete Banner"),
	HUDFade               UMETA(DisplayName = "HUD Element Fade"),
	MusicTransition       UMETA(DisplayName = "Music State Transition"),
	NarrativeLoading      UMETA(DisplayName = "Narrative Asset Preload"),
	MotionMangaStart      UMETA(DisplayName = "Motion Manga Start"),
	PanelActive           UMETA(DisplayName = "Panel Active Playback"),
	DialogueActive        UMETA(DisplayName = "Dialogue / Caption Active"),
	PanelTransition       UMETA(DisplayName = "Panel Transition"),
	FinalPanel            UMETA(DisplayName = "Final Climax Panel"),
	ChapterComplete       UMETA(DisplayName = "Chapter Complete Resolution"),
	ArcComplete           UMETA(DisplayName = "Arc Complete Celebration"),
	RewardPresentation    UMETA(DisplayName = "Reward Presentation"),
	ArchiveUpdate         UMETA(DisplayName = "Story Archive Update"),
	WorldStateUpdate      UMETA(DisplayName = "World State Update Commit"),
	NarrativeComplete     UMETA(DisplayName = "Narrative Complete"),
	ReturnToGame          UMETA(DisplayName = "Return To Game Control"),
	Skipped               UMETA(DisplayName = "Narrative Skipped"),
	FailedRecovery        UMETA(DisplayName = "Failed Graceful Recovery")
};

/**
 * 2.5D Motion Manga Motion Profiles
 */
UENUM(BlueprintType)
enum class EEOSPanelMotionProfile : uint8
{
	Static         UMETA(DisplayName = "Static Subtle Breathing"),
	PanLeft        UMETA(DisplayName = "Pan Left Tracking"),
	PanRight       UMETA(DisplayName = "Pan Right Tracking"),
	ZoomIn         UMETA(DisplayName = "Dramatic Zoom In (1.00 -> 1.08)"),
	ZoomOut        UMETA(DisplayName = "Wide Reveal Zoom Out (1.08 -> 1.00)"),
	Impact         UMETA(DisplayName = "Impact Strike Impulse Shake"),
	Reveal         UMETA(DisplayName = "Silhouette To Bloom Reveal"),
	DramaticHold   UMETA(DisplayName = "Dramatic Climax Hold"),
	BossReveal     UMETA(DisplayName = "Boss Entrance Push")
};

/**
 * Panel Visual Transitions
 */
UENUM(BlueprintType)
enum class EEOSPanelTransition : uint8
{
	Fade         UMETA(DisplayName = "Soft Alpha Fade"),
	Crossfade    UMETA(DisplayName = "Panel Crossfade"),
	Slide        UMETA(DisplayName = "Directional Slide"),
	InkSplash    UMETA(DisplayName = "Sumi Ink Splash Transition"),
	WhiteFlash   UMETA(DisplayName = "White Impact Flash"),
	BlackFlash   UMETA(DisplayName = "Black Cinematic Cut"),
	ImpactCut    UMETA(DisplayName = "Hard Impact Jump Cut"),
	PageTurn     UMETA(DisplayName = "Manga Page Turn Curvature")
};

/**
 * Single Dialogue / Caption Entry
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSDialogueEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString SpeakerID; // e.g. "CHAR_NARUTO", "CHAR_SASUKE", "CHAR_KAKASHI"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString LocalizationKey; // e.g. "LOC_A01_C04_D001"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString VoiceID; // e.g. "VOX_A01_NARUTO_01"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString PortraitID; // e.g. "PORTRAIT_NARUTO_DETERMINED"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString Emotion = "DETERMINED";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float Duration = 3.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bAutoAdvance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString CanonStatus = "CANON_VERIFIED";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString Provenance = "MANGA_CHAPTER_33";
};

/**
 * Individual Motion Manga Panel Definition (2.5D Layered)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMotionMangaPanel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString PanelID; // e.g. "MM_A01_C04_P001"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString SequenceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	int32 PanelIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ImageAsset; // Primary or Fallback Composite

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString BackgroundAsset; // Layer 0 (Depth 0.10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	TArray<FString> CharacterLayerAssets; // Layer 1-2 (Depth 0.50)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ForegroundFX; // Layer 3 (Depth 0.75 - 1.00)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString CaptionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString CameraProfileID = "CAM_DEFAULT_DRIFT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	EEOSPanelMotionProfile MotionProfile = EEOSPanelMotionProfile::ZoomIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	EEOSPanelTransition TransitionIn = EEOSPanelTransition::Fade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	EEOSPanelTransition TransitionOut = EEOSPanelTransition::Fade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString SFXID; // e.g. "SFX_CHAKRA_PULSE_01"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString VoiceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	float Duration = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	float HoldDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	bool bIsFinalPanel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ArchiveUnlockID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString CanonStatus = "CANON_VERIFIED";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString Provenance = "SOURCE_DERIVED";
};

/**
 * Motion Manga Sequence Container
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMotionMangaSequenceRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString SequenceID; // e.g. "MM_SEQ_A01_C04_CLIMAX"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ArcID = "ARC_01";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ChapterID = "CH_01_04";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString StageID = "STG_01_04_05";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FText Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	TArray<FEOSMotionMangaPanel> Panels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString MusicID = "MUS_NARRATIVE_REFLECTIVE_01";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString ArchiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString FinalPanelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString WorldStateDeltaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	bool bReplayAllowed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	bool bSkipAllowed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	bool bFastForwardAllowed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionManga")
	FString CanonStatus = "CANON_VERIFIED";
};

/**
 * Staged HUD Fade Profile Timing
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNarrativeTransitionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	FString ProfileID = "TRANS_PROFILE_DEFAULT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float BossUIFadeTime = 0.10f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float ObjectiveUIFadeTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float SkillUIFadeTime = 0.40f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float MinimapFadeTime = 0.50f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float FullHUDHiddenTime = 0.60f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float NarrativeStartTime = 0.80f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	bool bLetterboxEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float CameraBlendDuration = 1.0f;
};

/**
 * Audio / Music State Transition Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMusicTransitionRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	FString TransitionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	FString FromMusicState = "COMBAT_TRACK";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	FString ToMusicState = "NARRATIVE_TRACK";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float FadeOutDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float CrossfadeDuration = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	FString StingerID = "STINGER_BOSS_DEFEAT_01";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 Priority = 100;
};

/**
 * Stage Result Package passed from Authoritative Stage System
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSStageResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	FString StageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	bool bIsSuccess = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	float CompletionTime = 145.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	TArray<FString> CompletedObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	bool bBossDefeated = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	FString NarrativeSequenceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	FString WorldStateDeltaID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageResult")
	FString TransactionID;
};

/**
 * Reward Presentation Item for UI Reveal (Phase 12 presents, does not calculate)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSRewardPresentationItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FString RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FString Type; // "EXP", "RYO", "GEAR", "MOTION_MANGA", "LORE"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FString Rarity = "SSR";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	bool bIsNew = true;
};

/**
 * Saved Pre-Narrative Input & Camera Context for Safe Restoration
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSPreNarrativeInputContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputContext")
	bool bInputWasLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputContext")
	bool bCombatAIWasActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputContext")
	bool bWasTargetLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputContext")
	FVector SavedPlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputContext")
	FRotator SavedCameraRotation = FRotator::ZeroRotator;
};

/**
 * Idempotent Narrative Transaction Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNarrativeTransaction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	FString TransactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	FString StageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	FString SequenceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bStageResultLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bNarrativeStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bNarrativeCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bArchiveCommitted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bWorldStateCommitted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bRewardGranted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction")
	bool bCompleted = false;
};

/**
 * Narrative & Motion Manga Accessibility & Playback Settings
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNarrativeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float TextSpeed = 1.0f; // 0.5x, 1.0x, 2.0x, Instant

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bAutoAdvance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString SubtitleSize = "Medium"; // Small, Medium, Large

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bSubtitleDarkBackground = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bReducedMotion = false; // Reduces shake, rapid zoom, parallax intensity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bReducedFlash = false; // Replaces white flashes with smooth crossfades

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float VoiceVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MusicVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SFXVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ReplayPlaybackSpeed = 1.0f; // 1.0x, 1.5x, 2.0x
};
