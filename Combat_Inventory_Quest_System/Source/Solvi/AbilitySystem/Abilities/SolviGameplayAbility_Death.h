// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "SolviGameplayAbility_Death.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SOLVI_API USolviGameplayAbility_Death : public UGSCGameplayAbility
{
	GENERATED_BODY()

public:
	USolviGameplayAbility_Death();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="Solvi|Ability")
	void StartDeath();

	UFUNCTION(BlueprintCallable, Category="Solvi|Ability")
	void FinishDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> MontageToPlay;

protected:

	// If enabled, the ability will automatically call StartDeath.  FinishDeath is always called when the ability ends if the death was started.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Solvi|Death")
	bool bAutoStartDeath;
};
