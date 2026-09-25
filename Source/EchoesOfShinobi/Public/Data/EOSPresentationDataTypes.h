#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "EOSCombatDataTypes.h"
#include "EOSPresentationDataTypes.generated.h"

/**
 * Camera Modes supported by the Shinobi dynamic camera state machine.
 */
UENUM(BlueprintType)
enum class EEOSCameraMode : uint8
{
	Exploration   UMETA(DisplayName = "Exploration Camera"),
	Combat        UMETA(DisplayName = "Combat Camera"),
	LockOn        UMETA(DisplayName = "Lock-On Camera"),
	Boss          UMETA(DisplayName = "Boss Arena Camera"),
	Jutsu         UMETA(DisplayName = "Jutsu Dynamic Camera"),
	Ultimate      UMETA(DisplayName = "Ultimate Cinematic Camera"),
	Dialogue      UMETA(DisplayName = "Dialogue Camera"),
	MotionManga   UMETA(DisplayName = "Motion Manga Parallax Camera")
};

/**
 * Camera Profile configuration.
 * Decouples camera distances, FOV, and offsets from hardcoded C++ values.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCameraProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	EEOSCameraMode ActiveMode = EEOSCameraMode::Exploration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetArmLength = 380.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector SocketOffset = FVector(0.0f, 35.0f, 65.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float FieldOfView = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraLagSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bEnableCameraLag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetHeightScreenFraction = 0.20f; // ~18-22% screen height composition

	void SetMode(EEOSCameraMode InMode)
	{
		ActiveMode = InMode;
		switch (InMode)
		{
		case EEOSCameraMode::Exploration:
			TargetArmLength = 380.0f;
			SocketOffset = FVector(0.0f, 35.0f, 65.0f);
			FieldOfView = 80.0f;
			CameraLagSpeed = 12.0f;
			break;
		case EEOSCameraMode::Combat:
			TargetArmLength = 320.0f;
			SocketOffset = FVector(0.0f, 45.0f, 55.0f);
			FieldOfView = 85.0f;
			CameraLagSpeed = 16.0f;
			break;
		case EEOSCameraMode::LockOn:
			TargetArmLength = 340.0f;
			SocketOffset = FVector(0.0f, 50.0f, 60.0f);
			FieldOfView = 82.0f;
			CameraLagSpeed = 18.0f;
			break;
		case EEOSCameraMode::Boss:
			TargetArmLength = 520.0f;
			SocketOffset = FVector(0.0f, 20.0f, 90.0f);
			FieldOfView = 90.0f;
			CameraLagSpeed = 10.0f;
			break;
		case EEOSCameraMode::Jutsu:
			TargetArmLength = 260.0f;
			SocketOffset = FVector(20.0f, 40.0f, 45.0f);
			FieldOfView = 88.0f;
			CameraLagSpeed = 22.0f;
			break;
		case EEOSCameraMode::Ultimate:
			TargetArmLength = 200.0f;
			SocketOffset = FVector(10.0f, 25.0f, 35.0f);
			FieldOfView = 75.0f;
			CameraLagSpeed = 25.0f;
			break;
		case EEOSCameraMode::Dialogue:
			TargetArmLength = 180.0f;
			SocketOffset = FVector(0.0f, 30.0f, 50.0f);
			FieldOfView = 70.0f;
			CameraLagSpeed = 8.0f;
			break;
		case EEOSCameraMode::MotionManga:
			TargetArmLength = 400.0f;
			SocketOffset = FVector(0.0f, 0.0f, 50.0f);
			FieldOfView = 75.0f;
			CameraLagSpeed = 6.0f;
			break;
		default:
			break;
		}
	}
};

/**
 * Movement Profile configuration.
 * Decouples walk/run/sprint/jump/rotation speeds from hardcoded C++ values.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMovementProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 650.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 1100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpZVelocity = 750.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AirControl = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DodgeImpulse = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SubstitutionDistance = 600.0f;
};

/**
 * Performance Profile configuration.
 * Measurable profiling acceptance presets targeting 60 FPS on GTX 1650.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSPerformanceProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	FString ProfileName = "PHASE13_GTX1650_MINIMUM";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float TargetResolutionScale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float ShadowDistance = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 MaxNiagaraParticles = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float FoliageDensityScale = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float MaxDrawDistance = 15000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float ProcessWorkingSetBudgetMB = 6000.0f; // Process RAM / Working Set < 6.0 GB (Packaged)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float DedicatedVRAMBudgetMB = 3200.0f; // Dedicated GPU memory < 3.2 GB (Packaged)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float TargetFPS = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float Low1PercentFPS = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float Low01PercentFPS = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float MaxGPUTimeMS = 16.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float MaxGameThreadTimeMS = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float MaxRenderThreadTimeMS = 14.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 MaxDrawCalls = 1800;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 MaxVisibleTriangles = 2500000;
};

/**
 * Character Visual Profile configuration.
 * Decoupled data-driven character presentation.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSCharacterVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString CharacterID = "CHAR_NARUTO";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString EraID = "ERA_GENIN";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString FormID = "FORM_BASE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString CostumeID = "COSTUME_CLASSIC_TRACKSUIT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString WeaponSetID = "WEAPON_KUNAI_SET";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString AnimationProfileID = "ANIM_NARUTO_GENIN";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString MaterialProfileID = "MAT_CEL_TOON_SHINOBI";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FString VFXProfileID = "VFX_NARUTO_DEFAULT";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FText CharacterDisplayName = FText::FromString("Naruto Uzumaki");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FText CharacterTitle = FText::FromString("Genin of the Hidden Leaf");
};

/**
 * Reusable Anime VFX Profile.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSVFXProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString VFXProfileID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	EEOSChakraElement Element = EEOSChakraElement::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FLinearColor PrimaryColor = FLinearColor(0.1f, 0.6f, 1.0f, 1.0f); // Azure chakra

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FLinearColor AccentColor = FLinearColor(0.8f, 0.95f, 1.0f, 1.0f); // Core white

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	float Intensity = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString ChargeVFXName = "NS_ChakraCharge";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString CastVFXName = "NS_JutsuCast";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString ProjectileVFXName = "NS_JutsuProjectile";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString ContactVFXName = "NS_HitImpact";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString ImpactVFXName = "NS_JutsuBurst";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	float CameraShakeMagnitude = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FName AudioEventTag = FName("Audio.Jutsu.Rasengan");
};

UENUM(BlueprintType)
enum class EEOSNPCType : uint8
{
	Villager         UMETA(DisplayName = "Konoha Villager"),
	ChuninGuard      UMETA(DisplayName = "Chunin Gate Guard"),
	AcademyStudent   UMETA(DisplayName = "Academy Student"),
	Shopkeeper       UMETA(DisplayName = "Shopkeeper"),
	NamedShinobi     UMETA(DisplayName = "Named Shinobi")
};

inline FString ResolveNPCInteraction(EEOSNPCType InType)
{
	switch (InType)
	{
	case EEOSNPCType::ChuninGuard:
		return TEXT("MISSION_TRAINING_GROUND_INVESTIGATION");
	case EEOSNPCType::Shopkeeper:
		return TEXT("SHOP_GEAR_INSPECTION");
	case EEOSNPCType::NamedShinobi:
		return TEXT("BOND_DIALOGUE_INTERACTION");
	case EEOSNPCType::Villager:
	case EEOSNPCType::AcademyStudent:
	default:
		return TEXT("DIALOGUE_AMBIENT");
	}
}

/**
 * Distance simulation LOD profile for living village NPCs.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSNPCSimulationProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float NearDistance = 1500.0f; // <15m: Full animation, head tracking, prompts

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float MediumDistance = 4000.0f; // 15-40m: Standard locomotion, simplified updates

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float FarDistance = 10000.0f; // 40-100m: Low-frequency tick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float VeryFarDistance = 15000.0f; // >100m: Culled / disabled simulation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float NearTickInterval = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float MediumTickInterval = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	float FarTickInterval = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Simulation")
	bool bEnableHeadTrackingInNearRange = true;
};

/**
 * Asset Provenance & Originality Verification Gate.
 * Enforces zero ripped/extracted proprietary Storm assets in ECHOES OF SHINOBI.
 */
