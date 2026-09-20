#pragma once

#include "../CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSJutsuTrialDataTypes.h"
#include "EOSRankSageDataTypes.generated.h"

/**
 * Shinobi Rank Hierarchy
 */
UENUM(BlueprintType)
enum class EEOSShinobiRank : uint8
{
	Academy UMETA(DisplayName = "Academy Student"),
	Genin   UMETA(DisplayName = "Genin (Field Shinobi)"),
	Chunin  UMETA(DisplayName = "Chūnin (Tactical Leader)"),
	Jonin   UMETA(DisplayName = "Jōnin (Elite Specialist)"),
	Sage    UMETA(DisplayName = "Sage Pathway / Special")
};

/**
 * Sage Path Types
 */
UENUM(BlueprintType)
enum class EEOSSagePath : uint8
{
	None            UMETA(DisplayName = "None / Incompatible"),
	Toad            UMETA(DisplayName = "Mount Myōboku Toad Sage"),
	Snake           UMETA(DisplayName = "Ryūchi Cave Snake Sage"),
	Slug            UMETA(DisplayName = "Katsuyu / Shikkotsu Forest Path"),
	SageModeMitsuki UMETA(DisplayName = "Mitsuki Sage Transformation")
};

/**
 * Rank Progression Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSRankProgressionRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	FString RankProgressionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	EEOSShinobiRank CurrentRank = EEOSShinobiRank::Academy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	EEOSShinobiRank TargetRank = EEOSShinobiRank::Genin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	int32 RequiredLevel = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	FString RequiredTrialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rank")
	bool bIsRankPassed = false;
};

/**
 * Mentor Training Session Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMentorTrainingRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	FString SessionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	FString MentorID; // Kakashi, Jiraiya, Guy, Tsunade, Orochimaru

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	FString StudentCharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	FString TeachingFocus; // Precision, Chakra Control, Speed, Impact, Research

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	float BonusMasteryXPMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mentor")
	bool bSessionCompleted = false;
};

/**
 * Nature Energy Model
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNatureEnergyModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NatureEnergy")
	float TargetGaugeMin = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NatureEnergy")
	float TargetGaugeMax = 55.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NatureEnergy")
	float HoldDurationRequired = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NatureEnergy")
	float InstabilityDecayRate = 4.0f;
};

/**
 * Sage Pathway Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSSagePathwayRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	FString SagePathID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	EEOSSagePath PathType = EEOSSagePath::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	EEOSSageCompatibility Compatibility = EEOSSageCompatibility::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	FString MentorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	FEOSNatureEnergyModel NatureEnergyModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	FString UnlockedFormID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage")
	bool bSageUnlocked = false;
};

/**
 * Rank Trial Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSRankTrialRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	FString TrialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	EEOSShinobiRank RankType = EEOSShinobiRank::Academy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	FString CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	float TimeTargetSeconds = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	float MaxDamageTakenAllowed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankTrial")
	FString TrialGrade = "None"; // Bronze, Silver, Gold
};
