#include "../../Public/Data/EOSAIDefinition.h"

UEOSAIDefinition::UEOSAIDefinition()
{
}

FPrimaryAssetId UEOSAIDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EOSAIDefinition"), FName(*AIProfileID));
}
