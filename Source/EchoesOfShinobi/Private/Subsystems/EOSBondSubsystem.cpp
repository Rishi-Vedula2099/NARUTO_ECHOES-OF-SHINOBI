#include "../../Public/Subsystems/EOSBondSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSBondSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultBondDatabase();
}

void UEOSBondSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSBondSubsystem::InitializeDefaultBondDatabase()
{
	// 1. Setup Canon Bonds
	FEOSCharacterBondRecord NarutoSasuke;
	NarutoSasuke.BondID = TEXT("BOND_NARUTO_SASUKE");
	NarutoSasuke.CharacterA = TEXT("CHR_NARUTO_GENIN");
	NarutoSasuke.CharacterB = TEXT("CHR_SASUKE_GENIN");
	NarutoSasuke.BondType = EEOSBondType::Rival;
	NarutoSasuke.BondLevel = 1;
	NarutoSasuke.CurrentXP = 0;
	NarutoSasuke.RequiredXP = 1000;
	NarutoSasuke.TeamSynergyEffectDescription = TEXT("Rival Resonance: +15% Tag-Team Swap Speed & +10% Linked Jutsu Damage");
	NarutoSasuke.SynergySwapSpeedBonusPct = 15.0f;
	NarutoSasuke.SynergyJutsuPowerBonusPct = 10.0f;
	NarutoSasuke.bIsMaxLevel = false;
	RegisterBond(NarutoSasuke);

	FEOSCharacterBondRecord JiraiyaNaruto;
	JiraiyaNaruto.BondID = TEXT("BOND_JIRAIYA_NARUTO");
	JiraiyaNaruto.CharacterA = TEXT("CHR_JIRAIYA_SAGE");
	JiraiyaNaruto.CharacterB = TEXT("CHR_NARUTO_GENIN");
	JiraiyaNaruto.BondType = EEOSBondType::MasterStudent;
	JiraiyaNaruto.BondLevel = 1;
	JiraiyaNaruto.CurrentXP = 0;
	JiraiyaNaruto.RequiredXP = 1000;
	JiraiyaNaruto.TeamSynergyEffectDescription = TEXT("Gallant Mentor: +20% Chakra Generation for Apprentice");
	JiraiyaNaruto.SynergySwapSpeedBonusPct = 10.0f;
	JiraiyaNaruto.SynergyJutsuPowerBonusPct = 12.0f;
	RegisterBond(JiraiyaNaruto);

	FEOSCharacterBondRecord KakashiObito;
	KakashiObito.BondID = TEXT("BOND_KAKASHI_OBITO");
	KakashiObito.CharacterA = TEXT("CHR_KAKASHI_HATAKE");
	KakashiObito.CharacterB = TEXT("CHR_OBITO_MASK");
	KakashiObito.BondType = EEOSBondType::HistoricalConnection;
	KakashiObito.BondLevel = 1;
	KakashiObito.CurrentXP = 0;
	KakashiObito.RequiredXP = 1000;
	KakashiObito.TeamSynergyEffectDescription = TEXT("Dual Kamui Paradox: Space-time evasion bonus");
	RegisterBond(KakashiObito);

	// 2. Setup Contextual Dialogue
	FEOSBondDialogueRecord D1;
	D1.DialogueID = TEXT("DLG_NS_START");
	D1.BondID = TEXT("BOND_NARUTO_SASUKE");
	D1.Trigger = EEOSBondDialogueTrigger::MissionStart;
	D1.SpeakerCharID = TEXT("CHR_NARUTO_GENIN");
	D1.DialogueText = TEXT("Hey Sasuke, just try to keep up with me out there!");
	RegisterBondDialogue(D1);

	FEOSBondDialogueRecord D2;
	D2.DialogueID = TEXT("DLG_NS_LOWHP");
	D2.BondID = TEXT("BOND_NARUTO_SASUKE");
	D2.Trigger = EEOSBondDialogueTrigger::LowHP;
	D2.SpeakerCharID = TEXT("CHR_NARUTO_GENIN");
	D2.DialogueText = TEXT("Sasuke! Don't you dare fall on me here!");
	RegisterBondDialogue(D2);

	FEOSBondDialogueRecord D3;
	D3.DialogueID = TEXT("DLG_NS_SWAP");
	D3.BondID = TEXT("BOND_NARUTO_SASUKE");
	D3.Trigger = EEOSBondDialogueTrigger::CharacterSwap;
	D3.SpeakerCharID = TEXT("CHR_SASUKE_GENIN");
	D3.DialogueText = TEXT("Took you long enough, Naruto. Chidori primed!");
	RegisterBondDialogue(D3);

	FEOSBondDialogueRecord D4;
	D4.DialogueID = TEXT("DLG_NS_ULT");
	D4.BondID = TEXT("BOND_NARUTO_SASUKE");
	D4.Trigger = EEOSBondDialogueTrigger::UltimateCombo;
	D4.SpeakerCharID = TEXT("CHR_NARUTO_GENIN");
	D4.DialogueText = TEXT("Synchronized Strike — Scorch Style: Halo Hurricane Jet Black Arrow!");
	RegisterBondDialogue(D4);
}

