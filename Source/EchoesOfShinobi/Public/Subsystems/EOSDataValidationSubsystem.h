#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EditorSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EOSDataTypes.h"
#include "EOSDataValidationSubsystem.generated.h"

UCLASS()
class ECHOESOFSHINOBI_API UEOSDataValidationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "EOS Validation")
	bool ValidateStableId(const FEOSStableId& StableId, EEOSValidationSeverity& OutSeverity);

	UFUNCTION(BlueprintCallable, Category = "EOS Validation")
	bool ValidateChakraCost(float Cost, EEOSValidationSeverity& OutSeverity);
};
