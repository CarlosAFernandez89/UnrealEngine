// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMeleeAbility.h"

#include "AbilitySystemComponent.h"
#include "DES350Project/GameplayAbility/Abilities/AbilityTasks/MyPlayMontageWaitForEvent.h"
#include "DES350Project/Player/Hero/MyHeroCharacter.h"

void UMyMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = MeleeMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UMyPlayMontageWaitForEvent* Task = UMyPlayMontageWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UMyMeleeAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UMyMeleeAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UMyMeleeAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UMyMeleeAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UMyMeleeAbility::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UMyMeleeAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMyMeleeAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMyMeleeAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority )
	{
		AMyHeroCharacter* Hero = Cast<AMyHeroCharacter>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());


	}
	
}
