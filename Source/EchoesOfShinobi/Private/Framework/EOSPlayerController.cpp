#include "../../Public/Framework/EOSPlayerController.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "EnhancedInputSubsystems.h"
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
}

void AEOSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
