#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Data/EOSUIDataTypes.h"
#include "EOSShinobiHUD.generated.h"

class AEOSCharacterBase;
class UUserWidget;

/**
 * Authoritative Shinobi HUD for NARUTO: ECHOES OF SHINOBI.
 * Features an immediate Canvas rendering pipeline for guaranteed runtime display,
 * dynamic proximity interaction queries, minimap radar, Team 7 squad cards,
 * boss health & stagger gauge, style-rank combo tracking, mission tracking,
 * and UMG widget support.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSShinobiHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEOSShinobiHUD();

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	TSubclassOf<UUserWidget> ShinobiHUDWidgetClass;

	// Target/Boss Information for top bar
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetTargetBossInfo(const FString& InBossName, float InHealthPercent);

	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetBossPhaseInfo(int32 CurrentPhase, int32 TotalPhases, float StaggerPercent, bool bIsStaggered);

	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetTargetBossExtendedInfo(const FString& InBossName, const FString& InSubtitle, float InHealthPercent, int32 InPhase, int32 InTotalPhases, float InStaggerPercent, bool bInStaggered);

	// Combo Counter Increment & Style Rank
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void RegisterHit(int32 HitCount, float HitScore);

	UFUNCTION(BlueprintPure, Category = "EOS|UI")
	FString GetCurrentStyleRank() const;

	// Contextual Interaction Prompt
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetInteractionPrompt(const FString& InPromptText, bool bShow);

	// Mission Tracker
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetMissionInfo(const FString& InTitle, const FString& InObjective, const FString& InProgress);

	// Combat Notification / Jutsu Banner
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void ShowJutsuBanner(const FString& InJutsuName, float DurationSeconds = 2.0f);

	// ViewModel Sync
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void ApplyCombatViewModel(const FEOSCombatHUDViewModel& InViewModel);

	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void ApplyBossViewModel(const FEOSBossHUDViewModel& InViewModel);

	// HUD Visibility / Presentation fade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float HUDOpacity = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetHUDOpacity(float InOpacity) { HUDOpacity = FMath::Clamp(InOpacity, 0.0f, 1.0f); }

	// Team 7 Active Party Index (0 = Naruto, 1 = Sasuke, 2 = Sakura)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 ActivePartyIndex = 0;

	// Substitution Stock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 SubstitutionStock = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 MaxSubstitutionStock = 4;

	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float Stamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float MaxStamina = 100.0f;

	// Cooldown Overlays (0.0 to 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float Jutsu1Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float Jutsu2Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float Jutsu3Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float UltimateGauge = 1.0f;

	// Query Helpers for Unit Testing & Verification
	UFUNCTION(BlueprintPure, Category = "EOS|UI")
	bool HasActiveTarget() const { return bHasActiveTarget; }

	UFUNCTION(BlueprintPure, Category = "EOS|UI")
	int32 GetComboHits() const { return CurrentComboHits; }

	UFUNCTION(BlueprintPure, Category = "EOS|UI")
	float GetTargetHealthPercent() const { return ActiveTargetHealthPercent; }

protected:
	void DrawHealthAndChakraBars(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawJutsuAndUltimateSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawSquadSelectionSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawTargetBossBar(float ScreenW, float ScreenH);
	void DrawComboDisplay(float ScreenW, float ScreenH);
	void DrawDataDrivenMinimap(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawMissionTracker(float ScreenW, float ScreenH);
	void DrawContextualInteractionPrompt(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawJutsuBanner(float ScreenW, float ScreenH);

	void DrawAnimePanel(float X, float Y, float W, float H, const FLinearColor& BgColor, const FLinearColor& BorderColor, float BorderThickness = 1.5f);

	// Target Info State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveTargetName = TEXT("Kakashi Hatake (Sharingan)");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveBossSubtitle = TEXT("Copy Ninja of the Leaf");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float ActiveTargetHealthPercent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float GhostTargetHealthPercent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 BossCurrentPhase = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 BossTotalPhases = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float BossStaggerPercent = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	bool bBossStaggered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	bool bHasActiveTarget = false;

	// Combo State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	int32 CurrentComboHits = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString StyleRank = TEXT("C");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float CurrentComboScore = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float ComboDecayTimer = 0.0f;

	// Mission State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveMissionTitle = TEXT("PATH OF THE HOKAGE");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveMissionObjective = TEXT("Master Rasengan & Inspect Golden Street");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveMissionProgress = TEXT("Progress: 1 / 3");

	// Interaction Prompt State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	bool bShowInteractionPrompt = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveInteractionPrompt = TEXT("[F] TALK — Teuchi (Ichiraku Ramen)");

	// Jutsu / Awakening Banner State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	FString ActiveBannerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float BannerTimer = 0.0f;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveHUDWidget;
};
