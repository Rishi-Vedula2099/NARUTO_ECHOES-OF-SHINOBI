#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSRankSageDataTypes.h"
#include "EOSTailedBeastDataTypes.generated.h"

/**
 * The 9 Canonical Tailed Beasts
 */
UENUM(BlueprintType)
enum class EEOSTailedBeastID : uint8
{
	Shukaku  UMETA(DisplayName = "1-Tail: Shukaku"),
	Matatabi UMETA(DisplayName = "2-Tails: Matatabi"),
	Isobu    UMETA(DisplayName = "3-Tails: Isobu"),
	SonGoku  UMETA(DisplayName = "4-Tails: Son Gokū"),
	Kokuo    UMETA(DisplayName = "5-Tails: Kokuō"),
	Saiken   UMETA(DisplayName = "6-Tails: Saiken"),
	Chomei   UMETA(DisplayName = "7-Tails: Chōmei"),
	Gyuki    UMETA(DisplayName = "8-Tails: Gyūki"),
	Kurama   UMETA(DisplayName = "9-Tails: Kurama")
};

/**
 * Resonance Progression States
 */
UENUM(BlueprintType)
enum class EEOSTailedBeastResonanceState : uint8
{
	Unsynced     UMETA(DisplayName = "Unsynced / Locked"),
	Aware        UMETA(DisplayName = "Aware (Initial Contact)"),
	Responsive   UMETA(DisplayName = "Responsive (Partial Manifestation)"),
	Resonant     UMETA(DisplayName = "Resonant (Transformation Active)"),
	Synchronized UMETA(DisplayName = "Synchronized (Chakra Link Mode)"),
	Mastered     UMETA(DisplayName = "Mastered (Complete Mastery)")
};

/**
 * Tailed Beast Definition Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSTailedBeastDefinitionRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	FString BeastID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	int32 BeastNumber = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	EEOSChakraElement PrimaryElement = EEOSChakraElement::Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	float BaseRemnantPower = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	float BaseChakraContribution = 500.0f;
};

/**
 * Compatibility Record between Character, Era, Form, and Beast
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSTailedBeastCompatibilityRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	EEOSCharacterEra Era = EEOSCharacterEra::ERA_GENIN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	FString FormID = "FORM_BASE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	FString TailedBeastID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TailedBeast")
	EEOSSageCompatibility Compatibility = EEOSSageCompatibility::NONE;
};

/**
 * Remnant Power & Resonance Progression Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSRemnantProgressionRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	FString TailedBeastID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	int32 CurrentTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	float RemnantPowerValue = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	float ChakraIncrease = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	float ChakraRegenModifier = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	EEOSTailedBeastResonanceState ResonanceState = EEOSTailedBeastResonanceState::Unsynced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remnant")
	float CurrentResonanceXP = 0.0f;
};

/**
 * Transformation Runtime State
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSTailedBeastTransformationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FString ActiveBeastID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FString FormID = "FORM_KURAMA_LINK";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	EEOSTailedBeastResonanceState ResonanceState = EEOSTailedBeastResonanceState::Resonant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	float DurationSeconds = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bIsActive = false;
};
