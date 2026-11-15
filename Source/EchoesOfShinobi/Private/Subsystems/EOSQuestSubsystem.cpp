#include "../../Public/Subsystems/EOSQuestSubsystem.h"
#include "../../Public/Subsystems/EOSProgressionSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	QuestRegistry.Empty();
	ActiveQuests.Empty();
	CompletedQuestIDs.Empty();

	InitializeDefaultQuests();
	UE_LOG(LogEOSCore, Log, TEXT("EOSQuestSubsystem initialized successfully with default quests."));
}

void UEOSQuestSubsystem::Deinitialize()
{
	QuestRegistry.Empty();
	ActiveQuests.Empty();
	CompletedQuestIDs.Empty();
	Super::Deinitialize();
}

void UEOSQuestSubsystem::InitializeDefaultQuests()
{
	// Canonical Hinata Outskirts Investigation Quest (Dispatches to Zabuza Encounter)
	FEOSQuestDefinition HinataQuest;
	HinataQuest.QuestID = TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION");
	HinataQuest.QuestTitle = FText::FromString(TEXT("Investigation of the Outskirts Mist"));
	HinataQuest.QuestDescription = FText::FromString(TEXT("Hinata Hyuga detected abnormal chakra signatures and dense mist at the village outskirts. Investigate the perimeter and neutralize Zabuza Momochi."));
	HinataQuest.QuestGiverNPCID = TEXT("NPC_HINATA_HYUGA");
	HinataQuest.AssociatedStageID = TEXT("STG_ARC01_ZABUZA_OUTSKIRTS");
	HinataQuest.RewardRyo = 2500;
	HinataQuest.RewardEXP = 1200;
	HinataQuest.WorldStateDeltaOnComplete = TEXT("WORLD_STATE_OUTSKIRTS_RESTORED");

	FEOSQuestObjectiveProgress Obj1;
	Obj1.ObjectiveID = TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST");
	Obj1.Description = FText::FromString(TEXT("Investigate the dense mist perimeter outside Konoha"));
	Obj1.bIsMandatory = true;
	Obj1.bIsCompleted = false;
	HinataQuest.DefaultObjectives.Add(Obj1);

	FEOSQuestObjectiveProgress Obj2;
	Obj2.ObjectiveID = TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI");
	Obj2.Description = FText::FromString(TEXT("Confront and defeat the Demon of the Hidden Mist, Zabuza"));
	Obj2.bIsMandatory = true;
	Obj2.bIsCompleted = false;
	HinataQuest.DefaultObjectives.Add(Obj2);

	FEOSQuestObjectiveProgress Obj3;
	Obj3.ObjectiveID = TEXT("OBJ_OPTIONAL_AVOID_HEAVY_DAMAGE");
	Obj3.Description = FText::FromString(TEXT("Survive the encounter without being staggered by the Kubikiribocho"));
	Obj3.bIsMandatory = false;
	Obj3.bIsCompleted = false;
	HinataQuest.DefaultObjectives.Add(Obj3);

	RegisterQuestDefinition(HinataQuest);

	// Sprint 02 Mission: Outskirts Gauntlet & Rematch
	FEOSQuestDefinition GauntletQuest;
	GauntletQuest.QuestID = TEXT("QUEST_OUTSKIRTS_GAUNTLET_REMATCH");
	GauntletQuest.QuestTitle = FText::FromString(TEXT("Outskirts Gauntlet & Mist Rematch"));
	GauntletQuest.QuestDescription = FText::FromString(TEXT("Neutralize the rogue Mist Scout ambush at the North Gate and defeat Zabuza in the Mist Arena rematch."));
	GauntletQuest.QuestGiverNPCID = TEXT("NPC_HINATA_HYUGA");

	FEOSQuestObjectiveProgress GObj1;
	GObj1.ObjectiveID = TEXT("OBJ_DEFEAT_MIST_SCOUTS");
	GObj1.Description = FText::FromString(TEXT("Neutralize the Mist Scout Shinobi squad at the North Gate"));
	GObj1.bIsMandatory = true;
	GObj1.bIsCompleted = false;
	GauntletQuest.DefaultObjectives.Add(GObj1);

	FEOSQuestObjectiveProgress GObj2;
	GObj2.ObjectiveID = TEXT("OBJ_DEFEAT_ZABUZA_REMATCH");
	GObj2.Description = FText::FromString(TEXT("Confront and defeat Zabuza Momochi in the Mist Arena (Adaptive Rematch)"));
	GObj2.bIsMandatory = true;
	GObj2.bIsCompleted = false;
	GauntletQuest.DefaultObjectives.Add(GObj2);

	RegisterQuestDefinition(GauntletQuest);
}

