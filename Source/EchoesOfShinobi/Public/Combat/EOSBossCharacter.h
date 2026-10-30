#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/EOSBossDataTypes.h"
#include "../Data/EOSAIDataTypes.h"
#include "EOSBossCharacter.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

/**
 * Authoritative Boss Character Actor for NARUTO: ECHOES OF SHINOBI.
 * Contract Section 22: entrance, health bar, phase system, telegraph, attack,
 * movement, VFX, hit reaction, stagger, recovery, transition, defeat.
 * Contract Section 23: fairness — no input reading, no cooldown ignoring, 
 * exposes telegraphs, reaction windows, recovery, weaknesses, counter opportunities.
 *
 * GOLDEN BOSS: Zabuza Momochi — Demon of the Hidden Mist.
 * First production boss validating the complete boss encounter pipeline.
 */
UCLASS()
class ECHOESOFSHINOBI_API AEOSBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEOSBossCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ──── Boss Encounter Lifecycle ────

	/** Initialize boss from encounter definition data. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	void InitializeFromEncounterDefinition(const FEOSBossEncounterDefinition& InDefinition);

	/** Trigger boss entrance sequence. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	void TriggerEntrance();

	/** Transition FSM to a new state with validation. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	bool TransitionToState(EEOSBossState NewState);

	/** Force a phase transition to the specified phase. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	bool TransitionToPhase(int32 TargetPhase);

	/** Execute an attack pattern by ID. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	bool ExecuteAttackPattern(const FString& PatternID);

	/** Apply stagger damage (contributes to stagger gauge). */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	void ApplyStaggerDamage(float StaggerDamageAmount);

	/** Reset the encounter to initial state. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Boss")
	void ResetEncounter();

	// ──── Query Interface ────

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	EEOSBossState GetCurrentFSMState() const { return EncounterState.CurrentFSMState; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	int32 GetCurrentPhase() const { return EncounterState.CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	int32 GetTotalPhases() const { return EncounterDefinition.Phases.Num(); }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	float GetHealthPercent() const { return EncounterState.GetHealthPercent(); }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	float GetCurrentHP() const { return EncounterState.CurrentHP; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	float GetMaxHP() const { return EncounterState.MaxHP; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	float GetStaggerPercent() const { return EncounterState.StaggerState.GetStaggerPercent(); }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	bool IsStaggered() const { return EncounterState.StaggerState.bIsStaggered; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	bool IsDefeated() const { return EncounterState.CurrentFSMState == EEOSBossState::Defeated; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FString GetBossName() const { return EncounterDefinition.BossName; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FString GetBossSubtitle() const { return EncounterDefinition.BossSubtitle; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FEOSBossEncounterState GetEncounterState() const { return EncounterState; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FEOSBossEncounterDefinition GetEncounterDefinition() const { return EncounterDefinition; }

	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FEOSBossEncounterResult GenerateEncounterResult() const;

	/** Get the attack pattern data by ID. */
	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	FEOSBossAttackPattern GetAttackPatternByID(const FString& PatternID) const;

	/** Get attack patterns available in the current phase. */
	UFUNCTION(BlueprintPure, Category = "EOS|Boss")
	TArray<FEOSBossAttackPattern> GetCurrentPhaseAttackPatterns() const;

	// ──── Fairness Enforcement (Contract Section 23) ────

	/** Validate that the boss is NOT reading frame-0 inputs. Always returns true. */
	UFUNCTION(BlueprintPure, Category = "EOS|Boss|Fairness")
	bool ValidateNoInputReading() const { return !bAttemptsInputReading; }

	/** Validate that attack cooldowns are being respected. */
	UFUNCTION(BlueprintPure, Category = "EOS|Boss|Fairness")
	bool ValidateCooldownRespected() const;

	/** Validate that hit reactions and recovery are properly applied. */
	UFUNCTION(BlueprintPure, Category = "EOS|Boss|Fairness")
	bool ValidateHitReactionRespected() const;

	// ──── Data ────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Boss")
	FEOSBossEncounterDefinition EncounterDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS|Boss")
	FEOSBossEncounterState EncounterState;

protected:
	virtual void BeginPlay() override;

	// FSM Tick Handlers
	void TickEntrance(float DeltaTime);
	void TickIdle(float DeltaTime);
	void TickTelegraph(float DeltaTime);
	void TickAttacking(float DeltaTime);
	void TickRecovery(float DeltaTime);
	void TickHitReaction(float DeltaTime);
	void TickStaggered(float DeltaTime);
	void TickPhaseTransition(float DeltaTime);

	// Internal helpers
	void UpdateStaggerDecay(float DeltaTime);
	void CheckPhaseTransition();
	void SelectNextAttackPattern();
	void UpdateHUDState();

	/** Fairness flags — always false; contract explicitly forbids setting true. */
	UPROPERTY()
	bool bAttemptsInputReading = false;

	/** Cooldown tracking per-pattern. */
	UPROPERTY()
	TMap<FString, float> PatternCooldownTimers;

	/** Time spent in current FSM state. */
	UPROPERTY()
	float CurrentStateTimer = 0.0f;

	/** Elapsed encounter time. */
	UPROPERTY()
	float EncounterElapsedTime = 0.0f;
};
