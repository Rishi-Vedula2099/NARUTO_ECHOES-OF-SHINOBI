#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSMetaProgressionDataTypes.h"
#include "EOSGearSubsystem.generated.h"

/**
 * Reusable Runtime Gear, Weapon, Artifact & Loadout Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSGearSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates all default weapons, artifacts, equipment, and gear sets */
	void InitializeDefaultGearDatabase();

	// -------------------------------------------------------------
	// Gear Registry & Inventory
	// -------------------------------------------------------------
	bool RegisterGearItem(const FEOSGearItemRecord& ItemRecord);
	bool GetGearItem(const FString& ItemID, FEOSGearItemRecord& OutItem) const;
	TArray<FEOSGearItemRecord> GetInventoryItems() const;
	bool SetItemLocked(const FString& ItemID, bool bLocked);
	bool SetItemFavorite(const FString& ItemID, bool bFavorite);

	// -------------------------------------------------------------
	// Enhancement & Advancement
	// -------------------------------------------------------------
	bool EnhanceGearItem(const FString& ItemID, int32 LevelsGained);
	bool AdvanceGearTier(const FString& ItemID);

	// -------------------------------------------------------------
	// Gear Sets & Set Bonus Evaluation
	// -------------------------------------------------------------
	bool RegisterGearSet(const FEOSGearSetBonus& SetBonus);
	bool GetGearSet(const FString& SetID, FEOSGearSetBonus& OutSet) const;
	int32 EvaluateEquippedSetCount(const FString& CharacterID, const FString& SetID) const;
	bool IsSetBonusActive(const FString& CharacterID, const FString& SetID, int32 RequiredPieces) const;

	// -------------------------------------------------------------
	// Equipment & Build Loadout Pipeline
	// -------------------------------------------------------------
	bool EquipItemToCharacter(const FString& CharacterID, const FString& ItemID, EEOSGearSlot Slot);
	bool UnequipItem(const FString& ItemID);
	TArray<FEOSGearItemRecord> GetCharacterEquippedGear(const FString& CharacterID) const;
	
	bool SaveBuildLoadout(const FString& CharacterID, const FEOSCharacterBuildLoadout& Loadout);
	bool GetBuildLoadout(const FString& CharacterID, const FString& BuildID, FEOSCharacterBuildLoadout& OutLoadout) const;
	float CalculateGearPower(const FString& CharacterID) const;

private:
	/** Item and set registries */
	TMap<FString, FEOSGearItemRecord> ItemInventory;
	TMap<FString, FEOSGearSetBonus> SetRegistry;

	/** Saved Character Build Loadouts */
	TMap<FString, TMap<FString, FEOSCharacterBuildLoadout>> CharacterBuilds;
};
