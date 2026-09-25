#include "../../Public/Subsystems/EOSUISubsystem.h"
#include "../../EchoesOfShinobi.h"

UEOSUISubsystem::UEOSUISubsystem()
{
}

void UEOSUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultScreenRegistry();
	UE_LOG(LogEOSUI, Log, TEXT("UEOSUISubsystem Initialized. 10-Layer Compositor & Screen Registry Ready."));
}

void UEOSUISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSUISubsystem::InitializeDefaultScreenRegistry()
{
	// 1. Core Entry & Hubs
	FEOSUIScreenDefinition MainMenu;
	MainMenu.ScreenID = TEXT("UI_MAIN_MENU");
	MainMenu.DisplayName = FText::FromString(TEXT("Main Menu"));
	MainMenu.Layer = EEOSUILayer::Screen;
	MainMenu.InputMode = EEOSUIInputMode::UIOnly;
	MainMenu.bCanStack = false;
	RegisterScreen(MainMenu);

	FEOSUIScreenDefinition Profile;
	Profile.ScreenID = TEXT("UI_PROFILE");
	Profile.DisplayName = FText::FromString(TEXT("Shinobi Profile"));
	Profile.Layer = EEOSUILayer::Screen;
	RegisterScreen(Profile);

	// 2. Character & Progression
	FEOSUIScreenDefinition CharRoster;
	CharRoster.ScreenID = TEXT("UI_CHARACTER_ROSTER");
	CharRoster.DisplayName = FText::FromString(TEXT("Character Roster"));
	RegisterScreen(CharRoster);

	FEOSUIScreenDefinition CharDetail;
	CharDetail.ScreenID = TEXT("UI_CHARACTER_DETAIL");
	CharDetail.DisplayName = FText::FromString(TEXT("Character Detail"));
	RegisterScreen(CharDetail);

	FEOSUIScreenDefinition JutsuTree;
	JutsuTree.ScreenID = TEXT("UI_JUTSU_TREE");
	JutsuTree.DisplayName = FText::FromString(TEXT("Jutsu Skill Tree"));
	RegisterScreen(JutsuTree);

	FEOSUIScreenDefinition JutsuTrial;
	JutsuTrial.ScreenID = TEXT("UI_JUTSU_TRIAL");
	JutsuTrial.DisplayName = FText::FromString(TEXT("Jutsu Learning Trial"));
	JutsuTrial.InputMode = EEOSUIInputMode::GameAndUI;
	RegisterScreen(JutsuTrial);

	// 3. Equipment, Bonds & Knowledge
	FEOSUIScreenDefinition GearInv;
	GearInv.ScreenID = TEXT("UI_GEAR_INVENTORY");
	GearInv.DisplayName = FText::FromString(TEXT("Gear & Equipment"));
	RegisterScreen(GearInv);

	FEOSUIScreenDefinition GearComp;
	GearComp.ScreenID = TEXT("UI_GEAR_COMPARISON");
	GearComp.DisplayName = FText::FromString(TEXT("Equipment Comparison"));
	GearComp.Layer = EEOSUILayer::Popup;
	RegisterScreen(GearComp);

	FEOSUIScreenDefinition BondGraph;
	BondGraph.ScreenID = TEXT("UI_BOND_GRAPH");
	BondGraph.DisplayName = FText::FromString(TEXT("Character Bond Network"));
	RegisterScreen(BondGraph);

	FEOSUIScreenDefinition Intel;
	Intel.ScreenID = TEXT("UI_SHINOBI_INTEL");
	Intel.DisplayName = FText::FromString(TEXT("Shinobi Intel & Knowledge Graph"));
	RegisterScreen(Intel);

	FEOSUIScreenDefinition Collection;
	Collection.ScreenID = TEXT("UI_COLLECTION");
	Collection.DisplayName = FText::FromString(TEXT("Shinobi Collection"));
	RegisterScreen(Collection);

	// 4. LiveOps, Modes & World
	FEOSUIScreenDefinition Gacha;
	Gacha.ScreenID = TEXT("UI_GACHA_SUMMON");
	Gacha.DisplayName = FText::FromString(TEXT("Shinobi Summons"));
	RegisterScreen(Gacha);

	FEOSUIScreenDefinition Events;
	Events.ScreenID = TEXT("UI_EVENT_HUB");
	Events.DisplayName = FText::FromString(TEXT("Limited Events Hub"));
	RegisterScreen(Events);

	FEOSUIScreenDefinition Bounties;
	Bounties.ScreenID = TEXT("UI_BOUNTY_BOARD");
	Bounties.DisplayName = FText::FromString(TEXT("ANBU Weekly Bounty Board"));
	RegisterScreen(Bounties);

	FEOSUIScreenDefinition WorldMap;
	WorldMap.ScreenID = TEXT("UI_WORLD_MAP");
	WorldMap.DisplayName = FText::FromString(TEXT("World Exploration Map"));
	RegisterScreen(WorldMap);

	FEOSUIScreenDefinition GameModes;
	GameModes.ScreenID = TEXT("UI_GAME_MODES");
	GameModes.DisplayName = FText::FromString(TEXT("Game Modes Hub"));
	RegisterScreen(GameModes);

	// 5. Narrative & Combat
	FEOSUIScreenDefinition MangaArchive;
	MangaArchive.ScreenID = TEXT("UI_MOTION_MANGA_ARCHIVE");
	MangaArchive.DisplayName = FText::FromString(TEXT("Story & Motion Manga Archive"));
	RegisterScreen(MangaArchive);

	FEOSUIScreenDefinition CombatHUD;
	CombatHUD.ScreenID = TEXT("UI_COMBAT_HUD");
	CombatHUD.DisplayName = FText::FromString(TEXT("Combat HUD"));
	CombatHUD.Layer = EEOSUILayer::HUD;
	CombatHUD.InputMode = EEOSUIInputMode::GameOnly;
	RegisterScreen(CombatHUD);

	FEOSUIScreenDefinition BossHUD;
	BossHUD.ScreenID = TEXT("UI_BOSS_HUD");
	BossHUD.DisplayName = FText::FromString(TEXT("Boss HUD"));
	BossHUD.Layer = EEOSUILayer::HUD;
	BossHUD.InputMode = EEOSUIInputMode::GameOnly;
	RegisterScreen(BossHUD);

	// 6. Settings & System
	FEOSUIScreenDefinition Settings;
	Settings.ScreenID = TEXT("UI_SETTINGS");
	Settings.DisplayName = FText::FromString(TEXT("Master Settings"));
	RegisterScreen(Settings);

	FEOSUIScreenDefinition Pause;
	Pause.ScreenID = TEXT("UI_PAUSE");
	Pause.DisplayName = FText::FromString(TEXT("Pause Menu"));
	Pause.Layer = EEOSUILayer::Modal;
	Pause.bCanPauseGameplay = true;
	RegisterScreen(Pause);

	// Initial default screen is Main Menu
	PushScreen(TEXT("UI_MAIN_MENU"));
}

