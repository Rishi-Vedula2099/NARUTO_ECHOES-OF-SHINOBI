#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSBossDataTypes.h"
#include "../Data/EOSAIDataTypes.h"
#include "EOSBossEncounterSubsystem.generated.h"

class AEOSBossCharacter;

/**
 * GameInstanceSubsystem orchestrating boss encounter lifecycle.
 * Contract Section 22: entrance, health bar, phase system, telegraph, attack,
 * movement, VFX, hit reaction, stagger, recovery, transition, defeat.
 * Integrates with UEOSAdaptiveAISubsystem for nemesis memory and adaptation.
 * Integrates with AEOSShinobiHUD for boss health/phase/stagger display.
 *
 * GOLDEN BOSS: Zabuza Momochi — Demon of the Hidden Mist.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSBossEncounterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSBossEncounterSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ──── Encounter Registry ────

	/** Register a boss encounter definition in the global registry. */
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|BossEncounter")
	void RegisterBossEncounter(const FEOSBossEncounterDefinition& Definition);

	/** Retrieve a registered boss encounter definition by ID. */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	FEOSBossEncounterDefinition GetBossEncounterDefinition(const FString& BossID) const;

	/** Check if a boss encounter is registered. */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	bool IsBossRegistered(const FString& BossID) const;

	/** Get all registered boss IDs. */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	TArray<FString> GetAllRegisteredBossIDs() const;

	// ──── Golden Boss Initialization ────

	/** Initialize the golden boss registry with Zabuza Momochi encounter data. */
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|BossEncounter")
	void InitializeGoldenBossRegistry();

	// ──── Active Encounter Management ────

	/** Start a boss encounter by ID. Returns the created boss character. */
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|BossEncounter")
	AEOSBossCharacter* StartEncounter(const FString& BossID, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	/** Get the currently active boss character (nullptr if no encounter is active). */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	AEOSBossCharacter* GetActiveBoss() const { return ActiveBossCharacter; }

	/** Is an encounter currently active? */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	bool IsEncounterActive() const { return ActiveBossCharacter != nullptr; }

	/** End the current encounter and record results. */
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|BossEncounter")
	FEOSBossEncounterResult EndEncounter();

	// ──── Encounter History ────

	/** Get encounter result history for a specific boss. */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	TArray<FEOSBossEncounterResult> GetEncounterHistory(const FString& BossID) const;

	/** Get total number of encounters (all bosses). */
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|BossEncounter")
	int32 GetTotalEncounterCount() const;

	// ──── Adaptive AI Integration ────

	/** Push encounter result to AdaptiveAISubsystem boss memory. */
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|BossEncounter")
	void RecordToAdaptiveAI(const FEOSBossEncounterResult& Result);

private:
	UPROPERTY()
	TMap<FString, FEOSBossEncounterDefinition> BossEncounterRegistry;

	UPROPERTY()
	TArray<FEOSBossEncounterResult> EncounterHistory;

	UPROPERTY()
	TObjectPtr<AEOSBossCharacter> ActiveBossCharacter;

	/** Build the canonical Zabuza Momochi encounter data. */
	FEOSBossEncounterDefinition BuildZabuzaEncounterDefinition() const;
};