bool UEOSBondSubsystem::RegisterBond(const FEOSCharacterBondRecord& BondRecord)
{
	if (BondRecord.BondID.IsEmpty()) return false;
	BondRegistry.Add(BondRecord.BondID, BondRecord);
	return true;
}

bool UEOSBondSubsystem::GetBond(const FString& BondID, FEOSCharacterBondRecord& OutBond) const
{
	if (const FEOSCharacterBondRecord* Found = BondRegistry.Find(BondID))
	{
		OutBond = *Found;
		return true;
	}
	return false;
}

bool UEOSBondSubsystem::GetBondBetweenCharacters(const FString& CharA, const FString& CharB, FEOSCharacterBondRecord& OutBond) const
{
	for (const auto& Pair : BondRegistry)
	{
		const FEOSCharacterBondRecord& B = Pair.Value;
		if ((B.CharacterA == CharA && B.CharacterB == CharB) ||
		    (B.CharacterA == CharB && B.CharacterB == CharA))
		{
			OutBond = B;
			return true;
		}
	}
	return false;
}

TArray<FEOSCharacterBondRecord> UEOSBondSubsystem::GetAllBonds() const
{
	TArray<FEOSCharacterBondRecord> Result;
	for (const auto& Pair : BondRegistry)
	{
		Result.Add(Pair.Value);
	}
	return Result;
}

bool UEOSBondSubsystem::AddBondXP(const FString& BondID, int32 AmountGained)
{
	if (AmountGained <= 0) return false;
	if (FEOSCharacterBondRecord* Found = BondRegistry.Find(BondID))
	{
		if (Found->bIsMaxLevel) return false;

		Found->CurrentXP += AmountGained;
		while (Found->CurrentXP >= Found->RequiredXP && Found->BondLevel < 5)
		{
			Found->CurrentXP -= Found->RequiredXP;
			Found->BondLevel++;
			Found->RequiredXP += 500; // Scaling threshold
			if (Found->BondLevel >= 5)
			{
				Found->BondLevel = 5;
				Found->bIsMaxLevel = true;
				break;
			}
		}
		return true;
	}
	return false;
}

bool UEOSBondSubsystem::SetBondLevel(const FString& BondID, int32 Level)
{
	if (FEOSCharacterBondRecord* Found = BondRegistry.Find(BondID))
	{
		Found->BondLevel = FMath::Clamp(Level, 1, 5);
		Found->bIsMaxLevel = (Found->BondLevel >= 5);
		return true;
	}
	return false;
}

bool UEOSBondSubsystem::RegisterBondDialogue(const FEOSBondDialogueRecord& DialogueRecord)
{
	if (DialogueRecord.DialogueID.IsEmpty() || DialogueRecord.BondID.IsEmpty()) return false;
	DialogueRegistry.FindOrAdd(DialogueRecord.BondID).Add(DialogueRecord);
	return true;
}

bool UEOSBondSubsystem::GetDialogueForTrigger(const FString& BondID, EEOSBondDialogueTrigger Trigger, FEOSBondDialogueRecord& OutDialogue) const
{
	if (const TArray<FEOSBondDialogueRecord>* FoundList = DialogueRegistry.Find(BondID))
	{
		for (const FEOSBondDialogueRecord& D : *FoundList)
		{
			if (D.Trigger == Trigger)
			{
				OutDialogue = D;
				return true;
			}
		}
	}
	return false;
}

bool UEOSBondSubsystem::EvaluateTeamSynergy(const FString& CharA, const FString& CharB, float& OutSwapSpeedBonus, float& OutJutsuBonus) const
{
	FEOSCharacterBondRecord Bond;
	if (GetBondBetweenCharacters(CharA, CharB, Bond))
	{
		if (Bond.BondLevel >= 2)
		{
			float LevelMultiplier = (float)Bond.BondLevel / 5.0f;
			OutSwapSpeedBonus = Bond.SynergySwapSpeedBonusPct * LevelMultiplier;
			OutJutsuBonus = Bond.SynergyJutsuPowerBonusPct * LevelMultiplier;
			return true;
		}
	}
	OutSwapSpeedBonus = 0.0f;
	OutJutsuBonus = 0.0f;
	return false;
}
