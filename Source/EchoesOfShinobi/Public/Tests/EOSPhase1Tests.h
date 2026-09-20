#pragma once

#include "CoreMinimal.h"

class ECHOESOFSHINOBI_API FEOSPhase1Tests
{
public:
	static bool RunAllPhase1ValidationTests();
	static bool TestAttributeSetInitialization();
	static bool TestDataValidationSubsystem();
	static bool TestSaveSubsystemSerialization();
	static bool TestTelemetrySubsystemRecording();
};
