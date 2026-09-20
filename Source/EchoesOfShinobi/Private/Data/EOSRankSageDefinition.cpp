#include "../../Public/Data/EOSRankSageDefinition.h"

UEOSRankSageDefinition::UEOSRankSageDefinition()
{
}

FPrimaryAssetId UEOSRankSageDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("EOSRankSageDefinition"), FName(*RankRecord.RankProgressionID));
}
