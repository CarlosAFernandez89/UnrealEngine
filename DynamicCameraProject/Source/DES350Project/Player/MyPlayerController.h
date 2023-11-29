// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

class UInputAction;
struct FInputActionValue;
class AMyHeroCharacter;

UCLASS()
class DES350PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	FTimerManager* WorldTimerManager;

public:
	// Sets default values for this actor's properties
	AMyPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(Blueprintable)
	void ShowHUD();

	UFUNCTION(Blueprintable)
	void HideHUD();

	// Handle move input
	void Move(const FInputActionValue& Value);
	FRotator YawRotation;
 
	// Handle look input
	void Look(const FInputActionValue& Value);

	// Handle Spring
	void Sprint_Start(const FInputActionValue& Value);
	void Sprint_End(const FInputActionValue& Value);
	FTimerHandle SprintCameraLogic_TimerHandle;
	void SprintCameraLogic();

	UPROPERTY()
	float SprintSpeed;
	UPROPERTY()
	float DefaultWalkSpeed;

	// Handle Movement mode
	void ToggleMovementMode(const FInputActionValue& Value);

	UPROPERTY()
	bool bFreeCamera;

	UPROPERTY()
	bool bIsUsingWorldCamera;
	UFUNCTION()
	void ToggleWorldCameraSettings();
	
	UPROPERTY()
	FTimerHandle MovementDirection_TimerHandle;
	UFUNCTION()
	void InterpolateMovementDirections();
	UPROPERTY()
	FRotator StartRotator;
	UPROPERTY()
	FRotator TargetRotator;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPlayerMovingMouse();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FInputActionValue GetInputActionValue(const UInputAction* ForAction);

	UPROPERTY()
	AMyHeroCharacter* PlayerCharacter;

	// Handle SpringArm Target Arm Length Movement
	void CameraZoom(const FInputActionValue& Value);
	void CameraZoomLerp();
	
	UPROPERTY()
	FTimerHandle CameraZoomTimerHandle;
	
	UPROPERTY()
	float WantedTargetArmLength;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Camera|SpringArm")
	float TargetArmLengthAlpha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Camera|SpringArm")
	float MinTargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Camera|SpringArm")
	float MaxTargetArmLength;


	UPROPERTY(EditAnywhere, Category = "GAS|UI")
	TSubclassOf<class UMyDamageTextWidgetComponent> DamageNumberClass;

	class AMyHUD* GetHUD();

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AMyCharacter* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, AMyCharacter* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, AMyCharacter* TargetCharacter);

	// Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the Server.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS|UI")
	TSubclassOf<class AMyHUD> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "GAS|UI")
	class AMyHUD* UIHUDWidget;
	
};
