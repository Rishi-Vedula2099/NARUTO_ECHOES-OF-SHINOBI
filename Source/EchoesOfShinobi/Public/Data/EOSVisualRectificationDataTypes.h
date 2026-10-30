#pragma once

#include "CoreMinimal.h"
#include "EOSVisualRectificationDataTypes.generated.h"

/**
 * One row of Data/Visual/EOSVisualAssetRegistry.csv (Master Plan Step 2).
 * Columns mirror the shipped CSV header exactly:
 * AssetID,CharacterID,ActorType,Category,TargetPath,SkeletalMeshAsset,SkeletonAsset,
 * AnimBlueprint,MaterialInstances,CollisionProfile,VFXProfile,IconPortraitAsset,
 * VisualAssetSource,FinalAssetStatus,CanonStatus,AuthorReviewStatus
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSVisualAssetRegistryRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString AssetID;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString CharacterID;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString ActorType;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString Category;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString TargetPath;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString SkeletalMeshAsset;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString SkeletonAsset;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString AnimBlueprint;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString MaterialInstances;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString CollisionProfile;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString VFXProfile;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString IconPortraitAsset;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString VisualAssetSource;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString FinalAssetStatus;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString CanonStatus;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString AuthorReviewStatus;
};

/**
 * Deterministic per-asset visual integrity verdict (Master Plan Section 5).
 * One instance per required asset slot; a pass requires a real authored asset.
 */
USTRUCT(BlueprintType)
struct ECHOESOFSHINOBI_API FEOSVisualAssetSlotVerdict
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString AssetID;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString AssetSlot;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString ResolvedPath;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	FString FailureCode;

	UPROPERTY(BlueprintReadOnly, Category = "EOS|Visuals")
	bool bPassed = false;
};
