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
	case EEOSStageType::STORY:
		StageMult = 100;
		break;
	case EEOSStageType::ELITE:
		StageMult = 150;
		break;
	case EEOSStageType::CHALLENGE:
		StageMult = 180;
		break;
	case EEOSStageType::MINI_BOSS:
		StageMult = 220;
		break;
	case EEOSStageType::BOSS:
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
