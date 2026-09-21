#include "../../Public/Subsystems/EOSGearSubsystem.h"
#include "../../EchoesOfShinobi.h"

void UEOSGearSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeDefaultGearDatabase();
}

void UEOSGearSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSGearSubsystem::InitializeDefaultGearDatabase()
{
	// 1. Setup Gear Sets
	FEOSGearSetBonus UchihaSet;
	UchihaSet.SetID = TEXT("SET_UCHIHA_LEGACY");
	UchihaSet.SetName = TEXT("Uchiha Legacy");
	UchihaSet.TwoPieceBonusDescription = TEXT("Fire Nature Damage +15%, Critical Hit Rate +8%");
	UchihaSet.FourPieceBonusDescription = TEXT("Blazing Susanoo Resonance — Critical hits apply Black Flame Burn dealing 3% max HP");
	UchihaSet.TwoPieceAttackBonusPct = 15.0f;
	UchihaSet.TwoPieceCritRateBonusPct = 8.0f;
	UchihaSet.FourPieceElementalDamagePct = 25.0f;
	UchihaSet.FourPieceChakraRegenPct = 12.0f;
	RegisterGearSet(UchihaSet);

	FEOSGearSetBonus WillOfFireSet;
	WillOfFireSet.SetID = TEXT("SET_WILL_OF_FIRE");
	WillOfFireSet.SetName = TEXT("Will of Fire");
	WillOfFireSet.TwoPieceBonusDescription = TEXT("Max Health +20%, Chakra Regeneration +12%");
	WillOfFireSet.FourPieceBonusDescription = TEXT("Unbroken Resolve — Fatal damage triggers 3s invulnerability & 25% heal");
	WillOfFireSet.TwoPieceAttackBonusPct = 10.0f;
	WillOfFireSet.TwoPieceCritRateBonusPct = 5.0f;
	WillOfFireSet.FourPieceElementalDamagePct = 20.0f;
	WillOfFireSet.FourPieceChakraRegenPct = 15.0f;
	RegisterGearSet(WillOfFireSet);

	FEOSGearSetBonus SevenMistSet;
	SevenMistSet.SetID = TEXT("SET_SEVEN_SWORDSMEN");
	SevenMistSet.SetName = TEXT("Seven Swordsmen of the Mist");
	SevenMistSet.TwoPieceBonusDescription = TEXT("Physical Slash Damage +20%, +15% against Mist/Bleed targets");
	SevenMistSet.TwoPieceAttackBonusPct = 20.0f;
	SevenMistSet.TwoPieceCritRateBonusPct = 10.0f;
	RegisterGearSet(SevenMistSet);

	// 2. Setup Core Weapons & Artifacts
	FEOSGearItemRecord Kusanagi;
	Kusanagi.ItemID = TEXT("WEAPON_KUSANAGI_SWORD");
	Kusanagi.DisplayName = TEXT("Sword of Kusanagi (Chidori Blade)");
	Kusanagi.GearType = EEOSMetaGearType::Weapon;
	Kusanagi.WeaponClass = EEOSWeaponClass::SpecialBlade;
	Kusanagi.EquipSlot = EEOSGearSlot::WEAPON_TOOL;
	Kusanagi.CurrentTier = EEOSGearTier::Tier1_Basic;
	Kusanagi.CurrentLevel = 1;
	Kusanagi.MaxLevel = 10;
	Kusanagi.BasePrimaryStatBonus = 120.0f;
	Kusanagi.StatGrowthPerLevel = 8.0f;
	Kusanagi.PassiveSkillID = TEXT("SKILL_CHIDORI_CONDUCTIVITY");
	Kusanagi.SetID = TEXT("SET_UCHIHA_LEGACY");
	RegisterGearItem(Kusanagi);

	FEOSGearItemRecord UchihaFan;
	UchihaFan.ItemID = TEXT("ARTIFACT_UCHIHA_FIRE_FAN");
	UchihaFan.DisplayName = TEXT("Uchiha Crest Flame Talisman");
	UchihaFan.GearType = EEOSMetaGearType::Artifact;
	UchihaFan.EquipSlot = EEOSGearSlot::ACCESSORY_1;
	UchihaFan.CurrentTier = EEOSGearTier::Tier1_Basic;
	UchihaFan.CurrentLevel = 1;
	UchihaFan.MaxLevel = 10;
	UchihaFan.BasePrimaryStatBonus = 80.0f;
	UchihaFan.StatGrowthPerLevel = 5.0f;
	UchihaFan.SetID = TEXT("SET_UCHIHA_LEGACY");
	RegisterGearItem(UchihaFan);

	FEOSGearItemRecord UchihaArmor;
	UchihaArmor.ItemID = TEXT("EQUIP_UCHIHA_COMBAT_ROBE");
	UchihaArmor.DisplayName = TEXT("Uchiha High Collar Battle Robe");
	UchihaArmor.GearType = EEOSMetaGearType::Equipment;
	UchihaArmor.EquipSlot = EEOSGearSlot::CHEST_ARMOR;
	UchihaArmor.CurrentTier = EEOSGearTier::Tier1_Basic;
	UchihaArmor.CurrentLevel = 1;
	UchihaArmor.MaxLevel = 10;
	UchihaArmor.BasePrimaryStatBonus = 95.0f;
	UchihaArmor.StatGrowthPerLevel = 6.0f;
	UchihaArmor.SetID = TEXT("SET_UCHIHA_LEGACY");
	RegisterGearItem(UchihaArmor);

	FEOSGearItemRecord UchihaBoots;
	UchihaBoots.ItemID = TEXT("EQUIP_UCHIHA_GREAVES");
	UchihaBoots.DisplayName = TEXT("Shadow Shuriken Shinobi Boots");
	UchihaBoots.GearType = EEOSMetaGearType::Equipment;
	UchihaBoots.EquipSlot = EEOSGearSlot::BOOTS;
	UchihaBoots.CurrentTier = EEOSGearTier::Tier1_Basic;
	UchihaBoots.CurrentLevel = 1;
	UchihaBoots.MaxLevel = 10;
	UchihaBoots.BasePrimaryStatBonus = 65.0f;
	UchihaBoots.StatGrowthPerLevel = 4.0f;
	UchihaBoots.SetID = TEXT("SET_UCHIHA_LEGACY");
	RegisterGearItem(UchihaBoots);

	FEOSGearItemRecord Samehada;
	Samehada.ItemID = TEXT("WEAPON_SAMEHADA");
	Samehada.DisplayName = TEXT("Samehada (Shark Skin Greatsword)");
	Samehada.GearType = EEOSMetaGearType::Weapon;
	Samehada.WeaponClass = EEOSWeaponClass::SpecialBlade;
	Samehada.EquipSlot = EEOSGearSlot::WEAPON_TOOL;
	Samehada.CurrentTier = EEOSGearTier::Tier1_Basic;
	Samehada.CurrentLevel = 1;
	Samehada.MaxLevel = 10;
	Samehada.BasePrimaryStatBonus = 140.0f;
	Samehada.StatGrowthPerLevel = 9.0f;
	Samehada.SetID = TEXT("SET_SEVEN_SWORDSMEN");
	RegisterGearItem(Samehada);

	FEOSGearItemRecord RaijinKunai;
	RaijinKunai.ItemID = TEXT("WEAPON_FLYING_RAIJIN_KUNAI");
	RaijinKunai.DisplayName = TEXT("Three-Pronged Flying Raijin Kunai");
	RaijinKunai.GearType = EEOSMetaGearType::Weapon;
	RaijinKunai.WeaponClass = EEOSWeaponClass::SignatureWeapon;
	RaijinKunai.EquipSlot = EEOSGearSlot::WEAPON_TOOL;
	RaijinKunai.CurrentTier = EEOSGearTier::Tier1_Basic;
	RaijinKunai.CurrentLevel = 1;
	RaijinKunai.MaxLevel = 10;
	RaijinKunai.BasePrimaryStatBonus = 110.0f;
	RaijinKunai.StatGrowthPerLevel = 7.0f;
	RaijinKunai.SetID = TEXT("SET_WILL_OF_FIRE");
	RegisterGearItem(RaijinKunai);

	FEOSGearItemRecord SageUrn;
	SageUrn.ItemID = TEXT("ARTIFACT_SAGE_TOAD_OIL_URN");
	SageUrn.DisplayName = TEXT("Myoboku Sacred Toad Oil Urn");
	SageUrn.GearType = EEOSMetaGearType::Artifact;
	SageUrn.EquipSlot = EEOSGearSlot::ACCESSORY_1;
	SageUrn.CurrentTier = EEOSGearTier::Tier1_Basic;
	SageUrn.CurrentLevel = 1;
	SageUrn.MaxLevel = 10;
	SageUrn.BasePrimaryStatBonus = 75.0f;
	SageUrn.StatGrowthPerLevel = 5.0f;
	SageUrn.SetID = TEXT("SET_WILL_OF_FIRE");
	RegisterGearItem(SageUrn);
}

