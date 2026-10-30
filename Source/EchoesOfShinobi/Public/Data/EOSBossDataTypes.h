#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSAIDataTypes.h"
#include "EOSBossDataTypes.generated.h"

/**
 * Boss Encounter FSM States.
 * Contract Section 22: entrance, health bar, phase system, telegraph, attack,
 * movement, VFX, hit reaction, stagger, recovery, transition, defeat.
 */
UENUM(BlueprintType)
enum class EEOSBossState : uint8
{
	Inactive       UMETA(DisplayName = "Inactive — Not Yet Triggered"),
	Entrance       UMETA(DisplayName = "Entrance — Cinematic Introduction"),
	Idle           UMETA(DisplayName = "Idle — Awaiting Action Decision"),
	Telegraph      UMETA(DisplayName = "Telegraph — Warning Player of Next Attack"),
	Attacking      UMETA(DisplayName = "Attacking — Executing Attack Pattern"),
	Recovery       UMETA(DisplayName = "Recovery — Post-Attack Vulnerable Window"),
	Repositioning  UMETA(DisplayName = "Repositioning — Tactical Movement"),
	HitReaction    UMETA(DisplayName = "HitReaction — Responding to Player Damage"),
	Staggered      UMETA(DisplayName = "Staggered — Vulnerability Window for Player"),
	PhaseTransition UMETA(DisplayName = "PhaseTransition — Changing Combat Phase"),
	Defeated       UMETA(DisplayName = "Defeated — Boss HP Depleted")
};

/**
 * Boss Attack Type Classification.
 */
UENUM(BlueprintType)
enum class EEOSBossAttackType : uint8
{
	MeleeSwing         UMETA(DisplayName = "Melee Swing — Close Range Physical"),
	MeleeCombo         UMETA(DisplayName = "Melee Combo — Multi-Hit Close Range"),
	RangedProjectile   UMETA(DisplayName = "Ranged Projectile — Distance Attack"),
	AreaOfEffect       UMETA(DisplayName = "AoE — Area Denial / Zone"),
	GrabThrow          UMETA(DisplayName = "Grab/Throw — Unblockable Contact"),
	JutsuSignature     UMETA(DisplayName = "Jutsu Signature — Boss's Unique Technique"),
	JutsuUltimate      UMETA(DisplayName = "Jutsu Ultimate — Phase Transition Climax")
};

/**
 * Boss Telegraph Visual Type — how the boss warns the player.
 * Contract Section 23: Bosses must expose telegraphs, reaction windows,
 * recovery, weaknesses, counter opportunities.
 */
UENUM(BlueprintType)
enum class EEOSBossTelegraphType : uint8
{
	WindUp             UMETA(DisplayName = "Wind-Up — Anticipation Pose"),
	GroundIndicator    UMETA(DisplayName = "Ground Indicator — Highlighted Zone"),
	ChakraFlare        UMETA(DisplayName = "Chakra Flare — Visual Chakra Buildup"),
	AudioCue           UMETA(DisplayName = "Audio Cue — Sound Warning"),
	PostureTell        UMETA(DisplayName = "Posture Tell — Body Language Shift")
};

