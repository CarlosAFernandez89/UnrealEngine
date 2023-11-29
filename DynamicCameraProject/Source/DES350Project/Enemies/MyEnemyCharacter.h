// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DES350Project/Player/MyCharacter.h"
#include "GameplayEffectTypes.h"
#include "DES350Project/Player/HUD/MyFloatingStatusBarWidget.h"
#include "MyEnemyCharacter.generated.h"

UCLASS()
class DES350PROJECT_API AMyEnemyCharacter : public AMyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemyCharacter();

public:
	AMyEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	class UMyAbilitySystemComponent* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY()
	class UMyCharacterAttributeSetBase* HardRefAttributeSetBase;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	UMyFloatingStatusBarWidget* UIFloatingStatusBar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GAS|UI")
	class UWidgetComponent* UIFloatingStatusBarComponent;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
