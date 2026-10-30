#include "../../Public/Subsystems/EOSJutsuTrialSubsystem.h"

UEOSJutsuTrialSubsystem::UEOSJutsuTrialSubsystem()
{
}

void UEOSJutsuTrialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	JutsuRegistry.Empty();
	InitializeCanonJutsuRegistry();
}

void UEOSJutsuTrialSubsystem::Deinitialize()
{
	JutsuRegistry.Empty();
	Super::Deinitialize();
}

void UEOSJutsuTrialSubsystem::InitializeCanonJutsuRegistry()
{
	// 1. Fire Style: Fireball Technique (Snake -> Ram -> Monkey -> Boar -> Horse -> Tiger)
	{
		FEOSJutsuTrialRecord Record;
		Record.JutsuID = TEXT("JUT_FIREBALL");
		Record.JutsuName = TEXT("Fire Style: Fireball Technique");
		Record.CharacterID = TEXT("CHAR_SASUKE");
		Record.EraID = TEXT("ERA_GENIN");
		Record.AdvancementNodeID = TEXT("NODE_KATON_01");
		Record.AdvancementBand = EEOSAdvancementBand::BandA_Foundation;
		Record.LearningMethod = EEOSLearningMethod::HandSign;
		Record.TrialState = EEOSJutsuTrialState::Eligible;
		Record.HandSignSequence.SequenceID = TEXT("SEQ_FIREBALL");
		Record.HandSignSequence.RequiredSigns = { EEOSHandSign::Serpent, EEOSHandSign::Ram, EEOSHandSign::Monkey, EEOSHandSign::Boar, EEOSHandSign::Horse, EEOSHandSign::Tiger };
		Record.HandSignSequence.ExecutionWindowSeconds = 3.5f;
		Record.HandSignSequence.RecognitionDifficulty = 3;
		Record.ChakraModel.TargetGaugeMin = 45.0f;
		Record.ChakraModel.TargetGaugeMax = 65.0f;
		Record.ChakraModel.StabilityDecayRate = 6.0f;
		Record.ChakraModel.HoldDurationRequired = 1.5f;
		Record.MasteryBand = EEOSMasteryBand::Level1_Learned;
		Record.ContinuityStatus = TEXT("CANON_STORY");
		RegisterJutsuRecord(Record);
	}

	// 2. Chidori (One Thousand Birds) (Ox -> Hare -> Monkey)
	{
		FEOSJutsuTrialRecord Record;
		Record.JutsuID = TEXT("JUT_CHIDORI");
		Record.JutsuName = TEXT("Lightning Blade: Chidori");
		Record.CharacterID = TEXT("CHAR_SASUKE");
		Record.EraID = TEXT("ERA_CHUNIN_EXAM");
		Record.AdvancementNodeID = TEXT("NODE_RAITON_01");
		Record.AdvancementBand = EEOSAdvancementBand::BandB_Core;
		Record.LearningMethod = EEOSLearningMethod::HandSign;
		Record.TrialState = EEOSJutsuTrialState::Eligible;
		Record.HandSignSequence.SequenceID = TEXT("SEQ_CHIDORI");
		Record.HandSignSequence.RequiredSigns = { EEOSHandSign::Ox, EEOSHandSign::Hare, EEOSHandSign::Monkey };
		Record.HandSignSequence.ExecutionWindowSeconds = 2.5f;
		Record.HandSignSequence.RecognitionDifficulty = 4;
		Record.ChakraModel.TargetGaugeMin = 45.0f;
		Record.ChakraModel.TargetGaugeMax = 55.0f;
		Record.ChakraModel.StabilityDecayRate = 8.0f;
		Record.ChakraModel.HoldDurationRequired = 2.0f;
		Record.MasteryBand = EEOSMasteryBand::Level1_Learned;
		Record.ContinuityStatus = TEXT("CANON_STORY");
		RegisterJutsuRecord(Record);
	}

	// 3. Spiral Sphere: Rasengan (Pure Shape Transformation, zero hand signs, precision gauge hold)
	{
		FEOSJutsuTrialRecord Record;
		Record.JutsuID = TEXT("JUTSU_NARUTO_RASENGAN");
		Record.JutsuName = TEXT("Rasengan (Spiral Sphere)");
		Record.CharacterID = TEXT("CHAR_NARUTO");
		Record.EraID = TEXT("ERA_SEARCH_FOR_TSUNADE");
		Record.AdvancementNodeID = TEXT("NODE_RASENGAN_01");
		Record.AdvancementBand = EEOSAdvancementBand::BandB_Core;
		Record.LearningMethod = EEOSLearningMethod::ChakraControl;
		Record.TrialState = EEOSJutsuTrialState::Mastered;
		Record.HandSignSequence.SequenceID = TEXT("SEQ_RASENGAN_SHAPE");
		Record.HandSignSequence.RequiredSigns = {};
		Record.HandSignSequence.ExecutionWindowSeconds = 2.0f;
		Record.HandSignSequence.RecognitionDifficulty = 5;
		Record.ChakraModel.TargetGaugeMin = 70.0f;
		Record.ChakraModel.TargetGaugeMax = 95.0f;
		Record.ChakraModel.StabilityDecayRate = 12.0f;
		Record.ChakraModel.HoldDurationRequired = 2.0f;
		Record.MasteryBand = EEOSMasteryBand::Level3_Trained;
		Record.bIsEquipped = true;
		Record.ContinuityStatus = TEXT("CANON_STORY");
		RegisterJutsuRecord(Record);
	}

	// 4. Shadow Clone Technique (Cross Sign: Tiger)
	{
		FEOSJutsuTrialRecord Record;
		Record.JutsuID = TEXT("JUTSU_NARUTO_SHADOW_CLONE");
		Record.JutsuName = TEXT("Shadow Clone Technique");
		Record.CharacterID = TEXT("CHAR_NARUTO");
		Record.EraID = TEXT("ERA_GENIN");
		Record.AdvancementNodeID = TEXT("NODE_CLONE_01");
		Record.AdvancementBand = EEOSAdvancementBand::BandA_Foundation;
		Record.LearningMethod = EEOSLearningMethod::HandSign;
		Record.TrialState = EEOSJutsuTrialState::Mastered;
		Record.HandSignSequence.SequenceID = TEXT("SEQ_SHADOW_CLONE");
		Record.HandSignSequence.RequiredSigns = { EEOSHandSign::Tiger };
		Record.HandSignSequence.ExecutionWindowSeconds = 1.5f;
		Record.HandSignSequence.RecognitionDifficulty = 1;
		Record.ChakraModel.TargetGaugeMin = 40.0f;
		Record.ChakraModel.TargetGaugeMax = 60.0f;
		Record.ChakraModel.StabilityDecayRate = 5.0f;
		Record.ChakraModel.HoldDurationRequired = 1.0f;
		Record.MasteryBand = EEOSMasteryBand::Level4_Expert;
		Record.bIsEquipped = true;
		Record.ContinuityStatus = TEXT("CANON_STORY");
		RegisterJutsuRecord(Record);
	}

	// 5. Water Style: Water Dragon Jutsu (Ox -> Hare -> Monkey -> Rat -> Snake)
	{
		FEOSJutsuTrialRecord Record;
		Record.JutsuID = TEXT("JUTSU_WATER_DRAGON");
		Record.JutsuName = TEXT("Water Style: Water Dragon Jutsu");
		Record.CharacterID = TEXT("CHAR_ZABUZA");
		Record.EraID = TEXT("ERA_LAND_OF_WAVES");
		Record.AdvancementNodeID = TEXT("NODE_SUITON_01");
		Record.AdvancementBand = EEOSAdvancementBand::BandC_Advanced;
		Record.LearningMethod = EEOSLearningMethod::HandSign;
		Record.TrialState = EEOSJutsuTrialState::Eligible;
		Record.HandSignSequence.SequenceID = TEXT("SEQ_WATER_DRAGON_TRIAL");
		Record.HandSignSequence.RequiredSigns = { EEOSHandSign::Ox, EEOSHandSign::Hare, EEOSHandSign::Monkey, EEOSHandSign::Rat, EEOSHandSign::Serpent };
		Record.HandSignSequence.ExecutionWindowSeconds = 4.0f;
		Record.HandSignSequence.RecognitionDifficulty = 5;
		Record.ChakraModel.TargetGaugeMin = 50.0f;
		Record.ChakraModel.TargetGaugeMax = 70.0f;
		Record.ChakraModel.StabilityDecayRate = 8.0f;
		Record.ChakraModel.HoldDurationRequired = 2.0f;
		Record.MasteryBand = EEOSMasteryBand::Level2_Familiar;
		Record.ContinuityStatus = TEXT("CANON_STORY");
		RegisterJutsuRecord(Record);
	}
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