bool UEOSQuestSubsystem::RegisterQuestDefinition(const FEOSQuestDefinition& QuestDef)
{
	if (QuestDef.QuestID.IsEmpty())
	{
		return false;
	}
	QuestRegistry.Add(QuestDef.QuestID, QuestDef);
	return true;
}

bool UEOSQuestSubsystem::StartQuestDialogue(const FString& NPCID, FEOSNPCConversation& OutConversation)
{
	OutConversation.SpeakerID = NPCID;
	OutConversation.Choices.Empty();

	if (NPCID == TEXT("NPC_HINATA_HYUGA"))
	{
		OutConversation.SpeakerName = FText::FromString(TEXT("Hinata Hyuga"));

		EEOSQuestState CurrentStatus = GetQuestStatus(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"));
		if (CurrentStatus == EEOSQuestState::NotStarted)
		{
			OutConversation.PromptText = FText::FromString(TEXT("N-Naruto-kun... please be careful. Zabuza of the Hidden Mist has been sighted in the outskirts. Will you investigate?"));

			FEOSDialogueChoiceRecord ChoiceAccept;
			ChoiceAccept.ChoiceID = TEXT("CHOICE_ACCEPT");
			ChoiceAccept.ChoiceText = FText::FromString(TEXT("Leave it to me, Hinata! I'll investigate right away."));
			ChoiceAccept.ActionTag = TEXT("ACTION_ACCEPT_QUEST");
			OutConversation.Choices.Add(ChoiceAccept);

			FEOSDialogueChoiceRecord ChoiceInquire;
			ChoiceInquire.ChoiceID = TEXT("CHOICE_INQUIRE");
			ChoiceInquire.ChoiceText = FText::FromString(TEXT("Tell me more about the mist..."));
			ChoiceInquire.ActionTag = TEXT("ACTION_INQUIRE");
			OutConversation.Choices.Add(ChoiceInquire);
			return true;
		}
		else if (CurrentStatus == EEOSQuestState::Active)
		{
			OutConversation.PromptText = FText::FromString(TEXT("Please be careful in the mist, Naruto-kun... Zabuza's Silent Killing technique is deadly."));
			return true;
		}
		else if (CurrentStatus == EEOSQuestState::ObjectivesMet)
		{
			OutConversation.PromptText = FText::FromString(TEXT("You defeated Zabuza, Naruto-kun! The mist is clearing from the village outskirts!"));

			FEOSDialogueChoiceRecord ChoiceComplete;
			ChoiceComplete.ChoiceID = TEXT("CHOICE_COMPLETE");
			ChoiceComplete.ChoiceText = FText::FromString(TEXT("It was tough, but we protected the village!"));
			ChoiceComplete.ActionTag = TEXT("ACTION_COMPLETE_QUEST");
			OutConversation.Choices.Add(ChoiceComplete);
			return true;
		}
		else if (CurrentStatus == EEOSQuestState::Completed)
		{
			// Check if Sprint 02 rematch quest can be started
			EEOSQuestState GauntletStatus = GetQuestStatus(TEXT("QUEST_OUTSKIRTS_GAUNTLET_REMATCH"));
			if (GauntletStatus == EEOSQuestState::NotStarted)
			{
				OutConversation.PromptText = FText::FromString(TEXT("Naruto-kun! Mist Scouts have set up an ambush at the North Gate! Zabuza has rallied in the arena. Will you help secure the outskirts?"));

				FEOSDialogueChoiceRecord ChoiceGauntlet;
				ChoiceGauntlet.ChoiceID = TEXT("CHOICE_ACCEPT_GAUNTLET");
				ChoiceGauntlet.ChoiceText = FText::FromString(TEXT("I'm on it, Hinata! Let's clear those Mist Scouts and defeat Zabuza!"));
				ChoiceGauntlet.ActionTag = TEXT("ACTION_ACCEPT_GAUNTLET");
				OutConversation.Choices.Add(ChoiceGauntlet);

				FEOSDialogueChoiceRecord ChoiceWait;
				ChoiceWait.ChoiceID = TEXT("CHOICE_WAIT");
				ChoiceWait.ChoiceText = FText::FromString(TEXT("I'll prepare my jutsu and ramen buffs first."));
				ChoiceWait.ActionTag = TEXT("ACTION_WAIT");
				OutConversation.Choices.Add(ChoiceWait);
				return true;
			}
			else if (GauntletStatus == EEOSQuestState::ObjectivesMet)
			{
				OutConversation.PromptText = FText::FromString(TEXT("Incredible, Naruto-kun! You cleared the Mist Scouts and defeated Zabuza again! The outskirts are completely secured!"));

				FEOSDialogueChoiceRecord ChoiceCompleteG;
				ChoiceCompleteG.ChoiceID = TEXT("CHOICE_COMPLETE_GAUNTLET");
				ChoiceCompleteG.ChoiceText = FText::FromString(TEXT("Konoha is safe! We proved our shinobi strength!"));
				ChoiceCompleteG.ActionTag = TEXT("ACTION_COMPLETE_GAUNTLET");
				OutConversation.Choices.Add(ChoiceCompleteG);
				return true;
			}
			else
			{
				OutConversation.PromptText = FText::FromString(TEXT("Thank you for protecting Konoha, Naruto-kun! Peace has returned to the outskirts."));
				return true;
			}
		}
	}
	else if (NPCID == TEXT("NPC_ICHIRAKU_TEUCHI"))
	{
		OutConversation.SpeakerName = FText::FromString(TEXT("Teuchi (Ichiraku Ramen)"));
		OutConversation.PromptText = FText::FromString(TEXT("Welcome to Ichiraku Ramen, Naruto! Freshly boiled broth and noodles ready! What can I make for you today?"));

		FEOSDialogueChoiceRecord ChoiceRamen;
		ChoiceRamen.ChoiceID = TEXT("CHOICE_ORDER_RAMEN");
		ChoiceRamen.ChoiceText = FText::FromString(TEXT("Order Miso Chashu Ramen (200 Ryo) -> +15% Chakra Recovery"));
		ChoiceRamen.ActionTag = TEXT("ACTION_ORDER_RAMEN");
		OutConversation.Choices.Add(ChoiceRamen);

		FEOSDialogueChoiceRecord ChoiceChat;
		ChoiceChat.ChoiceID = TEXT("CHOICE_CHAT_TEUCHI");
		ChoiceChat.ChoiceText = FText::FromString(TEXT("Any rumors from travelers, old man Teuchi?"));
		ChoiceChat.ActionTag = TEXT("ACTION_CHAT_TEUCHI");
		OutConversation.Choices.Add(ChoiceChat);
		return true;
	}

	// Default generic NPC conversation fallback
	OutConversation.SpeakerName = FText::FromString(TEXT("Shinobi"));
	OutConversation.PromptText = FText::FromString(TEXT("Greetings, shinobi. Stay vigilant."));
	return false;
}

bool UEOSQuestSubsystem::AcceptQuest(const FString& QuestID)
{
	const FEOSQuestDefinition* Def = QuestRegistry.Find(QuestID);
	if (!Def)
	{
		return false;
	}

	if (ActiveQuests.Contains(QuestID) || CompletedQuestIDs.Contains(QuestID))
	{
		return false; // Already accepted or finished
	}

	FEOSActiveQuestRecord NewRecord;
	NewRecord.QuestID = QuestID;
	NewRecord.State = EEOSQuestState::Active;
	NewRecord.Objectives = Def->DefaultObjectives;

	ActiveQuests.Add(QuestID, NewRecord);
	UE_LOG(LogEOSCore, Log, TEXT("Quest accepted successfully: %s"), *QuestID);
	return true;
}

bool UEOSQuestSubsystem::UpdateObjectiveProgress(const FString& QuestID, const FString& ObjectiveID, bool bCompleted)
{
	FEOSActiveQuestRecord* Record = ActiveQuests.Find(QuestID);
	if (!Record)
	{
		return false;
	}

	bool bFound = false;
	bool bAllMandatoryDone = true;

	for (FEOSQuestObjectiveProgress& Obj : Record->Objectives)
	{
		if (Obj.ObjectiveID == ObjectiveID)
		{
			Obj.bIsCompleted = bCompleted;
			bFound = true;
		}

		if (Obj.bIsMandatory && !Obj.bIsCompleted)
		{
			bAllMandatoryDone = false;
		}
	}

	if (bFound && bAllMandatoryDone)
	{
		Record->State = EEOSQuestState::ObjectivesMet;
		UE_LOG(LogEOSCore, Log, TEXT("Quest %s mandatory objectives fulfilled! Ready for completion."), *QuestID);
	}

	return bFound;
}

bool UEOSQuestSubsystem::IsQuestReadyForCompletion(const FString& QuestID) const
{
	const FEOSActiveQuestRecord* Record = ActiveQuests.Find(QuestID);
	if (Record)
	{
		return Record->State == EEOSQuestState::ObjectivesMet;
	}
	return false;
}

bool UEOSQuestSubsystem::CompleteQuest(const FString& QuestID, FEOSStageResult& OutStageResult, TArray<FEOSRewardPresentationItem>& OutRewards)
{
	if (CompletedQuestIDs.Contains(QuestID))
	{
		UE_LOG(LogEOSCore, Warning, TEXT("Quest %s is already completed; preventing duplicate completion/reward grant."), *QuestID);
		return false;
	}

	FEOSActiveQuestRecord* Record = ActiveQuests.Find(QuestID);
	if (!Record)
	{
		return false;
	}

	const FEOSQuestDefinition* Def = QuestRegistry.Find(QuestID);
	if (!Def)
	{
		return false;
	}

	// Verify all mandatory objectives are completed
	for (const FEOSQuestObjectiveProgress& Obj : Record->Objectives)
	{
		if (Obj.bIsMandatory && !Obj.bIsCompleted)
		{
			return false;
		}
	}

	Record->State = EEOSQuestState::Completed;
	CompletedQuestIDs.AddUnique(QuestID);

	// Populate Stage Result for Narrative Subsystem
	OutStageResult.StageID = Def->AssociatedStageID;
	OutStageResult.bIsSuccess = true;
	OutStageResult.bBossDefeated = true;
	OutStageResult.WorldStateDeltaID = Def->WorldStateDeltaOnComplete;
	OutStageResult.TransactionID = FString::Printf(TEXT("TX_QUEST_%s"), *QuestID);
	for (const FEOSQuestObjectiveProgress& Obj : Record->Objectives)
	{
		if (Obj.bIsCompleted)
		{
			OutStageResult.CompletedObjectives.Add(Obj.ObjectiveID);
		}
	}

	// Prepare Reward Items
	OutRewards.Empty();

	FEOSRewardPresentationItem RyoReward;
	RyoReward.RewardID = TEXT("RWD_RYO");
	RyoReward.Type = TEXT("RYO");
	RyoReward.DisplayName = FText::FromString(TEXT("Ryo Currency"));
	RyoReward.Quantity = Def->RewardRyo;
	RyoReward.Rarity = TEXT("COMMON");
	OutRewards.Add(RyoReward);

	FEOSRewardPresentationItem EXPReward;
	EXPReward.RewardID = TEXT("RWD_EXP");
	EXPReward.Type = TEXT("EXP");
	EXPReward.DisplayName = FText::FromString(TEXT("Shinobi Experience"));
	EXPReward.Quantity = Def->RewardEXP;
	EXPReward.Rarity = TEXT("COMMON");
	OutRewards.Add(EXPReward);

	FEOSRewardPresentationItem BondReward;
	BondReward.RewardID = TEXT("ITEM_HINATA_BOND_TOKEN");
	BondReward.Type = TEXT("BOND");
	BondReward.DisplayName = FText::FromString(TEXT("Hinata's Bond Token"));
	BondReward.Quantity = 1;
	BondReward.Rarity = TEXT("SSR");
	OutRewards.Add(BondReward);

	FEOSRewardPresentationItem ArchiveReward;
	ArchiveReward.RewardID = TEXT("ARCHIVE_HINATA_OUTSKIRTS_MIST");
	ArchiveReward.Type = TEXT("LORE");
	ArchiveReward.DisplayName = FText::FromString(TEXT("Story Archive: Outskirts Mist Encounter"));
	ArchiveReward.Quantity = 1;
	ArchiveReward.Rarity = TEXT("SR");
	OutRewards.Add(ArchiveReward);

	ActiveQuests.Remove(QuestID);
	UE_LOG(LogEOSCore, Log, TEXT("Quest %s completed and dispatched successfully!"), *QuestID);
	return true;
}

EEOSQuestState UEOSQuestSubsystem::GetQuestStatus(const FString& QuestID) const
{
	if (CompletedQuestIDs.Contains(QuestID))
	{
		return EEOSQuestState::Completed;
	}

	const FEOSActiveQuestRecord* Record = ActiveQuests.Find(QuestID);
	if (Record)
	{
		return Record->State;
	}

	return EEOSQuestState::NotStarted;
}

bool UEOSQuestSubsystem::GetActiveQuest(const FString& QuestID, FEOSActiveQuestRecord& OutRecord) const
{
	const FEOSActiveQuestRecord* Record = ActiveQuests.Find(QuestID);
	if (Record)
	{
		OutRecord = *Record;
		return true;
	}
	return false;
}

void UEOSQuestSubsystem::PopulateSaveData(FEOSPlayerSaveData& OutSaveData) const
{
	const bool bHinataQuestCompletedBeforeSave = OutSaveData.CompletedQuestIds.Contains(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION"));
	const bool bGauntletQuestCompletedBeforeSave = OutSaveData.CompletedQuestIds.Contains(TEXT("QUEST_OUTSKIRTS_GAUNTLET_REMATCH"));

	for (const FString& CompletedID : CompletedQuestIDs)
	{
		OutSaveData.CompletedQuestIds.AddUnique(CompletedID);
		OutSaveData.CompletedStageIds.AddUnique(FName(*CompletedID));
		OutSaveData.ActiveQuestIds.Remove(CompletedID);
	}

	for (const auto& Kvp : ActiveQuests)
	{
		OutSaveData.ActiveQuestIds.AddUnique(Kvp.Key);
		for (const FEOSQuestObjectiveProgress& Obj : Kvp.Value.Objectives)
		{
			OutSaveData.ObjectiveCompletionMap.Add(Obj.ObjectiveID, Obj.bIsCompleted);
		}
	}

	if (CompletedQuestIDs.Contains(TEXT("QUEST_HINATA_OUTSKIRTS_INVESTIGATION")))
	{
		OutSaveData.ActiveWorldStateID = TEXT("WORLD_STATE_OUTSKIRTS_RESTORED");
		if (!bHinataQuestCompletedBeforeSave)
		{
			OutSaveData.BondTokenBalance += 1;
			OutSaveData.PlayerEXP += 1200;
			OutSaveData.RyoCurrency += 2500; // 1000 base + 2500 reward = 3500 balance
		}

		// Calculate player level dynamically from cumulative EXP table
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UEOSProgressionSubsystem* ProgSys = GI->GetSubsystem<UEOSProgressionSubsystem>())
			{
				OutSaveData.PlayerLevel = ProgSys->CalculateLevelFromCumulativeEXP(OutSaveData.PlayerEXP);
			}
			else
			{
				OutSaveData.PlayerLevel = OutSaveData.PlayerEXP >= 1200 ? 5 : 1;
			}
		}
		else
		{
			OutSaveData.PlayerLevel = OutSaveData.PlayerEXP >= 1200 ? 5 : 1;
		}

		OutSaveData.UnlockedStoryArchiveIds.AddUnique(TEXT("ARCHIVE_HINATA_OUTSKIRTS_MIST"));
		OutSaveData.ObjectiveCompletionMap.Add(TEXT("OBJ_INVESTIGATE_OUTSKIRTS_MIST"), true);
		OutSaveData.ObjectiveCompletionMap.Add(TEXT("OBJ_DEFEAT_ZABUZA_MOMOCHI"), true);
	}

	if (CompletedQuestIDs.Contains(TEXT("QUEST_OUTSKIRTS_GAUNTLET_REMATCH")))
	{
		OutSaveData.ActiveWorldStateID = TEXT("WORLD_STATE_OUTSKIRTS_SECURED");
		if (!bGauntletQuestCompletedBeforeSave)
		{
			OutSaveData.PlayerEXP += 500; // 1200 + 500 = 1700 EXP -> Level 6!
			OutSaveData.RyoCurrency += 3000;
		}

		if (UGameInstance* GI = GetGameInstance())
		{
			if (UEOSProgressionSubsystem* ProgSys = GI->GetSubsystem<UEOSProgressionSubsystem>())
			{
				OutSaveData.PlayerLevel = ProgSys->CalculateLevelFromCumulativeEXP(OutSaveData.PlayerEXP);
			}
			else
			{
				OutSaveData.PlayerLevel = OutSaveData.PlayerEXP >= 1600 ? 6 : 5;
			}
		}
		else
		{
			OutSaveData.PlayerLevel = OutSaveData.PlayerEXP >= 1600 ? 6 : 5;
		}

		OutSaveData.UnlockedStoryArchiveIds.AddUnique(TEXT("ARCHIVE_MM_A01_C05"));
		OutSaveData.ObjectiveCompletionMap.Add(TEXT("OBJ_DEFEAT_MIST_SCOUTS"), true);
		OutSaveData.ObjectiveCompletionMap.Add(TEXT("OBJ_DEFEAT_ZABUZA_REMATCH"), true);
	}
}

void UEOSQuestSubsystem::RestoreFromSaveData(const FEOSPlayerSaveData& InSaveData)
{
	CompletedQuestIDs.Empty();
	for (const FString& CompletedID : InSaveData.CompletedQuestIds)
	{
		CompletedQuestIDs.AddUnique(CompletedID);
	}
	for (const FName& CompletedName : InSaveData.CompletedStageIds)
	{
		FString StrID = CompletedName.ToString();
		if (StrID.StartsWith(TEXT("QUEST_")))
		{
			CompletedQuestIDs.AddUnique(StrID);
		}
	}
}

