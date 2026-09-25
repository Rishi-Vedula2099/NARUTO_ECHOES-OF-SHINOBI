#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSUIDataTypes.generated.h"

/**
 * 10-Layer Compositor Hierarchy
 */
UENUM(BlueprintType)
enum class EEOSUILayer : uint8
{
	Background   = 0   UMETA(DisplayName = "Layer 0: Background & 3D Backdrop"),
	World        = 10  UMETA(DisplayName = "Layer 1: In-World Markers & Damage Text"),
	HUD          = 20  UMETA(DisplayName = "Layer 2: Combat & Exploration HUD"),
	Screen       = 30  UMETA(DisplayName = "Layer 3: Full-Screen Menus & Hubs"),
	Modal        = 40  UMETA(DisplayName = "Layer 4: Blocking Confirmation Modals"),
	Popup        = 50  UMETA(DisplayName = "Layer 5: Transient Cards & Level-Up Popups"),
	Notification = 60  UMETA(DisplayName = "Layer 6: System Badges & Live Alerts"),
	Tooltip      = 70  UMETA(DisplayName = "Layer 7: Item & Stat Hover Cards"),
	Tutorial     = 80  UMETA(DisplayName = "Layer 8: Guided Tutorials & Focus Masks"),
	System       = 90  UMETA(DisplayName = "Layer 9: Hardware Cursor & Fatal Error Overlays")
};

/**
 * Screen Transition Styles
 */
UENUM(BlueprintType)
enum class EEOSUITransition : uint8
{
	None,
	Fade,
	SlideLeft,
	SlideRight,
	ScaleUp,
	Dissolve,
	InkSplash
};

/**
 * Custom Shinobi Cursor Styles
 */
UENUM(BlueprintType)
enum class EEOSCursorStyle : uint8
{
	DefaultChakra UMETA(DisplayName = "Kunai Tip with Chakra Breathing"),
	HoverSnap     UMETA(DisplayName = "Magnetic Snap with Ink Pulse"),
	SelectRing    UMETA(DisplayName = "Expanding Chakra Click Ring"),
	LockedSeal    UMETA(DisplayName = "Muted Sealing Ripple on Disabled"),
	DragGrip      UMETA(DisplayName = "Open Hand Camera/Model Drag")
};

/**
 * UI Input Routing Mode
 */
UENUM(BlueprintType)
enum class EEOSUIInputMode : uint8
{
	UIOnly,
	GameAndUI,
	GameOnly
};

/**
 * Screen Definition for Central Screen Registry
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSUIScreenDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString ScreenID; // e.g. "UI_MAIN_MENU", "UI_CHARACTER_DETAIL"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EEOSUILayer Layer = EEOSUILayer::Screen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EEOSUIInputMode InputMode = EEOSUIInputMode::UIOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bCanStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bCanPauseGameplay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString RequiredUnlockID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EEOSUITransition TransitionIn = EEOSUITransition::Fade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	EEOSUITransition TransitionOut = EEOSUITransition::Fade;
};

/**
 * Navigation Stack Snapshot for Deterministic Backflow
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSUINavigationEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	FString ScreenID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	FString ContextID; // e.g. "CHAR_NARUTO" or "STAGE_01_04"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	float ScrollOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	FString FocusedElementID;
};

/**
 * Combat HUD Reactive ViewModel
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCombatHUDViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float CurrentHP = 4500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float MaxHP = 4500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float CurrentChakra = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float MaxChakra = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	int32 SubstitutionStock = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float Jutsu1CooldownPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float Jutsu2CooldownPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float Jutsu3CooldownPercent = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	float UltimateGaugePercent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	int32 ComboHitCount = 28;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	FString StyleRank = "S";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	TArray<FString> ActivePartyCharacterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatHUD")
	int32 ActivePartyIndex = 0;
};

/**
 * Boss Encounter HUD Reactive ViewModel
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossHUDViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	FText BossDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	FText BossSubtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	float CurrentHP = 75000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	float MaxHP = 100000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	int32 CurrentPhase = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	int32 TotalPhases = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	float StaggerGaugePercent = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossHUD")
	bool bIsStaggered = false;
};

/**
 * Character Detail ViewModel with Power Breakdown
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterDetailViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString EraName = "Shippūden";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 Level = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString ShinobiRank = "Jōnin";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float TotalPower = 4850.0f;

	// Authoritative Formula Breakdown Components (Display Only)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float BasePower = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float LevelPower = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float JutsuPower = 950.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float GearPower = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float BondPower = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TArray<FString> EquippedJutsuIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TArray<FString> EquippedGearItemIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString ActiveSetBonusName = "Uchiha Legacy (4/4)";
};

/**
 * Side-by-Side Equipment Comparison ViewModel
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGearComparisonViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	FString CurrentItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	FString NewItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float CurrentAttack = 1250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float NewAttack = 1390.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float AttackDiff = 140.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float CurrentDefense = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float NewDefense = 820.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float DefenseDiff = -80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float PowerDelta = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	bool bActivatesSetBonus = true;
};

/**
 * Reusable Modal Definition
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSUIModalDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	FString ModalID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	FText ConfirmText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	FText CancelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	bool bIsWarning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modal")
	bool bBlocksBackgroundInput = true;
};

/**
 * System Notification Item
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNotificationItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FString NotificationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FString SourceSystem; // "MOTION_MANGA", "BOUNTY", "EVENT", "GEAR"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	int32 Priority = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FString DeepLinkTargetScreenID; // e.g. "UI_MOTION_MANGA_ARCHIVE"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	FString DeepLinkContextID; // e.g. "MM_SEQ_A01_C04"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	bool bIsRead = false;
};

/**
 * Graphics Settings ViewModel (Targeting GTX 1650 Scalability)
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGraphicsSettingsViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	FString PresetName = "Medium"; // "Low", "Medium", "High", "Epic", "GTX1650_Optimized"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 ResolutionX = 1920;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 ResolutionY = 1080;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 FrameRateTarget = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool bVsync = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 ShadowQuality = 2; // 0=Low, 1=Med, 2=High, 3=Epic

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 ParticleQuality = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool bPerformanceModeGTX1650 = false;
};

/**
 * Accessibility Settings ViewModel
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSAccessibilitySettingsViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	float UIScaleFactor = 1.0f; // 1.0x, 1.1x, 1.25x, 1.5x, 1.75x

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	FString SubtitleSize = "Medium"; // "Small", "Medium", "Large"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bSubtitleDarkBackground = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bReducedMotion = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bReducedFlash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	FString ColorAssistMode = "Standard"; // "Deuteranopia", "Protanopia", "Tritanopia"
};
