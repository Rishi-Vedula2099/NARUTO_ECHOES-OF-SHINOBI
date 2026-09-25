#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSDataTypes.generated.h"

UENUM(BlueprintType)
enum class EEOSCharacterEra : uint8
{
	ERA_UNKNOWN UMETA(DisplayName = "Unknown Era"),
	ERA_ACADEMY UMETA(DisplayName = "Academy Era"),
	ERA_GENIN UMETA(DisplayName = "Genin Era"),
	ERA_CHUNIN UMETA(DisplayName = "Chunin Era"),
	ERA_JONIN UMETA(DisplayName = "Jonin Era"),
	ERA_SHIPPUDEN UMETA(DisplayName = "Shippuden Era"),
	ERA_SAGE UMETA(DisplayName = "Sage Mode Era"),
	ERA_FIVE_KAGE_SUMMIT UMETA(DisplayName = "Five Kage Summit Era"),
	ERA_WAR UMETA(DisplayName = "Fourth Shinobi War Era"),
	ERA_FOURTH_WAR UMETA(DisplayName = "Fourth Shinobi War Era (Alt)"),
	ERA_HOKAGE UMETA(DisplayName = "Hokage Era"),
	ERA_BARYON UMETA(DisplayName = "Baryon Mode Era"),
	ERA_BORUTO UMETA(DisplayName = "Boruto Era"),
	ERA_BORUTO_ACADEMY UMETA(DisplayName = "Boruto Academy Era"),
	ERA_BORUTO_KARMA UMETA(DisplayName = "Boruto Karma Era"),
	ERA_BORUTO_TBV UMETA(DisplayName = "Boruto: Two Blue Vortex Era")
};

UENUM(BlueprintType)
enum class EEOSElement : uint8
{
	NONE UMETA(DisplayName = "None / Non-Elemental"),
	FIRE UMETA(DisplayName = "Fire (Katon)"),
	WIND UMETA(DisplayName = "Wind (Fūton)"),
	LIGHTNING UMETA(DisplayName = "Lightning (Raiton)"),
	EARTH UMETA(DisplayName = "Earth (Doton)"),
	WATER UMETA(DisplayName = "Water (Suiton)"),
	ICE UMETA(DisplayName = "Ice (Hyōton - Kekkei Genkai)"),
	WOOD UMETA(DisplayName = "Wood (Mokuton - Kekkei Genkai)"),
	LAVA UMETA(DisplayName = "Lava (Yōton - Kekkei Genkai)"),
	BOIL UMETA(DisplayName = "Boil (Futsuton - Kekkei Genkai)"),
	STORM UMETA(DisplayName = "Storm (Rantond - Kekkei Genkai)"),
	PARTICLE UMETA(DisplayName = "Particle (Jinton - Kekkei Tōta)")
};

UENUM(BlueprintType)
enum class EEOSJutsuType : uint8
{
	BASIC_ATTACK UMETA(DisplayName = "Basic Attack Combo"),
	JUTSU_PRIMARY UMETA(DisplayName = "Primary Jutsu"),
	JUTSU_SECONDARY UMETA(DisplayName = "Secondary Jutsu"),
	JUTSU_TACTICAL UMETA(DisplayName = "Tactical Jutsu"),
	ULTIMATE UMETA(DisplayName = "Ultimate Secret Technique"),
	TRANSFORMATION UMETA(DisplayName = "Awakening / Transformation"),
	SUPPORT_SKILL UMETA(DisplayName = "Support Assist Skill")
};

UENUM(BlueprintType)
enum class EEOSProvenanceType : uint8
{
	CANON_VERIFIED UMETA(DisplayName = "Canon Verified"),
	CANON_SOURCE_REFERENCE UMETA(DisplayName = "Canon Source Reference"),
	CANON_INSPIRED UMETA(DisplayName = "Canon Inspired Gameplay"),
	ORIGINAL_GAME_VARIANT UMETA(DisplayName = "Original Game Variant"),
	WHAT_IF UMETA(DisplayName = "What-If / Alternate Timeline"),
	TODO_AUTHORING UMETA(DisplayName = "TODO Authoring Pending")
};

UENUM(BlueprintType)
enum class EEOSValidationSeverity : uint8
{
	INFO UMETA(DisplayName = "Informational"),
	WARNING UMETA(DisplayName = "Warning"),
	ERROR_BLOCKING UMETA(DisplayName = "Blocking Error"),
	TODO_AUTHORING UMETA(DisplayName = "TODO Authoring Required"),
	AUTHOR_REVIEW UMETA(DisplayName = "Author Review Required")
};

USTRUCT(BlueprintType)
struct FEOSStableId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName StableId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EEOSProvenanceType Provenance = EEOSProvenanceType::TODO_AUTHORING;

	FEOSStableId() : StableId(NAME_None) {}
	FEOSStableId(FName InId, EEOSProvenanceType InProvenance = EEOSProvenanceType::CANON_VERIFIED)
		: StableId(InId), Provenance(InProvenance) {}

	bool IsValid() const { return !StableId.IsNone(); }
};
