#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSCombatDataTypes.h"
#include "EOSProgressionDataTypes.h"
#include "EOSJutsuTrialDataTypes.h"
#include "EOSCharacterDataTypes.generated.h"

/**
 * 2. Basic Attack Combo Data
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBasicAttackCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttacks")
	FString ComboID = "COMBO_LIGHT_DEFAULT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttacks")
	int32 TotalComboHits = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttacks")
	float BaseLightAttackDamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttacks")
	float HeavyFinisherDamageMultiplier = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttacks")
	bool bHasAirLauncher = true;
};

/**
 * 3. Skill Set Data
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSSkillSetContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	FString EvasionSkillID = "SKL_KAWARIMI"; // Body Replacement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	FString MobilitySkillID = "SKL_SHUNSHIN"; // Body Flicker / Dash

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TArray<FString> PassiveTraitIDs;
};

/**
 * 4. Active Jutsu Slots Container
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSJutsuSlotContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jutsu")
	FString SignatureJutsuID; // e.g. Rasengan / Chidori

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jutsu")
	FString OffensiveJutsuID; // e.g. Shadow Clone / Fireball

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jutsu")
	FString DefensiveJutsuID; // e.g. Mud Wall / Palm Bottom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jutsu")
	FString SupplementaryJutsuID; // e.g. Tracking Kunai / Field Heal
};

/**
 * 5. Ultimate Jutsu Definition
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSUltimateDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	FString UltimateID; // e.g. ULT_RASENSHURIKEN / ULT_KIRIN

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	FText UltimateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float GaugeCostRequired = 100.0f; // 100%

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float BaseUltimateMultiplier = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	bool bTriggersCinematicCutscene = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	FString TransformationFormTriggerID; // Form unlocked upon ultimate execution
};

/**
 * 6. Form / Transformation State Container
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSFormContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forms")
	FString BaseFormID = "FORM_BASE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forms")
	TArray<FString> UnlockedFormIDs; // e.g. FORM_SAGE_MODE, FORM_KURAMA_LINK, FORM_SUSANOO

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forms")
	FString ActiveFormID = "FORM_BASE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forms")
	float ActiveFormStatMultiplier = 1.0f;
};

/**
 * 7. 8-Slot Gear Grid Container
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGearGridContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString HeadGearID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString BodyGearID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString HandsGearID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString LegsGearID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString Accessory1ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString Accessory2ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FString ArtifactID;
};

/**
 * 8. Character Advancement Data
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterAdvancementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
	int32 CurrentEXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
	EEOSCharacterRarity StarTier = EEOSCharacterRarity::UNCOMMON_3STAR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
	int32 AscensionLevel = 0; // 0 to 5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
	FString ActiveAdvancementNodeID = "ADV_FOUNDATION";
};

/**
 * 9. Character Mastery Record
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterMasteryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mastery")
	TMap<FString, float> JutsuPracticeXPMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mastery")
	TMap<FString, EEOSMasteryBand> JutsuMasteryBandMap;
};

/**
 * 10. Compatibility Profile
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterCompatibilityProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
	TArray<EEOSChakraElement> PrimaryAffinities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
	EEOSSageCompatibility SagePathway = EEOSSageCompatibility::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
	EEOSTailedBeastResonance BeastResonance = EEOSTailedBeastResonance::NONE;
};

/**
 * Complete Unified 10-Layer Character Framework Block
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterFrameworkBlock
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|1.Stats")
	FEOSCombatStatBlock Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|2.BasicAttacks")
	FEOSBasicAttackCombo BasicAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|3.Skills")
	FEOSSkillSetContainer Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|4.Jutsu")
	FEOSJutsuSlotContainer Jutsu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|5.Ultimate")
	FEOSUltimateDefinition Ultimate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|6.Forms")
	FEOSFormContainer Forms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|7.Equipment")
	FEOSGearGridContainer Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|8.Advancement")
	FEOSCharacterAdvancementData Advancement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|9.Mastery")
	FEOSCharacterMasteryRecord Mastery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Framework|10.Compatibility")
	FEOSCharacterCompatibilityProfile Compatibility;
};
