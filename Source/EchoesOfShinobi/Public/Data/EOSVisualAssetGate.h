#pragma once

#include "CoreMinimal.h"
#include "EOSVisualRectificationDataTypes.h"
#include "EOSSharedVisualRectificationTypes.h"
#include "EOSVisualAssetGate.generated.h"

/**
 * Master Visual Asset Production & Runtime Activation (VISUAL-RECTIFICATION-MASTER-1.1).
 *
 * Single authoritative implementation of:
 *  - Step 2: EOSVisualAssetRegistry.csv ingestion (Data/Visual/EOSVisualAssetRegistry.csv)
 *  - Section 5: strict visual validation failure conditions (never pass on placeholders)
 *  - Step 23: FEOSVisualAssetValidationTest support + VisualAssetAutomationReport generation
 *
 * The gate loads the shipped registry CSV from disk at runtime (both editor and
 * packaged builds fall back to FPaths::ProjectContentDir()/ProjectDir lookups),
 * then validates every authored asset against the strict failure taxonomy:
 * nullptr / missing package / Engine/BasicShapes / default mannequin /
 * placeholder materials are HARD FAILURES — never passes.
 */
UCLASS(BlueprintType)
class ECHOESOFSHINOBI_API UEOSVisualAssetGate : public UObject
{
	GENERATED_BODY()

public:
	/** Load & parse Data/Visual/EOSVisualAssetRegistry.csv. Returns false if the registry file cannot be located. */
	bool LoadRegistryFromDisk();

	/** Number of rows successfully parsed from the registry. */
	UFUNCTION(BlueprintPure, Category = "EOS|Visuals")
	int32 GetRegistryRowCount() const { return RegistryRows.Num(); }

	/** Find a registry row by AssetID. */
	bool FindRegistryRow(const FString& AssetID, FEOSVisualAssetRegistryRow& OutRow) const;

	/** All parsed registry rows (order preserved). */
	const TArray<FEOSVisualAssetRegistryRow>& GetRegistryRows() const { return RegistryRows; }

	/**
	 * Strict integrity validation of a single registered asset (Section 5 taxonomy).
	 * Never passes for nullptr / missing package / forbidden placeholder assets.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Visuals")
	FEOSVisualAssetSlotVerdict ValidateRegistryAsset(const FEOSVisualAssetRegistryRow& Row, bool bLogResult = false) const;

	/** Validate the entire registry; returns per-asset verdicts in registry order. */
	UFUNCTION(BlueprintCallable, Category = "EOS|Visuals")
	TArray<FEOSVisualAssetSlotVerdict> ValidateAllRegistryAssets(bool bLogResults = false) const;

	/** Summary counts of the last validation run. */
	UFUNCTION(BlueprintPure, Category = "EOS|Visuals")
	int32 GetLastValidationPassCount() const { return LastValidationPassCount; }

	UFUNCTION(BlueprintPure, Category = "EOS|Visuals")
	int32 GetLastValidationFailCount() const { return LastValidationFailCount; }

	/**
	 * Step 23: write Docs/VisualBible/Reports/VisualAssetAutomationReport.md
	 * with the per-asset verdict table and locked status banner. Returns the written file path (empty on failure).
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Visuals")
	FString WriteAutomationReport(const TArray<FEOSVisualAssetSlotVerdict>& Verdicts) const;

private:
	bool ResolveRegistryFilePath(FString& OutFilePath) const;

	TArray<FEOSVisualAssetRegistryRow> RegistryRows;
	int32 LastValidationPassCount = 0;
	int32 LastValidationFailCount = 0;
};
