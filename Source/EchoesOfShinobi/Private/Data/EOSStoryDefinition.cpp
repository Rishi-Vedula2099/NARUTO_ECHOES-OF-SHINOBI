#include "../../Public/Data/EOSStoryDefinition.h"

UEOSStoryDefinition::UEOSStoryDefinition()
{
}

FPrimaryAssetId UEOSStoryDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EOSStoryDefinition"), FName(*StageRecord.StageID));
}