UENUM(BlueprintType)
enum class EEOSAssetSourceType : uint8
{
	InternallyAuthored    UMETA(DisplayName = "Internally Authored"),
	ProperlyLicensed      UMETA(DisplayName = "Properly Licensed Asset"),
	VisualReferenceOnly   UMETA(DisplayName = "Visual Reference Direction (Not Shipped)"),
	ForbiddenProprietary  UMETA(DisplayName = "Forbidden Proprietary Ripped Asset")
};

UENUM(BlueprintType)
enum class EEOSAssetOriginalityStatus : uint8
{
	AuthoritativeOriginal UMETA(DisplayName = "Authoritative Original Asset"),
	LicensedCompliant     UMETA(DisplayName = "Licensed Compliant Asset"),
	StrictlyProhibited    UMETA(DisplayName = "Prohibited Proprietary Asset")
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSAssetProvenanceRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString AssetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString AssetType; // Mesh, Material, Texture, Animation, VFX, Audio, UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	EEOSAssetSourceType SourceType = EEOSAssetSourceType::InternallyAuthored;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString LicenseStatus = "ORIGINAL_PROJECT_AUTHORSHIP";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString CanonStatus = "CANON_VERIFIED"; // CANON_VERIFIED or ORIGINAL_GAME_VARIANT

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	EEOSAssetOriginalityStatus OriginalityStatus = EEOSAssetOriginalityStatus::AuthoritativeOriginal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString ReferenceSource = "STORM_VISUAL_LANGUAGE_HIGH_LEVEL_DIRECTION";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provenance")
	FString AuthoringStatus = "AUTHORED_FOR_ECHOES_OF_SHINOBI";

