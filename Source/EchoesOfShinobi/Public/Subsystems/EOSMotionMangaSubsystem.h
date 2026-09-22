#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSNarrativeDataTypes.h"
#include "EOSMotionMangaSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEOSMotionMangaPanelChangedDelegate, const FString&, SequenceID, int32, PanelIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSMotionMangaFinalPanelDelegate, const FString&, SequenceID);

/**
 * GameInstanceSubsystem managing 2.5D Motion Manga panel sequencing,
 * multi-layer parallax offset calculations, camera motion profiles, and streaming.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSMotionMangaSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSMotionMangaSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Database & Speaker Registry Initialization
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	void InitializeDefaultMangaDatabase();

	// Sequence Lifecycle
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	bool LoadSequence(const FString& SequenceID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	bool StartPlayback();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	bool AdvancePanel();

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	bool GetActivePanel(FEOSMotionMangaPanel& OutPanel) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	int32 GetCurrentPanelIndex() const { return CurrentPanelIndex; }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	int32 GetTotalPanelCount() const { return ActiveSequence.Panels.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	bool IsActivePanelFinal() const;

	// 2.5D Parallax & Camera Math
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	FVector2D CalculateLayerParallaxOffset(const FVector2D& CameraDelta, float LayerDepthRatio, bool bReducedMotion = false) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	float EvaluateCameraScale(EEOSPanelMotionProfile Profile, float NormalizedTime, bool bReducedMotion = false) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	FVector2D EvaluateCameraOffset(EEOSPanelMotionProfile Profile, float NormalizedTime, bool bReducedMotion = false) const;

	// Dialogue & Speaker Resolution
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	void RegisterDialogueEntry(const FEOSDialogueEntry& Entry);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	bool GetDialogueEntry(const FString& DialogueID, FEOSDialogueEntry& OutEntry) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	void RegisterSpeaker(const FString& SpeakerID, const FText& DisplayName, const FString& DefaultPortrait);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	bool ResolveSpeaker(const FString& SpeakerID, FText& OutDisplayName, FString& OutPortrait) const;

	// Panel Streaming & Memory Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	void PreloadNextPanel();

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|MotionManga")
	void ReleasePreviousPanel();

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	bool IsAssetLoaded(const FString& AssetPath) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	FString ResolvePanelAssetWithFallback(const FString& DesiredAsset) const;

	// Telemetry & Statistics
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	int32 GetRegisteredSequenceCount() const { return SequenceRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|MotionManga")
	int32 GetRegisteredDialogueCount() const { return DialogueRegistry.Num(); }

public:
	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|MotionManga")
	FEOSMotionMangaPanelChangedDelegate OnPanelChanged;

	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|MotionManga")
	FEOSMotionMangaFinalPanelDelegate OnFinalPanelReached;

private:
	UPROPERTY()
	FEOSMotionMangaSequenceRecord ActiveSequence;

	UPROPERTY()
	int32 CurrentPanelIndex = 0;

	UPROPERTY()
	bool bIsPlaying = false;

	UPROPERTY()
	TMap<FString, FEOSMotionMangaSequenceRecord> SequenceRegistry;

	UPROPERTY()
	TMap<FString, FEOSDialogueEntry> DialogueRegistry;

	UPROPERTY()
	TMap<FString, FText> SpeakerNames;

	UPROPERTY()
	TMap<FString, FString> SpeakerPortraits;

	UPROPERTY()
	TSet<FString> LoadedAssetCache;
};
