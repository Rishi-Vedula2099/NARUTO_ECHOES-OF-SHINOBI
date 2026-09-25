#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSAIDataTypes.h"
#include "EOSAdaptiveAISubsystem.generated.h"

/**
 * GameInstanceSubsystem managing Telemetry Feature Extraction, Boss Memory,
 * Adaptation Director strategy evaluation, StateTree strategy binding, and Fairness Constraints.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSAdaptiveAISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSAdaptiveAISubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Telemetry Feature Extraction
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|AdaptiveAI")
	void RecordCombatEvent(const FString& PlayerID, const FString& EventType, float Value);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|AdaptiveAI")
	FEOSTelemetryFeatureSnapshot ExtractBehaviorFeatures(const FString& PlayerID) const;

	// Boss Memory System (Nemesis Record)
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|AdaptiveAI")
	FEOSBossMemoryRecord GetBossMemory(const FString& BossID) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|AdaptiveAI")
	void SaveBossMemory(const FEOSBossMemoryRecord& Record);

	// Adaptation Director Strategy Selection
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|AdaptiveAI")
	FEOSAdaptationDirective EvaluateAdaptationDirective(const FString& BossID, EEOSAITier Tier, const FEOSTelemetryFeatureSnapshot& Snapshot);

	// Fairness Constraints Validation
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|AdaptiveAI")
	static bool ValidateFairnessConstraints(const FEOSAdaptationDirective& Directive, bool bAttemptsInputReading, bool bAttemptsDamageFormulaTampering);

private:
	UPROPERTY()
	TMap<FString, FEOSTelemetryFeatureSnapshot> PlayerTelemetryRegistry;

	UPROPERTY()
	TMap<FString, FEOSBossMemoryRecord> BossMemoryRegistry;
};
