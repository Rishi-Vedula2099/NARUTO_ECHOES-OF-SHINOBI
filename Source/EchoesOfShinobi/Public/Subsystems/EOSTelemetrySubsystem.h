#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EOSDataTypes.h"
#include "EOSTelemetrySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FEOSTelemetrySnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Telemetry")
	int32 TotalDodgeCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Telemetry")
	int32 PerfectDodgeCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Telemetry")
	int32 GuardCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Telemetry")
	float AverageEngagementDistance = 300.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Telemetry")
	TMap<FName, int32> JutsuUsageFrequency;
};

UCLASS()
class ECHOESOFSHINOBI_API UEOSTelemetrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "EOS Telemetry")
	void RecordDodge(bool bIsPerfectDodge);

	UFUNCTION(BlueprintCallable, Category = "EOS Telemetry")
	void RecordJutsuCast(FName JutsuId);

	UFUNCTION(BlueprintCallable, Category = "EOS Telemetry")
	void RecordEngagementDistance(float Distance);

	UFUNCTION(BlueprintPure, Category = "EOS Telemetry")
	const FEOSTelemetrySnapshot& GetTelemetrySnapshot() const { return CurrentSnapshot; }

private:
	FEOSTelemetrySnapshot CurrentSnapshot;
};
