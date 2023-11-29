// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInputConfig.h"

const UInputAction* UMyInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for(const FMyInputAction& Action : NativeInputActions)
	{
		if(Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

const UInputAction* UMyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMyInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}
