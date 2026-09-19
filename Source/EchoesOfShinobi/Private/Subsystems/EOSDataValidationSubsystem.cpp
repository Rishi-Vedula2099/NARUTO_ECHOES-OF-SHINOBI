#include "Subsystems/EOSDataValidationSubsystem.h"
#include "EchoesOfShinobi.h"

void UEOSDataValidationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSData, Log, TEXT("EOSDataValidationSubsystem initialized."));
}

bool UEOSDataValidationSubsystem::ValidateStableId(const FEOSStableId& StableId, EEOSValidationSeverity& OutSeverity)
{
	if (!StableId.IsValid())
	{
		OutSeverity = EEOSValidationSeverity::ERROR_BLOCKING;
		UE_LOG(LogEOSData, Error, TEXT("Data Validation Failed: Stable ID is None!"));
		return false;
	}

	if (StableId.Provenance == EEOSProvenanceType::TODO_AUTHORING)
	{
		OutSeverity = EEOSValidationSeverity::TODO_AUTHORING;
		UE_LOG(LogEOSData, Warning, TEXT("Data Validation Warning: ID '%s' marked TODO_AUTHORING."), *StableId.StableId.ToString());
		return true;
	}

	OutSeverity = EEOSValidationSeverity::INFO;
	return true;
}

bool UEOSDataValidationSubsystem::ValidateChakraCost(float Cost, EEOSValidationSeverity& OutSeverity)
{
	if (Cost < 0.0f)
	{
		OutSeverity = EEOSValidationSeverity::ERROR_BLOCKING;
		UE_LOG(LogEOSData, Error, TEXT("Data Validation Failed: Negative Chakra Cost (%f)!"), Cost);
		return false;
	}

	OutSeverity = EEOSValidationSeverity::INFO;
	return true;
}
