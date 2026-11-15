#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOSPlayerController.generated.h"

class UInputMappingContext;

/**
 * Custom PlayerController for NARUTO: ECHOES OF SHINOBI.
 * Bridges Enhanced Input Subsystem, camera controls, and combat commands.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEOSPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	/** Dialogue and interaction queries */
	UFUNCTION(BlueprintPure, Category = "EOS|Dialogue")
	bool IsDialogueActive() const { return bIsDialogueActive; }

	UFUNCTION(BlueprintCallable, Category = "EOS|Dialogue")
	void TriggerInteraction();

	UFUNCTION(BlueprintCallable, Category = "EOS|Dialogue")
	void SelectDialogueChoice(int32 ChoiceIndex);

	UFUNCTION(BlueprintCallable, Category = "EOS|Dialogue")
	void CloseDialogue();

	/** Manual quick save and load for interactive standalone gameplay */
	UFUNCTION(BlueprintCallable, Category = "EOS|Save")
	void QuickSave();

	UFUNCTION(BlueprintCallable, Category = "EOS|Save")
	void QuickLoad();

	/** Automated in-engine testing support for Sprint 01 Standalone Mission */
	void RunAutomatedSprint01MissionSequence();
	void RunAutomatedSaveVerification();

	/** Automated in-engine testing support for Sprint 02 Standalone Mission */
	void RunAutomatedSprint02MissionSequence();
	void RunAutomatedSprint02SaveVerification();

	void EnsureSouthernBackdropAndAtmosphere();

	UFUNCTION()
	void OnMistScoutDefeated(class AEOSMistScout* DefeatedScout);

	UFUNCTION(BlueprintPure, Category = "EOS|Ramen")
	bool HasRamenBuff() const { return bHasRamenBuff; }

	UFUNCTION(BlueprintPure, Category = "EOS|Ramen")
	float GetRamenBuffRemainingDuration() const { return RamenBuffRemainingDuration; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	void OnKeyOnePressed();
	void OnKeyTwoPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Dialogue")
	bool bIsDialogueActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Dialogue")
	int32 ActiveDialogueChoiceCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Dialogue")
	FString CurrentDialogueSpeakerID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Mission")
	bool bOutskirtsEncounterTriggered = false;

	// Sprint 02 Outskirts Gauntlet & Rematch State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Mission")
	bool bGauntletScoutsSpawned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Mission")
	bool bGauntletScoutsResolved = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Mission")
	bool bRematchZabuzaSpawned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Mission")
	int32 DefeatedMistScoutCount = 0;

	// Sprint 02 Ramen Economy Buff State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Ramen")
	bool bHasRamenBuff = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Ramen")
	float RamenBuffRemainingDuration = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|Ramen")
	float ChakraRegenMultiplier = 1.0f;

	// Enhanced Input Context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// Sprint 03 Helper: Safe Viewport Capture
	void CaptureMissionScreenshot(const TCHAR* StepTag);

	// Sprint 02 / 03 Multi-Stage Execution Steps (Clean UObject Member Function Delegates)
	void ExecuteSprint02Stage1();
	void ExecuteSprint02Stage2();
	void ExecuteSprint02Stage3();
	void ExecuteSprint02Stage4();
	void ExecuteSprint02Stage4Capture();
	void ExecuteSprint02Stage5();
	void ExecuteSprint02Stage5Capture();
	void ExecuteSprint02Stage5DefeatScouts();
	void ExecuteSprint02Stage6();
	void ExecuteSprint02Stage6ACapture();
	void ExecuteSprint02Stage6BCapture();
	void ExecuteSprint02Stage6DefeatBoss();
	void ExecuteSprint02Stage7();
	void ExecuteSprint02Stage7Capture();
	void ExecuteSprint02Stage8();
	void RequestCleanExit();

private:
	FTimerHandle StageTimerHandle;
	FTimerHandle SubStageTimerHandle;
	FTimerHandle SubStageTimerHandle2;
};