void UEOSUISubsystem::RegisterScreen(const FEOSUIScreenDefinition& Definition)
{
	ScreenRegistry.Add(Definition.ScreenID, Definition);
}

bool UEOSUISubsystem::GetScreenDefinition(const FString& ScreenID, FEOSUIScreenDefinition& OutDefinition) const
{
	if (const FEOSUIScreenDefinition* Found = ScreenRegistry.Find(ScreenID))
	{
		OutDefinition = *Found;
		return true;
	}
	return false;
}

bool UEOSUISubsystem::PushScreen(const FString& ScreenID, const FString& ContextID)
{
	if (!ScreenRegistry.Contains(ScreenID))
	{
		UE_LOG(LogEOSUI, Warning, TEXT("[UI] PushScreen failed: ScreenID %s not registered."), *ScreenID);
		return false;
	}

	FString PreviousScreenID = GetCurrentScreenID();

	FEOSUINavigationEntry Entry;
	Entry.ScreenID = ScreenID;
	Entry.ContextID = ContextID;
	NavigationStack.Add(Entry);

	// Update Input Mode per screen definition
	CurrentInputMode = ScreenRegistry[ScreenID].InputMode;

	if (OnScreenChanged.IsBound())
	{
		OnScreenChanged.Broadcast(PreviousScreenID, ScreenID);
	}

	UE_LOG(LogEOSUI, Log, TEXT("[UI] Pushed Screen: %s (Stack Depth: %d)"), *ScreenID, NavigationStack.Num());
	return true;
}