bool UEOSGearSubsystem::RegisterGearItem(const FEOSGearItemRecord& ItemRecord)
{
	if (ItemRecord.ItemID.IsEmpty()) return false;
	ItemInventory.Add(ItemRecord.ItemID, ItemRecord);
	return true;
}

bool UEOSGearSubsystem::GetGearItem(const FString& ItemID, FEOSGearItemRecord& OutItem) const
{
	if (const FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		OutItem = *Found;
		return true;
	}
	return false;
}

TArray<FEOSGearItemRecord> UEOSGearSubsystem::GetInventoryItems() const
{
	TArray<FEOSGearItemRecord> Items;
	for (const auto& Pair : ItemInventory)
	{
		Items.Add(Pair.Value);
	}
	return Items;
}

bool UEOSGearSubsystem::SetItemLocked(const FString& ItemID, bool bLocked)
{
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		Found->bIsLocked = bLocked;
		return true;
	}
	return false;
}

bool UEOSGearSubsystem::SetItemFavorite(const FString& ItemID, bool bFavorite)
{
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		Found->bIsFavorite = bFavorite;
		return true;
	}
	return false;
}

bool UEOSGearSubsystem::EnhanceGearItem(const FString& ItemID, int32 LevelsGained)
{
	if (LevelsGained <= 0) return false;
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		int32 NewLevel = FMath::Clamp(Found->CurrentLevel + LevelsGained, 1, Found->MaxLevel);
		int32 Delta = NewLevel - Found->CurrentLevel;
		if (Delta > 0)
		{
			Found->CurrentLevel = NewLevel;
			Found->BasePrimaryStatBonus += (Delta * Found->StatGrowthPerLevel);
			return true;
		}
	}
	return false;
}

