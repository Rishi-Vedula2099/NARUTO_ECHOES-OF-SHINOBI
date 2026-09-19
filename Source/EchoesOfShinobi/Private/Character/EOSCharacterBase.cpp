#include "../../Public/Character/EOSCharacterBase.h"
#include "../../EchoesOfShinobi.h"

AEOSCharacterBase::AEOSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UEOSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEOSAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AEOSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEOSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (CharacterDefinitionAsset && AttributeSet)
		{
			AttributeSet->InitHealth(CharacterDefinitionAsset->BaseHealth);
			AttributeSet->InitMaxHealth(CharacterDefinitionAsset->BaseHealth);
			AttributeSet->InitChakra(CharacterDefinitionAsset->BaseChakra);
			AttributeSet->InitMaxChakra(CharacterDefinitionAsset->BaseChakra);
			AttributeSet->InitAttackPower(CharacterDefinitionAsset->BaseAttack);
			AttributeSet->InitDefensePower(CharacterDefinitionAsset->BaseDefense);
			AttributeSet->InitMovementSpeed(CharacterDefinitionAsset->BaseSpeed);

			UE_LOG(LogEOSCharacter, Log, TEXT("Initialized Character '%s' (Era: %d, Health: %f, Chakra: %f)"),
				*CharacterDefinitionAsset->DisplayName.ToString(),
				static_cast<int32>(CharacterDefinitionAsset->Era),
				CharacterDefinitionAsset->BaseHealth,
				CharacterDefinitionAsset->BaseChakra);
		}
	}
}

void AEOSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEOSCharacterBase::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void AEOSCharacterBase::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}
