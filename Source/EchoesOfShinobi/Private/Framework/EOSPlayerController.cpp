#include "../../Public/Framework/EOSPlayerController.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Misc/CommandLine.h"
#include "../../EchoesOfShinobi.h"

AEOSPlayerController::AEOSPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void AEOSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Enhanced Input Mapping Context if available
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// Explicitly align camera with avenue (+Y direction, Yaw = 90, Pitch = -5)
	SetControlRotation(FRotator(-5.0f, 90.0f, 0.0f));

	// Automated in-game visual capture verification support
	if (FParse::Param(FCommandLine::Get(), TEXT("CaptureGoldenScreenshot")) ||
	    FParse::Param(FCommandLine::Get(), TEXT("AutoCapture")))
	{
		UE_LOG(LogEOSCore, Log, TEXT("AUTOCAPTURE: Visual verification capture scheduled in 4.0s..."));
		FTimerHandle CaptureTimer;
		GetWorld()->GetTimerManager().SetTimer(CaptureTimer, [this]()
		{
			ConsoleCommand(TEXT("Shot"));
			UE_LOG(LogEOSCore, Log, TEXT("AUTOCAPTURE: Viewport screenshot command dispatched!"));

			FTimerHandle QuitTimer;
			GetWorld()->GetTimerManager().SetTimer(QuitTimer, [this]()
			{
				UE_LOG(LogEOSCore, Log, TEXT("AUTOCAPTURE: Auto-quitting after successful visual capture."));
				ConsoleCommand(TEXT("Quit"));
			}, 2.0f, false);
		}, 4.0f, false);
	}
}

void AEOSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Align controller rotation with avenue facing down toward the Hokage Monument
	SetControlRotation(FRotator(-5.0f, 90.0f, 0.0f));
}

void AEOSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
