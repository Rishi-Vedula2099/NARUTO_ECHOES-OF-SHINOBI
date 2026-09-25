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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Enhanced Input Context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
};
