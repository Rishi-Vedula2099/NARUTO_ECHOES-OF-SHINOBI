#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EOSShinobiHUD.generated.h"

class AEOSCharacterBase;
class UUserWidget;

/**
 * Authoritative Shinobi HUD for NARUTO: ECHOES OF SHINOBI.
 * Features an immediate Canvas rendering pipeline for guaranteed runtime display,
 * along with UMG widget instantiation support.
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

	// Combo Counter Increment
	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void RegisterHit(int32 HitCount, float HitScore);

	// HUD Visibility / Presentation fade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|UI")
	float HUDOpacity = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "EOS|UI")
	void SetHUDOpacity(float InOpacity) { HUDOpacity = FMath::Clamp(InOpacity, 0.0f, 1.0f); }

protected:
	void DrawHealthAndChakraBars(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawJutsuAndUltimateSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawSquadSelectionSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);
	void DrawTargetBossBar(float ScreenW, float ScreenH);
	void DrawComboDisplay(float ScreenW, float ScreenH);
	void DrawDataDrivenMinimap(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi);

	// Target Info State
	FString ActiveTargetName = TEXT("Kakashi Hatake (Sharingan)");
	float ActiveTargetHealthPercent = 1.0f;
	bool bHasActiveTarget = true;

	// Combo State
	int32 CurrentComboHits = 0;
	float CurrentComboScore = 0.0f;
	float ComboDecayTimer = 0.0f;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveHUDWidget;
};
