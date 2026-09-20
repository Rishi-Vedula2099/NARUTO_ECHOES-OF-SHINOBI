#include "../../Public/Subsystems/EOSJutsuTrialSubsystem.h"

UEOSJutsuTrialSubsystem::UEOSJutsuTrialSubsystem()
{
}

void UEOSJutsuTrialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	JutsuRegistry.Empty();
}

void UEOSJutsuTrialSubsystem::Deinitialize()
{
	JutsuRegistry.Empty();
	Super::Deinitialize();
}

void UEOSJutsuTrialSubsystem::RegisterJutsuRecord(const FEOSJutsuTrialRecord& InRecord)
{
	if (!InRecord.JutsuID.IsEmpty())
	{
		JutsuRegistry.Add(InRecord.JutsuID, InRecord);
	}
}

bool UEOSJutsuTrialSubsystem::EvaluateEligibility(const FString& JutsuID, const FString& ReachedAdvancementNodeID)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	if (Record->TrialState == EEOSJutsuTrialState::Locked && Record->AdvancementNodeID == ReachedAdvancementNodeID)
	{
		Record->TrialState = EEOSJutsuTrialState::Eligible;
		return true;
	}

	return Record->TrialState != EEOSJutsuTrialState::Locked;
}

bool UEOSJutsuTrialSubsystem::StartTrial(const FString& JutsuID)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	if (Record->TrialState == EEOSJutsuTrialState::Eligible || Record->TrialState == EEOSJutsuTrialState::Practice)
	{
		Record->TrialState = EEOSJutsuTrialState::Trial;
		return true;
	}

	return false;
}

bool UEOSJutsuTrialSubsystem::ValidateHandSignSequence(const FString& JutsuID, const TArray<EEOSHandSign>& SubmittedSigns, float TimeTakenSeconds)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	if (TimeTakenSeconds > Record->HandSignSequence.ExecutionWindowSeconds)
	{
		return false;
	}

	if (SubmittedSigns.Num() != Record->HandSignSequence.RequiredSigns.Num())
	{
		return false;
	}

	for (int32 i = 0; i < SubmittedSigns.Num(); ++i)
	{
		if (SubmittedSigns[i] != Record->HandSignSequence.RequiredSigns[i])
		{
			return false;
		}
	}

	return true;
}

bool UEOSJutsuTrialSubsystem::ValidateChakraControl(const FString& JutsuID, float CurrentGaugeValue, float DurationHeldSeconds)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	const bool bInTargetBand = (CurrentGaugeValue >= Record->ChakraModel.TargetGaugeMin) &&
	                           (CurrentGaugeValue <= Record->ChakraModel.TargetGaugeMax);

	const bool bDurationMet = (DurationHeldSeconds >= Record->ChakraModel.HoldDurationRequired);

	return bInTargetBand && bDurationMet;
}

EEOSJutsuTrialState UEOSJutsuTrialSubsystem::ProcessTrialResult(const FString& JutsuID, bool bPassed, float EarnedPracticeXP)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return EEOSJutsuTrialState::Locked;
	}

	Record->PracticeXP += EarnedPracticeXP;

	if (bPassed)
	{
		Record->TrialState = EEOSJutsuTrialState::Mastered;
		Record->MasteryBand = EEOSMasteryBand::Level1_Learned;
	}
	else
	{
		// Retain practice progress and return to practice state
		Record->TrialState = EEOSJutsuTrialState::Practice;
	}

	return Record->TrialState;
}

bool UEOSJutsuTrialSubsystem::EquipJutsu(const FString& JutsuID)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	if (Record->MasteryBand == EEOSMasteryBand::Level0_Unlearned)
	{
		return false; // Cannot equip unlearned jutsu
	}

	Record->bIsEquipped = true;
	return true;
}

bool UEOSJutsuTrialSubsystem::StoreJutsu(const FString& JutsuID)
{
	FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (!Record)
	{
		return false;
	}

	Record->bIsEquipped = false;
	return true;
}

bool UEOSJutsuTrialSubsystem::ReplaceJutsu(const FString& OldJutsuID, const FString& NewJutsuID)
{
	if (!StoreJutsu(OldJutsuID))
	{
		return false;
	}

	return EquipJutsu(NewJutsuID);
}

bool UEOSJutsuTrialSubsystem::GetJutsuRecord(const FString& JutsuID, FEOSJutsuTrialRecord& OutRecord) const
{
	const FEOSJutsuTrialRecord* Record = JutsuRegistry.Find(JutsuID);
	if (Record)
	{
		OutRecord = *Record;
		return true;
	}
	return false;
}

TArray<FEOSJutsuTrialRecord> UEOSJutsuTrialSubsystem::GetLearnedArchive(const FString& CharacterID) const
{
	TArray<FEOSJutsuTrialRecord> Archive;
	for (const auto& Kvp : JutsuRegistry)
	{
		if (Kvp.Value.CharacterID == CharacterID && Kvp.Value.MasteryBand != EEOSMasteryBand::Level0_Unlearned)
		{
			Archive.Add(Kvp.Value);
		}
	}
	return Archive;
}

TArray<FEOSJutsuTrialRecord> UEOSJutsuTrialSubsystem::GetEquippedJutsu(const FString& CharacterID) const
{
	TArray<FEOSJutsuTrialRecord> Equipped;
	for (const auto& Kvp : JutsuRegistry)
	{
		if (Kvp.Value.CharacterID == CharacterID && Kvp.Value.bIsEquipped)
		{
			Equipped.Add(Kvp.Value);
		}
	}
	return Equipped;
}
