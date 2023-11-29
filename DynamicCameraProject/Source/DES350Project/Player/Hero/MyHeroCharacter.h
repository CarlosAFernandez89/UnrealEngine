// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DES350Project/GameplayAbility/Abilities/MyAbilitySet.h"
#include "DES350Project/Player/MyCharacter.h"
#include "MyHeroCharacter.generated.h"

class UMyInputConfig;

UCLASS()
class DES350PROJECT_API AMyHeroCharacter : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMyHeroCharacter();
	AMyHeroCharacter(const class FObjectInitializer& ObjectInitializer);

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Abilities")
	UMyAbilitySet* AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	UMyInputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	class UInputMappingContext* DefaultMappingContext;

	virtual void FinishDying() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere ,Category = "Camera")
	class UMyCharacterCameraManager* MyCharacterCameraManager;
	
	FORCEINLINE virtual UMyCharacterCameraManager* GetCameraManagerComponent() const { return MyCharacterCameraManager; }

		
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;
	
	FORCEINLINE virtual USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	FORCEINLINE virtual UCameraComponent* GetCameraComponent() const { return CameraComponent; }
};
