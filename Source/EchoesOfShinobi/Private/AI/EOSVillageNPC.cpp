#include "../../Public/AI/EOSVillageNPC.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../EchoesOfShinobi.h"

AEOSVillageNPC::AEOSVillageNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(38.0f, 90.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 160.0f;
}

void AEOSVillageNPC::BeginPlay()
{
	Super::BeginPlay();

	// Initialize dialogue and ID based on role
	switch (NPCType)
	{
	case EEOSNPCType::ChuninGuard:
		NPCID = TEXT("NPC_CHUNIN_GUARD_GATE");
		NPCName = FText::FromString("Chunin Gate Guard");
		DialogueText = FText::FromString("Shinobi! Head to the training grounds immediately for the tactical investigation!");
		break;
	case EEOSNPCType::Shopkeeper:
		NPCID = TEXT("NPC_ICHIRAKU_TEUCHI");
		NPCName = FText::FromString("Teuchi (Ichiraku Ramen)");
		DialogueText = FText::FromString("Welcome! Try our special Miso Chashu Ramen!");
		break;
	case EEOSNPCType::AcademyStudent:
		NPCID = TEXT("NPC_ACADEMY_KONOHAMARU");
		NPCName = FText::FromString("Academy Student");
		DialogueText = FText::FromString("One day I'm going to become the greatest Hokage!");
		break;
	case EEOSNPCType::Villager:
	default:
		NPCID = TEXT("NPC_LEAF_VILLAGER");
		NPCName = FText::FromString("Leaf Villager");
		DialogueText = FText::FromString("Konohagakure is lively today. The cherry blossoms are in full bloom.");
		break;
	}
}

void AEOSVillageNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedTime += DeltaTime;
	UpdateDistanceSimulationLOD();

	// Near range: Perform head tracking toward player pawn
	if (CurrentLOD == EEOSNPCSimulationLOD::Near && SimulationProfile.bEnableHeadTrackingInNearRange)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			const float DistToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
			if (DistToPlayer < 600.0f)
			{
				FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
				LookRot.Pitch = 0.0f;
				LookRot.Roll = 0.0f;
				SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookRot, DeltaTime, 4.0f));
			}
		}
	}
}

void AEOSVillageNPC::UpdateDistanceSimulationLOD()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		CurrentLOD = EEOSNPCSimulationLOD::Far;
		return;
	}

	const float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

	if (Distance < SimulationProfile.NearDistance)
	{
		CurrentLOD = EEOSNPCSimulationLOD::Near;
		PrimaryActorTick.TickInterval = SimulationProfile.NearTickInterval;
	}
	else if (Distance < SimulationProfile.MediumDistance)
	{
		CurrentLOD = EEOSNPCSimulationLOD::Medium;
		PrimaryActorTick.TickInterval = SimulationProfile.MediumTickInterval;
	}
	else if (Distance < SimulationProfile.FarDistance)
	{
		CurrentLOD = EEOSNPCSimulationLOD::Far;
		PrimaryActorTick.TickInterval = SimulationProfile.FarTickInterval;
	}
	else
	{
		CurrentLOD = EEOSNPCSimulationLOD::VeryFar;
		PrimaryActorTick.TickInterval = 0.5f; // Very low frequency when far
	}
}

FString AEOSVillageNPC::ResolveInteractionForType(EEOSNPCType InType)
{
	return ResolveNPCInteraction(InType);
}

FString AEOSVillageNPC::Interact(AActor* Interactor)
{
	UE_LOG(LogEOSCore, Log, TEXT("NPC Interaction triggered with %s (%s): %s"),
		*NPCID, *NPCName.ToString(), *DialogueText.ToString());

	return ResolveInteractionForType(NPCType);
}
