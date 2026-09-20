#include "../../Public/Subsystems/EOSTelemetrySubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSTelemetrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSTelemetry, Log, TEXT("EOSTelemetrySubsystem initialized."));
}

void UEOSTelemetrySubsystem::RecordDodge(bool bIsPerfectDodge)
{
	CurrentSnapshot.TotalDodgeCount++;
	if (bIsPerfectDodge)
	{
		CurrentSnapshot.PerfectDodgeCount++;
	}
	UE_LOG(LogEOSTelemetry, Verbose, TEXT("Dodge Recorded. Perfect: %s"), bIsPerfectDodge ? TEXT("True") : TEXT("False"));
}

void UEOSTelemetrySubsystem::RecordJutsuCast(FName JutsuId)
{
	if (!JutsuId.IsNone())
	{
		CurrentSnapshot.JutsuUsageFrequency.FindOrAdd(JutsuId)++;
		UE_LOG(LogEOSTelemetry, Verbose, TEXT("Jutsu Cast Recorded: %s"), *JutsuId.ToString());
	}
}

void UEOSTelemetrySubsystem::RecordEngagementDistance(float Distance)
{
	CurrentSnapshot.AverageEngagementDistance = (CurrentSnapshot.AverageEngagementDistance + Distance) * 0.5f;
}
