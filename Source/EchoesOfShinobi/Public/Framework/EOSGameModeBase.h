#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOSGameModeBase.generated.h"

class AEOSCharacterBase;
class AEOSPlayerController;
class AEOSShinobiHUD;

/**
 * Authoritative GameMode for NARUTO: ECHOES OF SHINOBI.
 * Enforces BP_ShinobiCharacter as DefaultPawnClass, eliminating DefaultPawn0.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEOSGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	void LogRuntimeBootState(AController* Controller, APawn* SpawnedPawn);
	void PerformRuntimeQualityAssertions(APawn* SpawnedPawn, APlayerController* Controller);
};
