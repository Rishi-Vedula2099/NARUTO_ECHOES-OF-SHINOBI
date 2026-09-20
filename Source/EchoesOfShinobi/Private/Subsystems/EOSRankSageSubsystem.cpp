#include "../../Public/Subsystems/EOSRankSageSubsystem.h"
#include "Math/UnrealMathUtility.h"

UEOSRankSageSubsystem::UEOSRankSageSubsystem()
{
}

void UEOSRankSageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RankRegistry.Empty();
	SageRegistry.Empty();
}

void UEOSRankSageSubsystem::Deinitialize()
{
	RankRegistry.Empty();
	SageRegistry.Empty();
	Super::Deinitialize();
}

void UEOSRankSageSubsystem::RegisterRankProgression(const FEOSRankProgressionRecord& Record)
{
	if (!Record.CharacterID.IsEmpty())
	{
		RankRegistry.Add(Record.CharacterID, Record);
	}
}

void UEOSRankSageSubsystem::RegisterSagePathway(const FEOSSagePathwayRecord& Record)
{
	if (!Record.CharacterID.IsEmpty())
	{
		SageRegistry.Add(Record.CharacterID, Record);
	}
}

bool UEOSRankSageSubsystem::EvaluateRankEligibility(const FString& CharacterID, EEOSShinobiRank TargetRank, int32 CharacterLevel)
{
	FEOSRankProgressionRecord* Record = RankRegistry.Find(CharacterID);
	if (!Record)
	{
		return false;
	}

	if (CharacterLevel >= Record->RequiredLevel && TargetRank > Record->CurrentRank)
	{
		Record->TargetRank = TargetRank;
		return true;
	}

	return false;
}

bool UEOSRankSageSubsystem::AdvanceRank(const FString& CharacterID, EEOSShinobiRank NewRank)
{
	FEOSRankProgressionRecord* Record = RankRegistry.Find(CharacterID);
	if (!Record)
	{
		return false;
	}

	Record->CurrentRank = NewRank;
	Record->bIsRankPassed = true;
	return true;
}

FEOSMentorTrainingRecord UEOSRankSageSubsystem::ExecuteMentorSession(const FString& CharacterID, const FString& MentorID, const FString& TeachingFocus)
{
	FEOSMentorTrainingRecord Session;
	Session.SessionID = FString::Printf(TEXT("MNT_%s_%s"), *CharacterID, *MentorID);
	Session.StudentCharacterID = CharacterID;
	Session.MentorID = MentorID;
	Session.TeachingFocus = TeachingFocus;
	Session.BonusMasteryXPMultiplier = 1.5f;
	Session.bSessionCompleted = true;

	return Session;
}

EEOSSageCompatibility UEOSRankSageSubsystem::CheckSageCompatibility(const FString& CharacterID, EEOSCharacterEra Era) const
{
	const FEOSSagePathwayRecord* Record = SageRegistry.Find(CharacterID);
	if (Record)
	{
		return Record->Compatibility;
	}

	// Default Canon Rules if not explicitly registered:
	if (CharacterID.Contains(TEXT("NARUTO")))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY; // Toad Sage
	}
	if (CharacterID.Contains(TEXT("SASUKE")))
	{
		return EEOSSageCompatibility::TRAINABLE; // Snake-linked pathway
	}
	if (CharacterID.Contains(TEXT("MITSUKI")))
	{
		return EEOSSageCompatibility::NATURAL_AFFINITY; // Sage Transformation
	}

	return EEOSSageCompatibility::INCOMPATIBLE;
}

bool UEOSRankSageSubsystem::ValidateNatureEnergyBalance(const FEOSNatureEnergyModel& Model, float CurrentEnergyValue, float DurationHeldSeconds) const
{
	const bool bInTargetBand = (CurrentEnergyValue >= Model.TargetGaugeMin) && (CurrentEnergyValue <= Model.TargetGaugeMax);
	const bool bDurationMet = (DurationHeldSeconds >= Model.HoldDurationRequired);

	return bInTargetBand && bDurationMet;
}

bool UEOSRankSageSubsystem::ProcessRankTrialResult(const FString& CharacterID, const FString& TrialID, bool bPassed, float TimeTakenSeconds, float DamageTaken)
{
	FEOSRankProgressionRecord* Record = RankRegistry.Find(CharacterID);
	if (!Record)
	{
		return false;
	}

	if (bPassed)
	{
		Record->CurrentRank = Record->TargetRank;
		Record->bIsRankPassed = true;
		return true;
	}

	return false;
}

bool UEOSRankSageSubsystem::ProcessSageTrialResult(const FString& CharacterID, const FString& SagePathID, bool bPassed)
{
	FEOSSagePathwayRecord* Record = SageRegistry.Find(CharacterID);
	if (!Record)
	{
		return false;
	}

	if (bPassed && Record->Compatibility != EEOSSageCompatibility::INCOMPATIBLE)
	{
		Record->bSageUnlocked = true;
		return true;
	}

	return false;
}

bool UEOSRankSageSubsystem::GetRankRecord(const FString& CharacterID, FEOSRankProgressionRecord& OutRecord) const
{
	const FEOSRankProgressionRecord* Record = RankRegistry.Find(CharacterID);
	if (Record)
	{
		OutRecord = *Record;
		return true;
	}
	return false;
}

bool UEOSRankSageSubsystem::GetSageRecord(const FString& CharacterID, FEOSSagePathwayRecord& OutRecord) const
{
	const FEOSSagePathwayRecord* Record = SageRegistry.Find(CharacterID);
	if (Record)
	{
		OutRecord = *Record;
		return true;
	}
	return false;
}