	bool IsCompliant() const
	{
		return (SourceType != EEOSAssetSourceType::ForbiddenProprietary) &&
		       (OriginalityStatus != EEOSAssetOriginalityStatus::StrictlyProhibited);
	}
};

/**
 * Phase 1, 2, 4, 5: Character Visual Form & Transformation Profiles
 */
UENUM(BlueprintType)
enum class EEOSVisualForm : uint8
{
	Base_Academy           UMETA(DisplayName = "Academy Shinobi"),
	Base_Genin             UMETA(DisplayName = "Genin Shinobi"),
	Base_Shippuden         UMETA(DisplayName = "Shippuden Shinobi"),
	SageMode               UMETA(DisplayName = "Mount Myoboku Sage Mode"),
	VermilionAwakening     UMETA(DisplayName = "One-Tail Vermilion Shroud"),
	KuramaChakraMode       UMETA(DisplayName = "Kurama Chakra Mode (KCM)"),
	BaryonMode             UMETA(DisplayName = "Baryon Mode Pinnacle"),
	CurseMarkStage2        UMETA(DisplayName = "Curse Mark Stage 2"),
	SusanooRibcage         UMETA(DisplayName = "Susano'o Ribcage Armor")
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSFormVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	EEOSVisualForm Form = EEOSVisualForm::Base_Genin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	FLinearColor EyePigmentationColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f); // Orange for Sage

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	FLinearColor PupilColor = FLinearColor(0.1f, 0.4f, 0.9f, 1.0f); // Blue default, Gold Sage, Crimson Kurama

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	FLinearColor ChakraAuraColor = FLinearColor(0.1f, 0.6f, 1.0f, 0.0f); // Vermilion red, Sage orange, KCM gold

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	float AuraIntensity = 0.0f; // 0 for base, 2.5 for Vermilion, 5.0 for KCM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	FString CloakMeshName = "None"; // e.g. "SM_SageCloak_RedBlack", "SM_KuramaFlameShroud"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	bool bEnableFaceMarkings = false; // Whiskers deepen, Sage eyelid orange banding

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Form Visual")
	float StanceProwessScale = 1.0f; // Feral crouch scaling for Vermilion
};

/**
 * Phase 7 & 8: 8 Great Shinobi Biome & World Stage Visual Profiles
 */
