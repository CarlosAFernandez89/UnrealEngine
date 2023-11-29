// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWaitReceiveDamage.h"

#include "DES350Project/GameplayAbility/MyAbilitySystemComponent.h"

void UMyWaitReceiveDamage::Activate()
{
	UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC)
	{
		ASC->ReceivedDamage.AddDynamic(this, &UMyWaitReceiveDamage::OnDamageReceived);
	}
}

void UMyWaitReceiveDamage::OnDamageReceived(UMyAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
                                            float MitigatedDamage)
{

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}

UMyWaitReceiveDamage* UMyWaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	UMyWaitReceiveDamage* MyObj = NewAbilityTask<UMyWaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UMyWaitReceiveDamage::OnDestroy(bool AbilityIsEnding)
{
	UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC)
	{
		ASC->ReceivedDamage.RemoveDynamic(this, &UMyWaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);
}
