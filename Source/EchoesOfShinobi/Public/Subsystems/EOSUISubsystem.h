#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSUIDataTypes.h"
#include "EOSUISubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEOSUIScreenChangedDelegate, const FString&, PreviousScreenID, const FString&, NewScreenID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSUIModalStateChangedDelegate, bool, bIsModalOpen);

/**
 * GameInstanceSubsystem managing UI Screen Registration, Navigation Stack,
 * 10-Layer Compositor, Modals, Cursor Styles, and Input Routing.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	typedef UGameInstanceSubsystem Super;
	UEOSUISubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Database & Screen Registry Initialization
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void InitializeDefaultScreenRegistry();

	// Screen Registry Queries
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void RegisterScreen(const FEOSUIScreenDefinition& Definition);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	bool GetScreenDefinition(const FString& ScreenID, FEOSUIScreenDefinition& OutDefinition) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	int32 GetTotalRegisteredScreenCount() const { return ScreenRegistry.Num(); }

	// Navigation Stack Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	bool PushScreen(const FString& ScreenID, const FString& ContextID = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	bool PopScreen();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	bool PopToRoot();

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	FString GetCurrentScreenID() const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	int32 GetNavigationStackDepth() const { return NavigationStack.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	TArray<FString> GetNavigationHistory() const;

	// Modal Dialog Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	bool OpenModal(const FEOSUIModalDefinition& Modal);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	bool CloseActiveModal();

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	bool IsModalActive() const { return ActiveModals.Num() > 0; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	bool GetActiveModal(FEOSUIModalDefinition& OutModal) const;

	// Input Mode & Cursor Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void SetUIInputMode(EEOSUIInputMode NewMode);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	EEOSUIInputMode GetUIInputMode() const { return CurrentInputMode; }

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void SetCursorStyle(EEOSCursorStyle NewStyle);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	EEOSCursorStyle GetCursorStyle() const { return CurrentCursorStyle; }

	// Settings & Accessibility
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void UpdateGraphicsSettings(const FEOSGraphicsSettingsViewModel& NewSettings);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	FEOSGraphicsSettingsViewModel GetGraphicsSettings() const { return GraphicsSettings; }

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|UI")
	void UpdateAccessibilitySettings(const FEOSAccessibilitySettingsViewModel& NewSettings);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|UI")
	FEOSAccessibilitySettingsViewModel GetAccessibilitySettings() const { return AccessibilitySettings; }

public:
	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|UI")
	FEOSUIScreenChangedDelegate OnScreenChanged;

	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|UI")
	FEOSUIModalStateChangedDelegate OnModalStateChanged;

private:
	UPROPERTY()
	TMap<FString, FEOSUIScreenDefinition> ScreenRegistry;

	UPROPERTY()
	TArray<FEOSUINavigationEntry> NavigationStack;

	UPROPERTY()
	TArray<FEOSUIModalDefinition> ActiveModals;

	UPROPERTY()
	EEOSUIInputMode CurrentInputMode = EEOSUIInputMode::UIOnly;

	UPROPERTY()
	EEOSCursorStyle CurrentCursorStyle = EEOSCursorStyle::DefaultChakra;

	UPROPERTY()
	FEOSGraphicsSettingsViewModel GraphicsSettings;

	UPROPERTY()
	FEOSAccessibilitySettingsViewModel AccessibilitySettings;
};
