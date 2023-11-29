// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCameraManager.h"

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "DES350Project/Utility/ObjectGetter.h"
#include "Hero/MyHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AMyPlayerCameraManager::AMyPlayerCameraManager()
{

}

void AMyPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	WorldTimerManager = &GetWorldTimerManager();
	if(WorldTimerManager)
	{
		WorldTimerManager->SetTimer(FOV_TimerHandle, this, &AMyPlayerCameraManager::InterpolateCameraFOV, FApp::GetDeltaTime(), true);
	}
	WorldTimerManager->PauseTimer(FOV_TimerHandle);
}

void AMyPlayerCameraManager::SprintCameraState(float FieldOfView)
{
	WantedFOV = FieldOfView;
	WorldTimerManager->UnPauseTimer(FOV_TimerHandle);
}

void AMyPlayerCameraManager::InterpolateCameraFOV()
{
	AMyHeroCharacter* PlayerCharacter = UObjectGetter::GetMyPlayerCharacter(this);
	
	PlayerCharacter->GetCameraComponent()->FieldOfView =
		UKismetMathLibrary::FInterpTo(
			PlayerCharacter->GetCameraComponent()->FieldOfView,
			WantedFOV,
			FApp::GetDeltaTime(),
			3.f
			);

	if(UKismetMathLibrary::NearlyEqual_FloatFloat(PlayerCharacter->GetCameraComponent()->FieldOfView, WantedFOV))
	{
		WorldTimerManager->PauseTimer(FOV_TimerHandle);
	}
}
