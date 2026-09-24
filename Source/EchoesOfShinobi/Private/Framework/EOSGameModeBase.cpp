#include "../../Public/Framework/EOSGameModeBase.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Framework/EOSPlayerController.h"
#include "../../Public/UI/EOSShinobiHUD.h"
#include "../../Public/Environment/EOSKonohaVillageBuilder.h"
#include "../../Public/Environment/EOSKonohaTrainingGroundsBuilder.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "../../EchoesOfShinobi.h"

AEOSGameModeBase::AEOSGameModeBase()
{
	// 1. Authoritative DefaultPawnClass Resolution:
	// Prioritize BP_ShinobiCharacter, safely fall back to AEOSCharacterBase C++ class.
	// Strictly eliminates DefaultPawn0.
	static ConstructorHelpers::FClassFinder<APawn> ShinobiBPFinder(TEXT("/Game/Characters/BP_ShinobiCharacter.BP_ShinobiCharacter_C"));
	if (ShinobiBPFinder.Class != nullptr)
	{
		DefaultPawnClass = ShinobiBPFinder.Class;
	}
	else
	{
		DefaultPawnClass = AEOSCharacterBase::StaticClass();
	}

	// 2. Authoritative Controller, HUD, and PlayerState
	PlayerControllerClass = AEOSPlayerController::StaticClass();
	HUDClass = AEOSShinobiHUD::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
}

void AEOSGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogEOSCore, Log, TEXT("EOSGameModeBase::InitGame initializing for map: %s"), *MapName);
}

void AEOSGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Ensure Authored Konoha Village District environment is spawned if not present
	if (GetWorld())
	{
		TArray<AActor*> FoundVillageBuilders;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEOSKonohaVillageBuilder::StaticClass(), FoundVillageBuilders);
		if (FoundVillageBuilders.Num() == 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<AEOSKonohaVillageBuilder>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			UE_LOG(LogEOSCore, Log, TEXT("EOSGameModeBase: Spawned AEOSKonohaVillageBuilder (500m x 500m Authored District) into active level."));
		}
	}
}

void AEOSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		APawn* PlayerPawn = NewPlayer->GetPawn();
		LogRuntimeBootState(NewPlayer, PlayerPawn);
		PerformRuntimeQualityAssertions(PlayerPawn, NewPlayer);
	}
}

AActor* AEOSGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	// Find deterministic training grounds PlayerStart
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() > 0)
	{
		for (AActor* Start : PlayerStarts)
		{
			if (Start->ActorHasTag(TEXT("KonohaTrainingStart")))
			{
				return Start;
			}
		}
		return PlayerStarts[0];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AEOSGameModeBase::LogRuntimeBootState(AController* Controller, APawn* SpawnedPawn)
{
	const FString MapName = GetWorld() ? GetWorld()->GetMapName() : TEXT("Unknown");
	const FString SpawnName = SpawnedPawn ? *SpawnedPawn->GetActorLocation().ToString() : TEXT("No Pawn");

	const UClass* PawnClassToLog = SpawnedPawn ? SpawnedPawn->GetClass() : DefaultPawnClass.Get();
	const UClass* ControllerClassToLog = Controller ? Controller->GetClass() : PlayerControllerClass.Get();

	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
	UE_LOG(LogEOSCore, Log, TEXT("EOS RUNTIME BOOT"));
	UE_LOG(LogEOSCore, Log, TEXT("GameMode: %s"), *GetNameSafe(this));
	UE_LOG(LogEOSCore, Log, TEXT("Pawn: %s"), *GetNameSafe(PawnClassToLog));
	UE_LOG(LogEOSCore, Log, TEXT("Controller: %s"), *GetNameSafe(ControllerClassToLog));
	UE_LOG(LogEOSCore, Log, TEXT("HUD: %s"), *GetNameSafe(HUDClass));
	UE_LOG(LogEOSCore, Log, TEXT("Map: %s"), *MapName);
	UE_LOG(LogEOSCore, Log, TEXT("Spawn: %s"), *SpawnName);
	UE_LOG(LogEOSCore, Log, TEXT("=================================================="));
}

void AEOSGameModeBase::PerformRuntimeQualityAssertions(APawn* SpawnedPawn, APlayerController* Controller)
{
	if (!SpawnedPawn)
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: SpawnedPawn is NULL!"));
		return;
	}

	if (SpawnedPawn->IsA(ADefaultPawn::StaticClass()))
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: DefaultPawn0 was spawned instead of AEOSCharacterBase!"));
	}

	AEOSCharacterBase* ShinobiChar = Cast<AEOSCharacterBase>(SpawnedPawn);
	if (!ShinobiChar)
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: Pawn is not derived from AEOSCharacterBase! Class: %s"), *GetNameSafe(SpawnedPawn->GetClass()));
		return;
	}

	if (!ShinobiChar->FollowCamera)
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: FollowCamera is missing on Shinobi character!"));
	}

	if (!ShinobiChar->GetCharacterMovement())
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: CharacterMovementComponent is missing on Shinobi character!"));
	}

	if (!Controller)
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: PlayerController is NULL!"));
	}

	if (!Controller->GetHUD())
	{
		UE_LOG(LogEOSCore, Error, TEXT("CRITICAL QUALITY FAILURE: HUD is NULL on PlayerController!"));
	}
}