/**
 * Individual Boss Attack Pattern Descriptor.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossAttackPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	FString PatternID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	FString PatternName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	EEOSBossAttackType AttackType = EEOSBossAttackType::MeleeSwing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	EEOSBossTelegraphType TelegraphType = EEOSBossTelegraphType::WindUp;

	/** Duration of telegraph warning before the attack lands (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	float TelegraphDuration = 0.8f;

	/** Base damage dealt if the attack connects. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	float BaseDamage = 80.0f;

	/** Range in Unreal units at which the attack is effective. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	float EffectiveRange = 250.0f;

	/** Cooldown between uses (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	float Cooldown = 3.0f;

	/** Recovery window after attack finishes (seconds) — player counterattack opportunity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	float RecoveryDuration = 1.2f;

	/** Can this attack be blocked/guarded? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	bool bCanBeBlocked = true;

	/** Can this attack be dodged? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	bool bCanBeDodged = true;

	/** Element (for elemental counter-play). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	EEOSChakraElement Element = EEOSChakraElement::Water;

	/** Niagara VFX soft reference for this attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	TSoftObjectPtr<class UNiagaraSystem> AttackVFXAsset;

	/** Available phases (empty = available in all phases). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
	TArray<int32> AvailableInPhases;
};

/**
 * Boss Phase Definition.
 * Each phase has its own HP threshold, available attack patterns,
 * speed modifiers, and transition VFX.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossPhaseDefinition
{
	GENERATED_BODY()

	/** Phase number (1-indexed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	int32 PhaseNumber = 1;

	/** Descriptive name for this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	FString PhaseName = TEXT("Phase 1");

	/** HP percentage threshold to enter this phase (1.0 = full HP). Boss enters
	 *  this phase when current HP% drops below the PREVIOUS phase threshold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	float HPThresholdPercent = 1.0f;

	/** Attack speed multiplier applied during this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	float AttackSpeedMultiplier = 1.0f;

	/** Movement speed multiplier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	float MovementSpeedMultiplier = 1.0f;

	/** Damage multiplier applied during this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	float DamageMultiplier = 1.0f;

	/** Stagger threshold for this phase (cumulative damage to trigger stagger). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	float StaggerThreshold = 300.0f;

	/** IDs of attack patterns available during this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	TArray<FString> AvailablePatternIDs;

	/** Niagara VFX for phase transition. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPhase")
	TSoftObjectPtr<class UNiagaraSystem> TransitionVFXAsset;
};

/**
 * Boss Stagger Model — cumulative damage tracking for stagger windows.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossStaggerModel
{
	GENERATED_BODY()

	/** Current accumulated stagger damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	float CurrentStaggerDamage = 0.0f;

	/** Threshold to trigger stagger (from current phase). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	float StaggerThreshold = 300.0f;

	/** Duration of stagger vulnerability window (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	float StaggerDuration = 3.0f;

	/** Decay rate of stagger damage per second when not being hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	float StaggerDecayRate = 20.0f;

	/** Is boss currently staggered? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	bool bIsStaggered = false;

	/** Remaining stagger time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stagger")
	float RemainingStaggerTime = 0.0f;

	/** How full is the stagger gauge (0.0 to 1.0). */
	float GetStaggerPercent() const
	{
		return StaggerThreshold > 0.0f ? FMath::Clamp(CurrentStaggerDamage / StaggerThreshold, 0.0f, 1.0f) : 0.0f;
	}
};

/**
 * Complete Boss Encounter Definition — the authoritative data-driven boss profile.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossEncounterDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	FString BossName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	FString BossSubtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	EEOSAITier AITier = EEOSAITier::MajorBoss;

	/** Max HP for this encounter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	float MaxHP = 5000.0f;

	/** Base effective defense. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	float BaseDefense = 80.0f;

	/** Primary chakra element for elemental counter-play. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	EEOSChakraElement PrimaryElement = EEOSChakraElement::Water;

	/** Ordered phase definitions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	TArray<FEOSBossPhaseDefinition> Phases;

	/** All available attack patterns for this boss. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	TArray<FEOSBossAttackPattern> AttackPatterns;

	/** Stagger model parameters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	FEOSBossStaggerModel StaggerModel;

	/** Soft reference to production skeletal mesh. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	TSoftObjectPtr<class USkeletalMesh> ProductionMesh;

	/** Soft reference to entrance cinematic VFX. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	TSoftObjectPtr<class UNiagaraSystem> EntranceVFXAsset;

	/** Soft reference to defeat VFX. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossDefinition")
	TSoftObjectPtr<class UNiagaraSystem> DefeatVFXAsset;
};

/**
 * Runtime Boss Encounter State — tracks the live state of an active encounter.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossEncounterState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	EEOSBossState CurrentFSMState = EEOSBossState::Inactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	int32 CurrentPhase = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	float CurrentHP = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	float MaxHP = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	FEOSBossStaggerModel StaggerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	FString ActivePatternID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	float StateTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	int32 TotalDamageDealt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	int32 TotalDamageReceived = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossState")
	int32 PlayerDeathCount = 0;

	float GetHealthPercent() const
	{
		return MaxHP > 0.0f ? FMath::Clamp(CurrentHP / MaxHP, 0.0f, 1.0f) : 0.0f;
	}
};

/**
 * Boss Encounter Result — recorded after boss defeat or player failure.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossEncounterResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	FString BossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	bool bPlayerVictory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	float EncounterDurationSeconds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	int32 TotalDamageDealt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	int32 TotalDamageReceived = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	int32 MaxComboHits = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	int32 PlayerDeathCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	FString CombatGrade = TEXT("C");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossResult")
	int32 HitScore = 0;
};
