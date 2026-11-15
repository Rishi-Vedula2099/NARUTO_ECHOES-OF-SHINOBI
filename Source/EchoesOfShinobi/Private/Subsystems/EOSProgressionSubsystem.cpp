#include "../../Public/Subsystems/EOSProgressionSubsystem.h"
#include "../../Public/Data/EOSStageDataTypes.h"
#include "../../EchoesOfShinobi.h"

void UEOSProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSCore, Log, TEXT("EOSProgressionSubsystem initialized successfully."));
}

float UEOSProgressionSubsystem::CalculateCharacterPower(float BaseStats, int32 Level, int32 StarTier, int32 AscensionTier, float SkillBonus, float GearBonus, int32 BondLevel) const
{
	float LevelContrib = Level * 15.0f;
	float StarContrib = StarTier * 100.0f;
	float AscensionContrib = AscensionTier * 250.0f;
	float BondContrib = BondLevel * 20.0f;

	float TotalPower = BaseStats + LevelContrib + StarContrib + AscensionContrib + SkillBonus + GearBonus + BondContrib;
	return TotalPower;
}

int32 UEOSProgressionSubsystem::CalculateRecommendedStagePower(int32 ArcIndex, int32 ChapterIndex, EEOSStageType StageType) const
{
	int32 ArcBase = ArcIndex * 500;
	int32 ChapterMult = ChapterIndex * 120;
	int32 StageMult = 100;

	switch (StageType)
	{
	case EEOSStageType::Story:
		StageMult = 100;
		break;
	case EEOSStageType::Elite:
		StageMult = 150;
		break;
	case EEOSStageType::Challenge:
		StageMult = 180;
		break;
	case EEOSStageType::MiniBoss:
		StageMult = 220;
		break;
	case EEOSStageType::Boss:
		StageMult = 300;
		break;
	}

	return ArcBase + ChapterMult + StageMult;
}

EEOSSageCompatibility UEOSProgressionSubsystem::EvaluateSageCompatibility(FEOSStableId CharacterId, FEOSStableId SageArtId) const
{
	if (CharacterId.StableId == FName("CHR_NARUTO") || CharacterId.StableId == FName("CHR_JIRAIYA"))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY;
	}

	return EEOSSageCompatibility::TRAINABLE;
}

int32 UEOSProgressionSubsystem::GetRequiredCumulativeEXPForLevel(int32 Level) const
{
	if (Level <= 1) return 0;
	if (Level == 2) return 200;
	if (Level == 3) return 500;
	if (Level == 4) return 850;
	if (Level == 5) return 1200;

	// Scaling for levels beyond 5: +400 EXP per additional level
	return 1200 + (Level - 5) * 400;
}

int32 UEOSProgressionSubsystem::CalculateLevelFromCumulativeEXP(int32 CumulativeEXP) const
{
	if (CumulativeEXP < 200) return 1;
	if (CumulativeEXP < 500) return 2;
	if (CumulativeEXP < 850) return 3;
	if (CumulativeEXP < 1200) return 4;

	// Level 5 and above
	return 5 + (CumulativeEXP - 1200) / 400;
}

