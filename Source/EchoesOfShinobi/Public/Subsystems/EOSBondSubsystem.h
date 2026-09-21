#pragma once

#include "../CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSMetaProgressionDataTypes.h"
#include "EOSBondSubsystem.generated.h"

/**
 * Reusable Runtime Character Relationship & Bond Subsystem
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSBondSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Populates canon character relationship bonds and dialogue */
	void InitializeDefaultBondDatabase();

	// -------------------------------------------------------------
	// Bond Registry & Progression
	// -------------------------------------------------------------
	bool RegisterBond(const FEOSCharacterBondRecord& BondRecord);
	bool GetBond(const FString& BondID, FEOSCharacterBondRecord& OutBond) const;
	bool GetBondBetweenCharacters(const FString& CharA, const FString& CharB, FEOSCharacterBondRecord& OutBond) const;
	TArray<FEOSCharacterBondRecord> GetAllBonds() const;

	bool AddBondXP(const FString& BondID, int32 AmountGained);
	bool SetBondLevel(const FString& BondID, int32 Level);

	// -------------------------------------------------------------
	// Contextual Combat & World Dialogue
	// -------------------------------------------------------------
	bool RegisterBondDialogue(const FEOSBondDialogueRecord& DialogueRecord);
	bool GetDialogueForTrigger(const FString& BondID, EEOSBondDialogueTrigger Trigger, FEOSBondDialogueRecord& OutDialogue) const;

	// -------------------------------------------------------------
	// Team Synergy Effects
	// -------------------------------------------------------------
	bool EvaluateTeamSynergy(const FString& CharA, const FString& CharB, float& OutSwapSpeedBonus, float& OutJutsuBonus) const;

private:
	TMap<FString, FEOSCharacterBondRecord> BondRegistry;
	TMap<FString, TArray<FEOSBondDialogueRecord>> DialogueRegistry;
};
