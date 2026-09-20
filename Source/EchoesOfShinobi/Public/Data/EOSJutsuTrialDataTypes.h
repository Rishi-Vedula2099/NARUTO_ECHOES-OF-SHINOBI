#pragma once

#include "../CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSJutsuTrialDataTypes.generated.h"

/**
 * State of Jutsu Eligibility & Learning Trial
 */
UENUM(BlueprintType)
enum class EEOSJutsuTrialState : uint8
{
	Locked       UMETA(DisplayName = "Locked (Not Eligible)"),
	Eligible     UMETA(DisplayName = "Eligible (Milestone Reached)"),
	Trial        UMETA(DisplayName = "Trial Active"),
	Practice     UMETA(DisplayName = "Practice / Retained XP"),
	Mastered     UMETA(DisplayName = "Mastered / Unlocked"),
	Refinement   UMETA(DisplayName = "Refinement Active")
};

/**
 * Learning Methods for Jutsu Acquisition
 */
UENUM(BlueprintType)
enum class EEOSLearningMethod : uint8
{
	HandSign          UMETA(DisplayName = "Hand-Sign Recognition & Execution"),
	ChakraControl     UMETA(DisplayName = "Chakra Control / Shaping"),
	ElementControl    UMETA(DisplayName = "Element / Nature Transformation"),
	WeaponTraining    UMETA(DisplayName = "Weapon / Precision Timing"),
	SummoningContract UMETA(DisplayName = "Summoning Contract & Combat Proof"),
	SageTraining      UMETA(DisplayName = "Sage Training / Nature Energy"),
	BloodlineForm     UMETA(DisplayName = "Bloodline Form / Kekkei Genkai"),
	Mentor            UMETA(DisplayName = "Mentor-Led Teaching"),
	CombatDiscovery   UMETA(DisplayName = "Combat Discovery Under Pressure"),
	ScrollResearch    UMETA(DisplayName = "Scroll Research / Forbidden Jutsu"),
	BossMemory        UMETA(DisplayName = "Boss Memory / Reproduce Counter")
};

/**
 * Advancement Bands
 */
UENUM(BlueprintType)
enum class EEOSAdvancementBand : uint8
{
	BandA_Foundation UMETA(DisplayName = "Band A: Academy / Foundation"),
	BandB_Core       UMETA(DisplayName = "Band B: Established Genin / Core"),
	BandC_Advanced   UMETA(DisplayName = "Band C: Chunin / Jonin Advanced"),
	BandD_Mastery    UMETA(DisplayName = "Band D: Signature / Mastery"),
	BandE_Form       UMETA(DisplayName = "Band E: Form-Exclusive"),
	BandF_Legendary  UMETA(DisplayName = "Band F: Late Game / Legendary")
};

/**
 * Mastery Levels (0 to 5)
 */
UENUM(BlueprintType)
enum class EEOSMasteryBand : uint8
{
	Level0_Unlearned UMETA(DisplayName = "Level 0: Unlearned"),
	Level1_Learned   UMETA(DisplayName = "Level 1: Learned (Base Move Available)"),
	Level2_Familiar  UMETA(DisplayName = "Level 2: Familiar (-5% Chakra Cost)"),
	Level3_Trained   UMETA(DisplayName = "Level 3: Trained (-10% Cast Time)"),
	Level4_Expert    UMETA(DisplayName = "Level 4: Expert (Advanced Combo Utility)"),
	Level5_Mastered  UMETA(DisplayName = "Level 5: Mastered (Signature Refinement)")
};

/**
 * Hand Signs for Ninjutsu
 */
UENUM(BlueprintType)
enum class EEOSHandSign : uint8
{
	Ram,
	Serpent,
	Tiger,
	Boar,
	Dog,
	Dragon,
	Hare,
	Horse,
	Monkey,
	Ox,
	Rat,
	Rooster
};

/**
 * Hand Sign Sequence Definition
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSHandSignSequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSign")
	FString SequenceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSign")
	TArray<EEOSHandSign> RequiredSigns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSign")
	float ExecutionWindowSeconds = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSign")
	int32 RecognitionDifficulty = 1; // 1 to 10
};

/**
 * Chakra Control Gauge Model
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSChakraControlModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChakraControl")
	float TargetGaugeMin = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChakraControl")
	float TargetGaugeMax = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChakraControl")
	float StabilityDecayRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChakraControl")
	float HoldDurationRequired = 2.0f;
};

/**
 * Comprehensive Jutsu Data Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSJutsuTrialRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString JutsuID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString JutsuName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString EraID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString AdvancementNodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	EEOSAdvancementBand AdvancementBand = EEOSAdvancementBand::BandA_Foundation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	EEOSLearningMethod LearningMethod = EEOSLearningMethod::HandSign;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	EEOSJutsuTrialState TrialState = EEOSJutsuTrialState::Locked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FEOSHandSignSequence HandSignSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FEOSChakraControlModel ChakraModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	EEOSMasteryBand MasteryBand = EEOSMasteryBand::Level0_Unlearned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	float PracticeXP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	bool bIsEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JutsuRecord")
	FString ContinuityStatus = "CANON_STORY";
};
