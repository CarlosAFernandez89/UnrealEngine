// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PrimaryGameLayout.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "Widget/PlayerHUD_Widget.h"
#include "SolviModularCharacter.generated.h"

class UPlayerHUD_Widget;
class UCommonUserWidget;

UENUM()
enum ESwitchTargetDirection
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

class USolviPawnData;
class USolviPawnExtensionComponent;
class USolviCameraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSolviHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveIDCalled, const FString&, ObjectiveID, int32, Value );


UCLASS()
class SOLVI_API ASolviModularCharacter : public AGSCModularCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Solvi|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Solvi|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	
public:
	// Sets default values for this character's properties
	ASolviModularCharacter(const FObjectInitializer& ObjectInitializer);

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle move input
	void Move(const FInputActionValue& Value);
 
	// Handle look input
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solvi|Character", Meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solvi|Character", Meta = (AllowPrivateAccess = "true"))
	class USolviInputConfigData* InputActions;

	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AActor* SwitchTarget(const TArray<AActor*>& Actors,
	ACharacter* Character, AActor* SelectedActor, ESwitchTargetDirection Direction);
	
	UFUNCTION(BlueprintCallable)
	bool CanPlayerMove();
	bool IsLockedOn();
	bool CanPlayerMoveCamera();

	UFUNCTION()
	void IA_ItemInteract();

	UFUNCTION()
	void IA_OpenInventory();
	UPrimaryGameLayout* GetRootUILayout() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenInventory();

	UPROPERTY(BlueprintAssignable)
	FSolviHealth_DeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnObjectiveIDCalled OnObjectiveIDCalled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventorySystemComponent> InventorySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UQuestLogComponent> QuestLogComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "HUD")
	UPlayerHUD_Widget* PlayerHUD;

};