bool UEOSUISubsystem::PopScreen()
{
	if (NavigationStack.Num() <= 1)
	{
		// Cannot pop root screen
		return false;
	}

	FString PreviousScreenID = GetCurrentScreenID();
	NavigationStack.Pop();
	FString NewScreenID = GetCurrentScreenID();

	if (ScreenRegistry.Contains(NewScreenID))
	{
		CurrentInputMode = ScreenRegistry[NewScreenID].InputMode;
	}

	if (OnScreenChanged.IsBound())
	{
		OnScreenChanged.Broadcast(PreviousScreenID, NewScreenID);
	}

	UE_LOG(LogEOSUI, Log, TEXT("[UI] Popped to Screen: %s (Stack Depth: %d)"), *NewScreenID, NavigationStack.Num());
	return true;
}

bool UEOSUISubsystem::PopToRoot()
{
	if (NavigationStack.Num() <= 1)
	{
		return true;
	}

	FString PreviousScreenID = GetCurrentScreenID();
	FEOSUINavigationEntry Root = NavigationStack[0];
	NavigationStack.Empty();
	NavigationStack.Add(Root);

	FString NewScreenID = Root.ScreenID;
	if (ScreenRegistry.Contains(NewScreenID))
	{
		CurrentInputMode = ScreenRegistry[NewScreenID].InputMode;
	}

	if (OnScreenChanged.IsBound())
	{
		OnScreenChanged.Broadcast(PreviousScreenID, NewScreenID);
	}

	return true;
}

FString UEOSUISubsystem::GetCurrentScreenID() const
{
	if (NavigationStack.Num() > 0)
	{
		return NavigationStack.Last().ScreenID;
	}
	return TEXT("");
}

TArray<FString> UEOSUISubsystem::GetNavigationHistory() const
{
	TArray<FString> History;
	for (const FEOSUINavigationEntry& Entry : NavigationStack)
	{
		History.Add(Entry.ScreenID);
	}
	return History;
}

bool UEOSUISubsystem::OpenModal(const FEOSUIModalDefinition& Modal)
{
	ActiveModals.Add(Modal);
	if (OnModalStateChanged.IsBound())
	{
		OnModalStateChanged.Broadcast(true);
	}
	UE_LOG(LogEOSUI, Log, TEXT("[UI] Modal Opened: %s (Active Modals: %d)"), *Modal.ModalID, ActiveModals.Num());
	return true;
}

bool UEOSUISubsystem::CloseActiveModal()
{
	if (ActiveModals.Num() == 0)
	{
		return false;
	}

	ActiveModals.Pop();
	if (OnModalStateChanged.IsBound())
	{
		OnModalStateChanged.Broadcast(ActiveModals.Num() > 0);
	}
	UE_LOG(LogEOSUI, Log, TEXT("[UI] Active Modal Closed. Remaining: %d"), ActiveModals.Num());
	return true;
}

bool UEOSUISubsystem::GetActiveModal(FEOSUIModalDefinition& OutModal) const
{
	if (ActiveModals.Num() > 0)
	{
		OutModal = ActiveModals.Last();
		return true;
	}
	return false;
}

void UEOSUISubsystem::SetUIInputMode(EEOSUIInputMode NewMode)
{
	CurrentInputMode = NewMode;
}

void UEOSUISubsystem::SetCursorStyle(EEOSCursorStyle NewStyle)
{
	CurrentCursorStyle = NewStyle;
}

void UEOSUISubsystem::UpdateGraphicsSettings(const FEOSGraphicsSettingsViewModel& NewSettings)
{
	GraphicsSettings = NewSettings;
	UE_LOG(LogEOSUI, Log, TEXT("[UI] Graphics Settings Updated: Preset=%s (1650 Performance Mode=%s)"),
		*GraphicsSettings.PresetName, GraphicsSettings.bPerformanceModeGTX1650 ? TEXT("TRUE") : TEXT("FALSE"));
}

void UEOSUISubsystem::UpdateAccessibilitySettings(const FEOSAccessibilitySettingsViewModel& NewSettings)
{
	AccessibilitySettings = NewSettings;
	UE_LOG(LogEOSUI, Log, TEXT("[UI] Accessibility Settings Updated: Scale=%.2fx, ReducedMotion=%s"),
		AccessibilitySettings.UIScaleFactor, AccessibilitySettings.bReducedMotion ? TEXT("TRUE") : TEXT("FALSE"));
}
