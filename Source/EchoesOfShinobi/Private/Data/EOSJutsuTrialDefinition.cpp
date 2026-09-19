#include "../../Public/Data/EOSJutsuTrialDefinition.h"

UEOSJutsuTrialDefinition::UEOSJutsuTrialDefinition()
{
}

FPrimaryAssetId UEOSJutsuTrialDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EOSJutsuTrialDefinition"), FName(*Record.JutsuID));
}
