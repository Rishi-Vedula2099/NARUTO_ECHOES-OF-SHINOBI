#pragma once

#include "CoreMinimal.h"

class ECHOESOFSHINOBI_API FEOSPhase2Tests
{
public:
	static bool RunAllPhase1AndPhase2ValidationTests();

	// Phase 1 Tests
	static bool TestAttributeSetInitialization();
	static bool TestDataValidationSubsystem();
	static bool TestSaveSubsystemSerialization();
	static bool TestTelemetrySubsystemRecording();

	// Phase 2 Tests
	static bool TestCharacterPowerFormula();
	static bool TestStagePowerThresholdScaling();
	static bool TestGearDefinitionAndSlotMapping();
	static bool TestSageAndTailedBeastCompatibility();
};