bool UEOSGearSubsystem::AdvanceGearTier(const FString& ItemID)
{
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		if (Found->CurrentLevel >= Found->MaxLevel && Found->CurrentTier != EEOSGearTier::Tier5_Transcendent)
		{
			switch (Found->CurrentTier)
			{
			case EEOSGearTier::Tier1_Basic:
				Found->CurrentTier = EEOSGearTier::Tier2_Reinforced;
				break;
			case EEOSGearTier::Tier2_Reinforced:
				Found->CurrentTier = EEOSGearTier::Tier3_Forged;
				break;
			case EEOSGearTier::Tier3_Forged:
				Found->CurrentTier = EEOSGearTier::Tier4_Masterwork;
				break;
			case EEOSGearTier::Tier4_Masterwork:
				Found->CurrentTier = EEOSGearTier::Tier5_Transcendent;
				break;
			default:
				return false;
			}
			Found->MaxLevel += 10;
			Found->BasePrimaryStatBonus *= 1.25f; // +25% stat multiplier breakthrough
			return true;
		}
	}
	return false;
}

bool UEOSGearSubsystem::RegisterGearSet(const FEOSGearSetBonus& SetBonus)
{
	if (SetBonus.SetID.IsEmpty()) return false;
	SetRegistry.Add(SetBonus.SetID, SetBonus);
	return true;
}

