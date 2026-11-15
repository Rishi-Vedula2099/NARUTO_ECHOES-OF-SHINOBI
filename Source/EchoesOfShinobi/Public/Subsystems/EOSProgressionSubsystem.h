#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EOSDataTypes.h"
#include "Data/EOSProgressionDataTypes.h"
#include "EOSProgressionSubsystem.generated.h"

UCLASS()
class ECHOESOFSHINOBI_API UEOSProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintPure, Category = "EOS Progression")
	float CalculateCharacterPower(float BaseStats, int32 Level, int32 StarTier, int32 AscensionTier, float SkillBonus, float GearBonus, int32 BondLevel) const;

	UFUNCTION(BlueprintPure, Category = "EOS Progression")
	int32 CalculateRecommendedStagePower(int32 ArcIndex, int32 ChapterIndex, EEOSStageType StageType) const;

	UFUNCTION(BlueprintPure, Category = "EOS Progression")
	EEOSSageCompatibility EvaluateSageCompatibility(FEOSStableId CharacterId, FEOSStableId SageArtId) const;

	/** Cumulative EXP required to reach a specific level (L1:0, L2:200, L3:500, L4:850, L5:1200, ...) */
	UFUNCTION(BlueprintPure, Category = "EOS Progression")
	int32 GetRequiredCumulativeEXPForLevel(int32 Level) const;

	/** Calculate player level dynamically from cumulative EXP based on the progression threshold curve */
	UFUNCTION(BlueprintPure, Category = "EOS Progression")
	int32 CalculateLevelFromCumulativeEXP(int32 CumulativeEXP) const;
};
