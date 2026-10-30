#include "../../Public/Data/EOSVisualAssetGate.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "../../EchoesOfShinobi.h"

bool UEOSVisualAssetGate::ResolveRegistryFilePath(FString& OutFilePath) const
{
	// Authoritative registry location: Data/Visual/EOSVisualAssetRegistry.csv (Step 2).
	const FString CandidatePaths[] = {
		FPaths::Combine(FPaths::ProjectDir(), TEXT("Data/Visual/EOSVisualAssetRegistry.csv")),
		FPaths::Combine(FPaths::ProjectContentDir(), TEXT("../Data/Visual/EOSVisualAssetRegistry.csv")),
		FPaths::Combine(FPaths::ProjectDir(), TEXT("../../Data/Visual/EOSVisualAssetRegistry.csv"))
	};

	for (const FString& Candidate : CandidatePaths)
	{
		if (FPaths::FileExists(Candidate))
		{
			OutFilePath = Candidate;
			return true;
		}
	}
	return false;
}

bool UEOSVisualAssetGate::LoadRegistryFromDisk()
{
	RegistryRows.Empty();
	LastValidationPassCount = 0;
	LastValidationFailCount = 0;

	FString RegistryFilePath;
	if (!ResolveRegistryFilePath(RegistryFilePath))
	{
		UE_LOG(LogEOSData, Error, TEXT("VISUAL_GATE: EOSVisualAssetRegistry.csv could not be located under Data/Visual/. Registry validation cannot run."));
		return false;
	}

	FString CsvContents;
	if (!FFileHelper::LoadFileToString(CsvContents, *RegistryFilePath))
	{
		UE_LOG(LogEOSData, Error, TEXT("VISUAL_GATE: Failed to read registry file '%s'."), *RegistryFilePath);
		return false;
	}

	TArray<FString> Lines;
	CsvContents.ParseIntoArrayLines(Lines);

	// Header row: AssetID,CharacterID,ActorType,Category,TargetPath,SkeletalMeshAsset,SkeletonAsset,AnimBlueprint,
	// MaterialInstances,CollisionProfile,VFXProfile,IconPortraitAsset,VisualAssetSource,FinalAssetStatus,CanonStatus,AuthorReviewStatus
	constexpr int32 ExpectedColumnCount = 16;
	int32 FirstDataRow = 0;
	for (int32 LineIdx = 0; LineIdx < Lines.Num(); ++LineIdx)
	{
		if (Lines[LineIdx].StartsWith(TEXT("AssetID")))
		{
			FirstDataRow = LineIdx + 1;
			break;
		}
	}

	for (int32 LineIdx = FirstDataRow; LineIdx < Lines.Num(); ++LineIdx)
	{
		const FString& Line = Lines[LineIdx];
		if (Line.TrimStartAndEnd().IsEmpty())
		{
			continue;
		}

		TArray<FString> Columns;
		Line.ParseIntoArray(Columns, TEXT(","), /*InCStr*/ false);
		if (Columns.Num() < ExpectedColumnCount)
		{
			UE_LOG(LogEOSData, Warning, TEXT("VISUAL_GATE: Skipping malformed registry row %d (expected %d columns, found %d): '%s'"),
				LineIdx, ExpectedColumnCount, Columns.Num(), *Line);
			continue;
		}

		FEOSVisualAssetRegistryRow Row;
		Row.AssetID = Columns[0].TrimStartAndEnd();
		Row.CharacterID = Columns[1].TrimStartAndEnd();
		Row.ActorType = Columns[2].TrimStartAndEnd();
		Row.Category = Columns[3].TrimStartAndEnd();
		Row.TargetPath = Columns[4].TrimStartAndEnd();
		Row.SkeletalMeshAsset = Columns[5].TrimStartAndEnd();
		Row.SkeletonAsset = Columns[6].TrimStartAndEnd();
		Row.AnimBlueprint = Columns[7].TrimStartAndEnd();
		Row.MaterialInstances = Columns[8].TrimStartAndEnd();
		Row.CollisionProfile = Columns[9].TrimStartAndEnd();
		Row.VFXProfile = Columns[10].TrimStartAndEnd();
		Row.IconPortraitAsset = Columns[11].TrimStartAndEnd();
		Row.VisualAssetSource = Columns[12].TrimStartAndEnd();
		Row.FinalAssetStatus = Columns[13].TrimStartAndEnd();
		Row.CanonStatus = Columns[14].TrimStartAndEnd();
		Row.AuthorReviewStatus = Columns[15].TrimStartAndEnd();

		RegistryRows.Add(Row);
	}

	UE_LOG(LogEOSData, Log, TEXT("VISUAL_GATE: Loaded %d registry rows from '%s'."), RegistryRows.Num(), *RegistryFilePath);
	return RegistryRows.Num() > 0;
}

