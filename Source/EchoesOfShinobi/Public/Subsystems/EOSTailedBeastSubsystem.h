#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSTailedBeastDataTypes.h"
#include "EOSTailedBeastSubsystem.generated.h"

/**
 * GameInstanceSubsystem managing Tailed Beast Definitions, Remnant Power,
 * Compatibility Gating, Resonance Progression, and GAS Transformation States.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSTailedBeastSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSTailedBeastSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Registration Functions
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	void RegisterTailedBeast(const FEOSTailedBeastDefinitionRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	void RegisterCompatibility(const FEOSTailedBeastCompatibilityRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	void RegisterRemnantProgression(const FEOSRemnantProgressionRecord& Record);

	// Compatibility & Remnant Power Queries
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|TailedBeast")
	EEOSSageCompatibility CheckBeastCompatibility(const FString& CharacterID, EEOSCharacterEra Era, const FString& FormID, const FString& BeastID) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|TailedBeast")
	float CalculateChakraContribution(const FString& CharacterID, const FString& BeastID) const;

	// Resonance Progression
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	EEOSTailedBeastResonanceState GainResonanceXP(const FString& CharacterID, const FString& BeastID, float EarnedXP);

	// Transformation Logic
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	bool ActivateBeastTransformation(const FString& CharacterID, const FString& BeastID, FEOSTailedBeastTransformationState& OutState);

	// Trial & Mastery Progression
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|TailedBeast")
	bool ProcessBeastTrialResult(const FString& CharacterID, const FString& BeastID, bool bPassed, float EarnedPracticeXP);

	// Queries
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|TailedBeast")
	bool GetRemnantProgression(const FString& CharacterID, FEOSRemnantProgressionRecord& OutRecord) const;

private:
	UPROPERTY()
	TMap<FString, FEOSTailedBeastDefinitionRecord> BeastRegistry;

	UPROPERTY()
	TMap<FString, FEOSTailedBeastCompatibilityRecord> CompatibilityRegistry;

	UPROPERTY()
	TMap<FString, FEOSRemnantProgressionRecord> RemnantRegistry;
};
