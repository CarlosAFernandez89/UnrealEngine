// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDamageExecutionCalculation.h"
#include "DES350Project/GameplayAbility/MyAbilitySystemComponent.h"
#include "DES350Project/GameplayAbility/MyCharacterAttributeSetBase.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct MyDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	MyDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyCharacterAttributeSetBase, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyCharacterAttributeSetBase, Armor, Target, false);
	}
};

UMyDamageExecutionCalculation::UMyDamageExecutionCalculation()
{
}

void UMyDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
}