bool UEOSVisualAssetGate::FindRegistryRow(const FString& AssetID, FEOSVisualAssetRegistryRow& OutRow) const
{
	for (const FEOSVisualAssetRegistryRow& Row : RegistryRows)
	{
		if (Row.AssetID == AssetID)
		{
			OutRow = Row;
			return true;
		}
	}
	return false;
}

FEOSVisualAssetSlotVerdict UEOSVisualAssetGate::ValidateRegistryAsset(const FEOSVisualAssetRegistryRow& Row, bool bLogResult) const
{
	// Master Plan Section 5 — strict failure conditions. The gate MUST FAIL when the
	// referenced package is absent or resolves to a placeholder/debug asset.
	FEOSVisualAssetSlotVerdict Verdict;
	Verdict.AssetID = Row.AssetID;
	Verdict.AssetSlot = TEXT("RegistryTarget");
	Verdict.ResolvedPath = Row.TargetPath;

	// The .uasset or .umap path from the CSV maps to an object path with the same stem.
	FString CleanPath = Row.TargetPath;
	const bool bIsMap = CleanPath.EndsWith(TEXT(".umap"));
	CleanPath.RemoveFromEnd(TEXT(".uasset"));
	CleanPath.RemoveFromEnd(TEXT(".umap"));

	int32 LastSlashIdx = INDEX_NONE;
	FString ObjectPath = CleanPath;
	if (CleanPath.FindLastChar(TEXT('/'), LastSlashIdx) && LastSlashIdx != INDEX_NONE)
	{
		const FString AssetStem = CleanPath.Mid(LastSlashIdx + 1);
		ObjectPath = FString::Printf(TEXT("%s.%s"), *CleanPath, *AssetStem);
	}

	Verdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

	if (CleanPath.StartsWith(TEXT("/Game/")))
	{
		UObject* ResolvedObject = nullptr;
		if (bIsMap)
		{
			ResolvedObject = StaticLoadObject(UWorld::StaticClass(), nullptr, *ObjectPath);
			if (!ResolvedObject)
			{
				ResolvedObject = StaticLoadObject(UPackage::StaticClass(), nullptr, *CleanPath);
			}
		}
		else
		{
			ResolvedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *ObjectPath);
		}

		if (!ResolvedObject)
		{
			Verdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
		}
	}

	if (Verdict.FailureCode == FEOSVisualRectificationStatus::FailNone)
	{
		// Placeholder / forbidden taxonomy scan (applies to registry-declared paths).
		if (Row.SkeletalMeshAsset.Contains(TEXT("/Engine/BasicShapes/")) ||
		    Row.TargetPath.Contains(TEXT("/Engine/BasicShapes/")))
		{
			Verdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenBasicShape;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenDefaultMannequin(Row.SkeletalMeshAsset))
		{
			Verdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenMannequin;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(Row.MaterialInstances))
		{
			Verdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenPlaceholderMaterial;
		}
	}

	Verdict.bPassed = (Verdict.FailureCode == FEOSVisualRectificationStatus::FailNone);

	if (bLogResult)
	{
		if (Verdict.bPassed)
		{
			UE_LOG(LogEOSData, Log, TEXT("VISUAL_GATE_PASS [%s] %s"), *Verdict.AssetID, *Verdict.ResolvedPath);
		}
		else
		{
			UE_LOG(LogEOSData, Warning, TEXT("VISUAL_GATE_FAIL [%s] %s -> %s"), *Verdict.AssetID, *Verdict.FailureCode, *Verdict.ResolvedPath);
		}
	}

	return Verdict;
}

