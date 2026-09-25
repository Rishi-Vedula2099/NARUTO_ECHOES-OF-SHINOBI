#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/EOSPresentationDataTypes.h"
#include "EOSVillageNPC.generated.h"


UENUM(BlueprintType)
enum class EEOSNPCBehavior : uint8
{
	Idle_Talking     UMETA(DisplayName = "Idle Talking"),
	Patrolling_Road  UMETA(DisplayName = "Patrolling Road"),
	Inspecting_Shop  UMETA(DisplayName = "Inspecting Shop"),
	Guarding_Gate    UMETA(DisplayName = "Guarding Gate")
};

UENUM(BlueprintType)
enum class EEOSNPCSimulationLOD : uint8
{
	Near       UMETA(DisplayName = "Near (<15m) Full Simulation"),
	Medium     UMETA(DisplayName = "Medium (15-40m) Simplified"),
	Far        UMETA(DisplayName = "Far (40-100m) Low Frequency"),
	VeryFar    UMETA(DisplayName = "Very Far (>100m) Culled")
};

/**
 * Living Village NPC with 4-Tier distance performance simulation.
 * Handles ambient village population, LOD scaling for 60 FPS budget,
 * head-tracking toward player, and interaction handoff (e.g. mission start).
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSVillageNPC : public ACharacter
{
	GENERATED_BODY()

public:
	AEOSVillageNPC();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	EEOSNPCType NPCType = EEOSNPCType::Villager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	EEOSNPCBehavior CurrentBehavior = EEOSNPCBehavior::Idle_Talking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	FEOSNPCSimulationProfile SimulationProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EOS|NPC")
	EEOSNPCSimulationLOD CurrentLOD = EEOSNPCSimulationLOD::Near;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	FString NPCID = "NPC_KONOHA_VILLAGER_01";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	FText NPCName = FText::FromString("Leaf Villager");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|NPC")
	FText DialogueText = FText::FromString("Good morning! Peace has returned to the village.");

	UFUNCTION(BlueprintCallable, Category = "EOS|NPC")
	FString Interact(AActor* Interactor);

	UFUNCTION(BlueprintPure, Category = "EOS|NPC")
	static FString ResolveInteractionForType(EEOSNPCType InType);

protected:
	virtual void BeginPlay() override;
	void UpdateDistanceSimulationLOD();

	float AccumulatedTime = 0.0f;
};
