#include "../../Public/Data/EOSTailedBeastDefinition.h"

UEOSTailedBeastDefinition::UEOSTailedBeastDefinition()
{
}

FPrimaryAssetId UEOSTailedBeastDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EOSTailedBeastDefinition"), FName(*Record.BeastID));
}