TArray<FEOSVisualAssetSlotVerdict> UEOSVisualAssetGate::ValidateAllRegistryAssets(bool bLogResults) const
{
	TArray<FEOSVisualAssetSlotVerdict> Verdicts;
	Verdicts.Reserve(RegistryRows.Num());

	int32 PassCount = 0;
	int32 FailCount = 0;

	for (const FEOSVisualAssetRegistryRow& Row : RegistryRows)
	{
		const FEOSVisualAssetSlotVerdict Verdict = ValidateRegistryAsset(Row, bLogResults);
		Verdict.bPassed ? ++PassCount : ++FailCount;
		Verdicts.Add(Verdict);
	}

	// Non-const cached summary (mutable counters live on the caller's copy; expose via const_cast-free counters).
	UEOSVisualAssetGate* MutableThis = const_cast<UEOSVisualAssetGate*>(this);
	MutableThis->LastValidationPassCount = PassCount;
	MutableThis->LastValidationFailCount = FailCount;

	return Verdicts;
}

FString UEOSVisualAssetGate::WriteAutomationReport(const TArray<FEOSVisualAssetSlotVerdict>& Verdicts) const
{
	int32 PassCount = 0;
	TArray<FString> Lines;

	Lines.Add(TEXT("# NARUTO: ECHOES OF SHINOBI — Visual Asset Automation Report"));
	Lines.Add(FString::Printf(TEXT("Generated: %s"), *FDateTime::UtcNow().ToString(TEXT("%Y-%m-%dT%H:%M:%SZ"))));
	Lines.Add(FString::Printf(TEXT("Registry rows validated: %d"), Verdicts.Num()));
	Lines.Add(TEXT(""));
	Lines.Add(TEXT("```"));
	Lines.Add(FString::Printf(TEXT("PHASE 1-13 SYSTEM IMPLEMENTATION:  %s"), FEOSVisualRectificationStatus::Phase1To13SystemImpl));
	Lines.Add(FString::Printf(TEXT("PHASE 13 VISUAL ASSET RECTIFICATION: %s"), FEOSVisualRectificationStatus::Phase13VisualRectification));
	Lines.Add(FString::Printf(TEXT("VISUAL ASSET AUTHORING:           %s"), FEOSVisualRectificationStatus::VisualAssetAuthoring));
	Lines.Add(FString::Printf(TEXT("VISUAL RUNTIME ACTIVATION:        %s"), FEOSVisualRectificationStatus::VisualRuntimeActivation));
	Lines.Add(FString::Printf(TEXT("FINAL VISUAL VERIFICATION:        %s"), FEOSVisualRectificationStatus::FinalVisualVerification));
	Lines.Add(TEXT("```"));
	Lines.Add(TEXT(""));
	Lines.Add(TEXT("| AssetID | ResolvedPath | FailureCode | Verdict |"));
	Lines.Add(TEXT("| :--- | :--- | :--- | :--- |"));

	for (const FEOSVisualAssetSlotVerdict& Verdict : Verdicts)
	{
		if (Verdict.bPassed)
		{
			++PassCount;
		}
		Lines.Add(FString::Printf(TEXT("| %s | %s | %s | %s |"),
			*Verdict.AssetID, *Verdict.ResolvedPath, *Verdict.FailureCode, Verdict.bPassed ? TEXT("PASS") : TEXT("FAIL")));
	}

	Lines.Add(TEXT(""));
	Lines.Add(FString::Printf(TEXT("**RESULT: %d PASS / %d FAIL** (strict Section 5 taxonomy — placeholders are hard failures)"),
		PassCount, Verdicts.Num() - PassCount));

	FString ReportPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Docs/VisualBible/Reports/VisualAssetAutomationReport.md"));
	if (!FFileHelper::SaveStringToFile(FString::Join(Lines, TEXT("\n")), *ReportPath))
	{
		UE_LOG(LogEOSData, Error, TEXT("VISUAL_GATE: Failed to write automation report to '%s'."), *ReportPath);
		return FString();
	}

	UE_LOG(LogEOSData, Log, TEXT("VISUAL_GATE: Automation report written to '%s'."), *ReportPath);
	return ReportPath;
}
