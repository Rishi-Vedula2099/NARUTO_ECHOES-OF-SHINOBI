#pragma once

/**
 * VISUAL-RECTIFICATION-MASTER-1.1 — Authoritative Visual Rectification Status Constants.
 * Mirrors the locked status declarations of the Master Visual Asset Production Plan
 * so every report, log, and automation output uses identical, unambiguous vocabulary.
 *
 * Intentionally PURE C++ (no UHT / no .generated.h): this header is shared by both
 * the Unreal target and the standalone EOS_STANDALONE_RUNNER launcher build.
 */
struct ECHOESOFSHINOBI_API FEOSVisualRectificationStatus
{
	static constexpr TCHAR Phase1To13SystemImpl[] = TEXT("PASS / EXISTING");
	static constexpr TCHAR Phase13VisualRectification[] = TEXT("IN PROGRESS");
	static constexpr TCHAR VisualAssetAuthoring[] = TEXT("IN PROGRESS");
	static constexpr TCHAR VisualRuntimeActivation[] = TEXT("BLOCKED UNTIL REQUIRED ASSETS EXIST");
	static constexpr TCHAR FinalVisualVerification[] = TEXT("PENDING");

	/** Failure codes emitted by the strict visual integrity gate (Plan Section 5 taxonomy). */
	static constexpr TCHAR FailNone[] = TEXT("NONE");
	static constexpr TCHAR FailMissingSoftReference[] = TEXT("MISSING_SOFT_REFERENCE");
	static constexpr TCHAR FailMissingPackage[] = TEXT("MISSING_PACKAGE");
	static constexpr TCHAR FailMissingSkeleton[] = TEXT("MISSING_SKELETON");
	static constexpr TCHAR FailForbiddenBasicShape[] = TEXT("FORBIDDEN_BASIC_SHAPE");
	static constexpr TCHAR FailForbiddenMannequin[] = TEXT("FORBIDDEN_MANNEQUIN");
	static constexpr TCHAR FailForbiddenPlaceholderMaterial[] = TEXT("FORBIDDEN_PLACEHOLDER_MATERIAL");

	/** Strict classifier: shared Engine/BasicShapes paths are hard visual failures. */
	static bool IsForbiddenEngineBasicShape(const FString& AssetPath)
	{
		return AssetPath.Contains(TEXT("/Engine/BasicShapes/"));
	}

	/** Strict classifier: default third-person template mannequins are hard visual failures. */
	static bool IsForbiddenDefaultMannequin(const FString& AssetPath)
	{
		return AssetPath.Contains(TEXT("SKM_Quinn")) || AssetPath.Contains(TEXT("SKM_Manny")) ||
		       AssetPath.Contains(TEXT("/Engine/Content/EngineMeshes/"));
	}

	/** Strict classifier: checkerboard / debug / placeholder materials are hard visual failures. */
	static bool IsForbiddenPlaceholderMaterial(const FString& AssetPath)
	{
		return AssetPath.Contains(TEXT("Checker")) || AssetPath.Contains(TEXT("PLACEHOLDER")) ||
		       AssetPath.Contains(TEXT("TECHNICAL_DEBUG")) || AssetPath.Contains(TEXT("WorldGridMaterial"));
	}
};
