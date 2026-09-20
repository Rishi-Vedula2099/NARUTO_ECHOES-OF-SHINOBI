#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSWorldDataTypes.h"
#include "EOSWorldSubsystem.generated.h"

/**
 * GameInstanceSubsystem managing dense free-roam world exploration,
 * Konoha districts, external Lands & Hidden Villages, fast-travel,
 * lore discovery, weather variants, and story-state damage reflection.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSWorldSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEOSWorldSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Database Initialization
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void InitializeFullWorldDatabase();

	// Zone Management
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void RegisterZone(const FEOSWorldZone& Zone);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	bool GetZone(const FString& ZoneID, FEOSWorldZone& OutZone) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	TArray<FEOSWorldZone> GetZonesByRegion(EEOSWorldRegion Region) const;

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	bool TransitionToZone(const FString& ZoneID);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	FString GetActiveZoneID() const { return ActiveZoneID; }

	// Fast-Travel Network
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void RegisterFastTravelPoint(const FEOSFastTravelPoint& Point);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	bool UnlockFastTravelPoint(const FString& PointID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	bool FastTravelTo(const FString& PointID, FString& OutTargetZoneID);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	TArray<FEOSFastTravelPoint> GetUnlockedFastTravelPoints() const;

	// Interactive Lore Props
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void RegisterLoreProp(const FEOSLoreProp& Prop);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	bool DiscoverLoreProp(const FString& PropID, FString& OutArchiveUnlockID);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	int32 GetDiscoveredLoreCount() const;

	// Dynamic Weather & Time-of-Day
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void SetWeatherTimeVariant(EEOSWeatherTimeVariant NewVariant);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	EEOSWeatherTimeVariant GetActiveWeatherTimeVariant() const { return CurrentWeatherVariant; }

	// World State Damage Reflection
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void ApplyWorldState(const FString& NewWorldStateID);

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	FString GetCurrentWorldState() const { return ActiveWorldStateID; }

	// Auditing & Counts
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	int32 GetTotalZoneCount() const { return ZoneRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	int32 GetTotalFastTravelPointCount() const { return FastTravelRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	int32 GetTotalLorePropCount() const { return LoreRegistry.Num(); }

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|World")
	int32 GetTotalNPCCount() const { return NPCRegistry.Num(); }

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|World")
	void AuditWorldCoverage(int32& OutKonohaZones, int32& OutLandZones, int32& OutDimensionZones, int32& OutTotalLore) const;

private:
	UPROPERTY()
	TMap<FString, FEOSWorldZone> ZoneRegistry;

	UPROPERTY()
	TMap<FString, FEOSFastTravelPoint> FastTravelRegistry;

	UPROPERTY()
	TMap<FString, FEOSLoreProp> LoreRegistry;

	UPROPERTY()
	TMap<FString, FEOSNPCInteractable> NPCRegistry;

	UPROPERTY()
	FString ActiveZoneID = "ZONE_KONOHA_MARKET";

	UPROPERTY()
	EEOSWeatherTimeVariant CurrentWeatherVariant = EEOSWeatherTimeVariant::Day;

	UPROPERTY()
	FString ActiveWorldStateID = "WORLD_STATE_KONOHA_NORMAL";
};