bool UEOSGearSubsystem::GetGearSet(const FString& SetID, FEOSGearSetBonus& OutSet) const
{
	if (const FEOSGearSetBonus* Found = SetRegistry.Find(SetID))
	{
		OutSet = *Found;
		return true;
	}
	return false;
}

int32 UEOSGearSubsystem::EvaluateEquippedSetCount(const FString& CharacterID, const FString& SetID) const
{
	if (CharacterID.IsEmpty() || SetID.IsEmpty()) return 0;
	int32 Count = 0;
	for (const auto& Pair : ItemInventory)
	{
		if (Pair.Value.EquippedCharacterID == CharacterID && Pair.Value.SetID == SetID)
		{
			Count++;
		}
	}
	return Count;
}

bool UEOSGearSubsystem::IsSetBonusActive(const FString& CharacterID, const FString& SetID, int32 RequiredPieces) const
{
	return EvaluateEquippedSetCount(CharacterID, SetID) >= RequiredPieces;
}

bool UEOSGearSubsystem::EquipItemToCharacter(const FString& CharacterID, const FString& ItemID, EEOSGearSlot Slot)
{
	if (CharacterID.IsEmpty() || ItemID.IsEmpty()) return false;
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		// Unequip previous item in this slot on this character
		for (auto& Pair : ItemInventory)
		{
			if (Pair.Value.EquippedCharacterID == CharacterID && Pair.Value.EquipSlot == Slot)
			{
				Pair.Value.EquippedCharacterID = TEXT("");
			}
		}

		Found->EquippedCharacterID = CharacterID;
		Found->EquipSlot = Slot;
		return true;
	}
	return false;
}

bool UEOSGearSubsystem::UnequipItem(const FString& ItemID)
{
	if (FEOSGearItemRecord* Found = ItemInventory.Find(ItemID))
	{
		Found->EquippedCharacterID = TEXT("");
		return true;
	}
	return false;
}

TArray<FEOSGearItemRecord> UEOSGearSubsystem::GetCharacterEquippedGear(const FString& CharacterID) const
{
	TArray<FEOSGearItemRecord> Equipped;
	for (const auto& Pair : ItemInventory)
	{
		if (Pair.Value.EquippedCharacterID == CharacterID)
		{
			Equipped.Add(Pair.Value);
		}
	}
	return Equipped;
}

bool UEOSGearSubsystem::SaveBuildLoadout(const FString& CharacterID, const FEOSCharacterBuildLoadout& Loadout)
{
	if (CharacterID.IsEmpty() || Loadout.BuildID.IsEmpty()) return false;
	CharacterBuilds.FindOrAdd(CharacterID).Add(Loadout.BuildID, Loadout);
	return true;
}

bool UEOSGearSubsystem::GetBuildLoadout(const FString& CharacterID, const FString& BuildID, FEOSCharacterBuildLoadout& OutLoadout) const
{
	if (const TMap<FString, FEOSCharacterBuildLoadout>* FoundMap = CharacterBuilds.Find(CharacterID))
	{
		if (const FEOSCharacterBuildLoadout* Found = FoundMap->Find(BuildID))
		{
			OutLoadout = *Found;
			return true;
		}
	}
	return false;
}

float UEOSGearSubsystem::CalculateGearPower(const FString& CharacterID) const
{
	float Power = 0.0f;
	TArray<FEOSGearItemRecord> Equipped = GetCharacterEquippedGear(CharacterID);
	for (const FEOSGearItemRecord& Item : Equipped)
	{
		Power += Item.BasePrimaryStatBonus * 2.5f;
		Power += (int32)Item.CurrentTier * 50.0f;
	}

	// Set bonuses provide dynamic power multipliers
	for (const auto& Pair : SetRegistry)
	{
		int32 Pieces = EvaluateEquippedSetCount(CharacterID, Pair.Key);
		if (Pieces >= 2) Power += 150.0f;
		if (Pieces >= 4) Power += 350.0f;
	}

	return Power;
}
