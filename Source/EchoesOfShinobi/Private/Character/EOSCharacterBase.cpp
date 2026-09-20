#include "../../Public/Character/EOSCharacterBase.h"

AEOSCharacterBase::AEOSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UEOSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEOSAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AEOSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEOSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterDefinitionAsset)
	{
		InitializeFromDefinition(CharacterDefinitionAsset);
	}
}

void AEOSCharacterBase::InitializeFromDefinition(UEOSCharacterDefinition* InDefinitionAsset)
{
	if (!InDefinitionAsset)
	{
		return;
	}

	CharacterDefinitionAsset = InDefinitionAsset;
	RuntimeFramework = InDefinitionAsset->Framework;

	// Populate AttributeSet from Stats
	if (AttributeSet)
	{
		AttributeSet->SetHealth(RuntimeFramework.Stats.MaxHP);
		AttributeSet->SetMaxHealth(RuntimeFramework.Stats.MaxHP);
		AttributeSet->SetChakra(RuntimeFramework.Stats.MaxChakra);
		AttributeSet->SetMaxChakra(RuntimeFramework.Stats.MaxChakra);
		AttributeSet->SetAttackPower(RuntimeFramework.Stats.EffectiveAttack);
		AttributeSet->SetDefensePower(RuntimeFramework.Stats.EffectiveDefense);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

FEOSCombatStatBlock AEOSCharacterBase::GetStats() const
{
	return RuntimeFramework.Stats;
}

FEOSBasicAttackCombo AEOSCharacterBase::GetBasicAttacks() const
{
	return RuntimeFramework.BasicAttacks;
}

FEOSSkillSetContainer AEOSCharacterBase::GetSkills() const
{
	return RuntimeFramework.Skills;
}

FEOSJutsuSlotContainer AEOSCharacterBase::GetJutsu() const
{
	return RuntimeFramework.Jutsu;
}

FEOSUltimateDefinition AEOSCharacterBase::GetUltimate() const
{
	return RuntimeFramework.Ultimate;
}

FEOSFormContainer AEOSCharacterBase::GetForms() const
{
	return RuntimeFramework.Forms;
}

FEOSGearGridContainer AEOSCharacterBase::GetEquipment() const
{
	return RuntimeFramework.Equipment;
}

FEOSCharacterAdvancementData AEOSCharacterBase::GetAdvancement() const
{
	return RuntimeFramework.Advancement;
}

FEOSCharacterMasteryRecord AEOSCharacterBase::GetMastery() const
{
	return RuntimeFramework.Mastery;
}

FEOSCharacterCompatibilityProfile AEOSCharacterBase::GetCompatibility() const
{
	return RuntimeFramework.Compatibility;
}

bool AEOSCharacterBase::TransformToForm(const FString& TargetFormID)
{
	if (RuntimeFramework.Forms.UnlockedFormIDs.Contains(TargetFormID) || TargetFormID == "FORM_BASE")
	{
		RuntimeFramework.Forms.ActiveFormID = TargetFormID;

		// Apply Form Stat Boost
		if (TargetFormID == "FORM_SAGE_MODE")
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.30f; // +30% stats
		}
		else if (TargetFormID == "FORM_KURAMA_LINK" || TargetFormID == "FORM_SUSANOO")
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.50f; // +50% stats
		}
		else
		{
			RuntimeFramework.Forms.ActiveFormStatMultiplier = 1.0f;
		}

		return true;
	}

	return false;
}

void AEOSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEOSCharacterBase::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AEOSCharacterBase::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
}