UENUM(BlueprintType)
enum class EEOSShinobiBiome : uint8
{
	LandOfFire_Konoha       UMETA(DisplayName = "Land of Fire - Konoha Village & Training Grounds"),
	LandOfWaves_Bridge      UMETA(DisplayName = "Land of Waves - Great Naruto Bridge & Misty Coasts"),
	LandOfWind_Sunagakure   UMETA(DisplayName = "Land of Wind - Suna Ochre Dunes & Adobe Spires"),
	LandOfLightning_Cloud   UMETA(DisplayName = "Land of Lightning - Kumogakure Mountain Crags"),
	LandOfWater_Mist        UMETA(DisplayName = "Land of Water - Kirigakure Bamboo & Dense Fog"),
	LandOfEarth_Stone       UMETA(DisplayName = "Land of Earth - Iwagakure Jagged Canyons"),
	ValleyOfTheEnd          UMETA(DisplayName = "Valley of the End - Monumental Statues & Cascade"),
	KamuiDimension          UMETA(DisplayName = "Kamui Dimension - Floating Obsidian Monoliths")
};

USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBiomeVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	EEOSShinobiBiome Biome = EEOSShinobiBiome::LandOfFire_Konoha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	FLinearColor SunColor = FLinearColor(1.0f, 0.95f, 0.88f); // 5400K daylight

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	float SunIntensity = 6.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	FLinearColor SkyFillColor = FLinearColor(0.85f, 0.92f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	FLinearColor FogColor = FLinearColor(0.75f, 0.85f, 0.95f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	float FogDensity = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	FString SignatureArchitecture = "Modular Japanese Timber & Terracotta Tile";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
	FString SignatureFlora = "Sakura Cherry Blossom & Konoha Giant Cedar";
};

/**
 * Phase 6: Adaptive AI & Boss Visual Presentation
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSBossVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	FString BossID = "BOSS_ZABUZA_MOMOCHI";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	FString ArenaVisualTheme = "MISTY_GREAT_NARUTO_BRIDGE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	FLinearColor EyeGlowColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f); // Crimson for Sharingan, Violet for Rinnegan

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	float AtmosphereMistDensity = 0.04f; // Thick Zabuza mist

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	FString SignatureWeaponSocket = "Socket_ExecutionerBlade_Back";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Visual")
	FString BossAuraParticle = "NS_SilentKillingMist";
};

/**
 * Phase 10: Gacha Summoning Altar Visual Presentation
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGachaVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FString SummoningCircleTexture = "T_ToadSummoningSeal_Circle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FLinearColor SealGlowColor_Normal = FLinearColor(0.2f, 0.6f, 1.0f, 1.0f); // Blue R

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FLinearColor SealGlowColor_SR = FLinearColor(0.85f, 0.2f, 0.9f, 1.0f); // Purple SR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FLinearColor SealGlowColor_SSR = FLinearColor(1.0f, 0.85f, 0.1f, 1.0f); // Gold SSR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FLinearColor SealGlowColor_UR = FLinearColor(1.0f, 0.2f, 0.2f, 1.0f); // Crimson / Rainbow UR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha Visual")
	FString SummonSmokePuffVFX = "NS_ToadSummon_SmokeCloud";
};

/**
 * Phase 11: Gear Visual Attachment Sockets & Bond Resonance
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSGearSocketProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FName WeaponBackSocket = FName("Socket_Weapon_Back");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FName WeaponHipSocket = FName("Socket_Weapon_Hip");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FName FlakJacketSocket = FName("Socket_Armor_Chest");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FName ToolPouchSocket = FName("Socket_Pouch_RightThigh");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FName ForeheadProtectorSocket = FName("Socket_Headband_Forehead");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear Sockets")
	FLinearColor BondResonanceThreadColor = FLinearColor(0.95f, 0.65f, 0.1f, 0.8f); // Golden chakra link
};

/**
 * Phase 12: 2.5D Motion Manga Onomatopoeia & Cel Styling
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSMotionMangaVisualProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Manga Visual")
	FString InkBorderBrush = "T_Manga_InkBorder_Rough";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Manga Visual")
	float HalftoneDotPitch = 0.008f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Manga Visual")
	FString SpeedLinesVFX = "NS_Manga_ImpactSpeedLines";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Manga Visual")
	TArray<FString> OnomatopoeiaGlyphs = {
		TEXT("DODODODO"), // Ominous approach
		TEXT("BASHIN"),   // Heavy hit
		TEXT("ZUBA"),     // Slash impact
		TEXT("GYURURURU"),// Chakra swirling
		TEXT("DON")       // Dramatic revelation
	};
};
