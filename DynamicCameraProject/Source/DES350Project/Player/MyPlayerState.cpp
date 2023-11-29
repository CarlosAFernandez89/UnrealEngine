// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "DES350Project/GameplayAbility/MyAbilitySystemComponent.h"
#include "HUD/MyHUD.h"

AMyPlayerState::AMyPlayerState()
{
	MyAbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	MyAbilitySystemComponent->SetIsReplicated(true);

	MyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	MyCharacterAttributeSetBase = CreateDefaultSubobject<UMyCharacterAttributeSetBase>(TEXT("AttributeSetBase"));
	NetUpdateFrequency = 100.f;

	
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return MyAbilitySystemComponent;
}


UMyCharacterAttributeSetBase* AMyPlayerState::GetAttributeSetBase() const
{
	return MyCharacterAttributeSetBase;
}

bool AMyPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AMyPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	if (PC)
	{
		AMyHUD* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->ShowAbilityConfirmCancelText(ShowText);
		}
	}
}

float AMyPlayerState::GetHealth() const
{
	return MyCharacterAttributeSetBase->GetHealth();
}

float AMyPlayerState::GetMaxHealth() const
{
	return MyCharacterAttributeSetBase->GetMaxHealth();
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(MyAbilitySystemComponent)
	{
		HealthChangedDelegateHandle = MyAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MyCharacterAttributeSetBase->GetHealthAttribute()).AddUObject(this, &AMyPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = MyAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MyCharacterAttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AMyPlayerState::MaxHealthChanged);

	}
}

void AMyPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	AMyCharacter* Hero = Cast<AMyCharacter>(GetPawn());
	if(Hero)
	{
		// TODO:Update Widget stuff here
	}

	if(!IsAlive() && !MyAbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if(Hero)
		{
			Hero->Die();
		}
	}
}

void AMyPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

	// Update floating status bar
	AMyCharacter* Hero = Cast<AMyCharacter>(GetPawn());
	if (Hero)
	{

	}

	// Update the HUD
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	if (PC)
	{

	}
}

bool AMyPlayerState::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration)
{
	if (MyAbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = MyAbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}