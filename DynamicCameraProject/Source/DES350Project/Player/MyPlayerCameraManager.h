// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DES350PROJECT_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	AMyPlayerCameraManager();

	virtual void BeginPlay() override;

	FTimerManager* WorldTimerManager;

public:
	UFUNCTION()
	void SprintCameraState(float FieldOfView);
	UPROPERTY()
	float WantedFOV;

	UFUNCTION()
	void InterpolateCameraFOV();
	FTimerHandle FOV_TimerHandle;
	
};
