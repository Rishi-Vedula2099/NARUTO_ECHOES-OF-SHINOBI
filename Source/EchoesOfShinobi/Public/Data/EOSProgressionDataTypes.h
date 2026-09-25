#pragma once

#include "CoreMinimal.h"
#include "EOSDataTypes.h"
#include "EOSProgressionDataTypes.generated.h"

UENUM(BlueprintType)
enum class EEOSCharacterRarity : uint8
{
	COMMON_2STAR UMETA(DisplayName = "★★ Common"),
	UNCOMMON_3STAR UMETA(DisplayName = "★★★ Uncommon"),
	HEROIC_4STAR UMETA(DisplayName = "★★★★ Heroic"),
	LEGENDARY_5STAR UMETA(DisplayName = "★★★★★ Legendary"),
	MYTHIC_6STAR UMETA(DisplayName = "★★★★★★ Mythic"),
	TRANSCENDED_7STAR UMETA(DisplayName = "★★★★★★★ Transcended")
};

UENUM(BlueprintType)
enum class EEOSGearSlot : uint8
{
	HEADBAND UMETA(DisplayName = "Headband"),
	CHEST_ARMOR UMETA(DisplayName = "Armor / Chest"),
	GLOVES UMETA(DisplayName = "Gloves"),
	BOOTS UMETA(DisplayName = "Boots"),
	ACCESSORY_1 UMETA(DisplayName = "Accessory 1"),
	ACCESSORY_2 UMETA(DisplayName = "Accessory 2"),
	WEAPON_TOOL UMETA(DisplayName = "Weapon / Ninja Tool"),
	SPECIAL_RELIC UMETA(DisplayName = "Special Relic")
};

UENUM(BlueprintType)
enum class EEOSSageCompatibility : uint8
{
	NONE UMETA(DisplayName = "None"),
	NATURAL_AFFINITY UMETA(DisplayName = "Natural Affinity"),
	TRAINABLE UMETA(DisplayName = "Trainable"),
	RESTRICTED UMETA(DisplayName = "Restricted"),
	INCOMPATIBLE UMETA(DisplayName = "Incompatible"),
	SNAKE UMETA(DisplayName = "Snake Pathway"),
	TOAD UMETA(DisplayName = "Toad Pathway"),
	SLUG UMETA(DisplayName = "Slug Pathway")
};

UENUM(BlueprintType)
enum class EEOSTailedBeastResonance : uint8
{
	NONE UMETA(DisplayName = "None"),
	RESONANT UMETA(DisplayName = "Perfect Resonance"),
	STABLE UMETA(DisplayName = "Stable Vessel"),
	DANGEROUS UMETA(DisplayName = "Unstable / Volatile"),
	INCOMPATIBLE UMETA(DisplayName = "Incompatible")
};

USTRUCT(BlueprintType)
struct FEOSSkillNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
	FEOSStableId NodeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
	FText NodeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
	float StatBonusMultiplier = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
	TArray<FEOSStableId> PrerequisiteNodeIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
	int32 MaterialCost = 100;
};

USTRUCT(BlueprintType)
struct FEOSGearInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	FEOSStableId GearId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	EEOSGearSlot Slot = EEOSGearSlot::HEADBAND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	float PrimaryStatBonus = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
	int32 UpgradeLevel = 1;
};
