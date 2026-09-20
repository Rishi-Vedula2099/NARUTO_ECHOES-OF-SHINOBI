#include "../../Public/Subsystems/EOSTailedBeastSubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSTailedBeastSubsystem::UEOSTailedBeastSubsystem()
{
}

void UEOSTailedBeastSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	BeastRegistry.Empty();
	CompatibilityRegistry.Empty();
	RemnantRegistry.Empty();
}

void UEOSTailedBeastSubsystem::Deinitialize()
{
	BeastRegistry.Empty();
	CompatibilityRegistry.Empty();
	RemnantRegistry.Empty();
	Super::Deinitialize();
}

void UEOSTailedBeastSubsystem::RegisterTailedBeast(const FEOSTailedBeastDefinitionRecord& Record)
{
	if (!Record.BeastID.IsEmpty())
	{
		BeastRegistry.Add(Record.BeastID, Record);
	}
}

void UEOSTailedBeastSubsystem::RegisterCompatibility(const FEOSTailedBeastCompatibilityRecord& Record)
{
	FString Key = FString::Printf(TEXT("%s_%s"), *Record.CharacterID, *Record.TailedBeastID);
	CompatibilityRegistry.Add(Key, Record);
}

void UEOSTailedBeastSubsystem::RegisterRemnantProgression(const FEOSRemnantProgressionRecord& Record)
{
	if (!Record.CharacterID.IsEmpty())
	{
		RemnantRegistry.Add(Record.CharacterID, Record);
	}
}

EEOSSageCompatibility UEOSTailedBeastSubsystem::CheckBeastCompatibility(const FString& CharacterID, EEOSCharacterEra Era, const FString& FormID, const FString& BeastID) const
{
	FString Key = FString::Printf(TEXT("%s_%s"), *CharacterID, *BeastID);
	const FEOSTailedBeastCompatibilityRecord* Record = CompatibilityRegistry.Find(Key);
	if (Record)
	{
		return Record->Compatibility;
	}

	// Default Canon Rules:
	if (CharacterID.Contains(TEXT("NARUTO")) && BeastID.Contains(TEXT("KURAMA")))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY;
	}
	if (CharacterID.Contains(TEXT("GAARA")) && BeastID.Contains(TEXT("SHUKAKU")))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY;
	}
	if (CharacterID.Contains(TEXT("BEE")) && BeastID.Contains(TEXT("GYUKI")))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY;
	}

	return EEOSSageCompatibility::INCOMPATIBLE;
}

float UEOSTailedBeastSubsystem::CalculateChakraContribution(const FString& CharacterID, const FString& BeastID) const
{
	const FEOSRemnantProgressionRecord* Record = RemnantRegistry.Find(CharacterID);
	if (!Record)
	{
		return 0.0f;
	}

	float Mult = 1.0f;
	switch (Record->ResonanceState)
	{
	case EEOSTailedBeastResonanceState::Synchronized: Mult = 1.5f; break;
	case EEOSTailedBeastResonanceState::Mastered:     Mult = 2.0f; break;
	case EEOSTailedBeastResonanceState::Resonant:     Mult = 1.25f; break;
	default: Mult = 1.0f; break;
	}

	return Record->ChakraIncrease * Mult;
}

EEOSTailedBeastResonanceState UEOSTailedBeastSubsystem::GainResonanceXP(const FString& CharacterID, const FString& BeastID, float EarnedXP)
{
	FEOSRemnantProgressionRecord* Record = RemnantRegistry.Find(CharacterID);
	if (!Record)
	{
		return EEOSTailedBeastResonanceState::Unsynced;
	}

	Record->CurrentResonanceXP += EarnedXP;

	if (Record->CurrentResonanceXP >= 1000.0f)
	{
		Record->ResonanceState = EEOSTailedBeastResonanceState::Mastered;
	}
	else if (Record->CurrentResonanceXP >= 500.0f)
	{
		Record->ResonanceState = EEOSTailedBeastResonanceState::Synchronized;
	}
	else if (Record->CurrentResonanceXP >= 200.0f)
	{
		Record->ResonanceState = EEOSTailedBeastResonanceState::Resonant;
	}
	else if (Record->CurrentResonanceXP >= 50.0f)
	{
		Record->ResonanceState = EEOSTailedBeastResonanceState::Responsive;
	}
	else
	{
		Record->ResonanceState = EEOSTailedBeastResonanceState::Aware;
	}

	return Record->ResonanceState;
}

bool UEOSTailedBeastSubsystem::ActivateBeastTransformation(const FString& CharacterID, const FString& BeastID, FEOSTailedBeastTransformationState& OutState)
{
	EEOSSageCompatibility Comp = CheckBeastCompatibility(CharacterID, EEOSCharacterEra::ERA_GENIN, "FORM_BASE", BeastID);
	if (Comp == EEOSSageCompatibility::INCOMPATIBLE)
	{
		return false;
	}

	FEOSRemnantProgressionRecord* Record = RemnantRegistry.Find(CharacterID);
	if (!Record || Record->ResonanceState == EEOSTailedBeastResonanceState::Unsynced)
	{
		return false;
	}

	OutState.ActiveBeastID = BeastID;
	OutState.FormID = "FORM_KURAMA_LINK";
	OutState.ResonanceState = Record->ResonanceState;
	OutState.DurationSeconds = 45.0f;
	OutState.bIsActive = true;

	return true;
}

bool UEOSTailedBeastSubsystem::ProcessBeastTrialResult(const FString& CharacterID, const FString& BeastID, bool bPassed, float EarnedPracticeXP)
{
	FEOSRemnantProgressionRecord* Record = RemnantRegistry.Find(CharacterID);
	if (!Record)
	{
		return false;
	}

	GainResonanceXP(CharacterID, BeastID, EarnedPracticeXP);

	if (bPassed)
	{
		Record->CurrentTier += 1;
		return true;
	}

	return false;
}

bool UEOSTailedBeastSubsystem::GetRemnantProgression(const FString& CharacterID, FEOSRemnantProgressionRecord& OutRecord) const
{
	const FEOSRemnantProgressionRecord* Record = RemnantRegistry.Find(CharacterID);
	if (Record)
	{
		OutRecord = *Record;
		return true;
	}
	return false;
}
