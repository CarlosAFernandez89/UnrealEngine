// Fill out your copyright notice in the Description page of Project Settings.
#include "MyDynamicWorldCameraSubsystem.h"

#include "DES350Project/Player/MyPlayerCameraManager.h"
#include "../Utility/ObjectGetter.h"



void UMyDynamicWorldCameraSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if(AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this))
	{
		MyPlayerCameraManager->StartCameraFade(1.f,0.f,3.f,FColor::Black);
	}
}

void UMyDynamicWorldCameraSubsystem::BeginDestroy()
{
	//TODO: Add timer so it shows the fade out before exiting the world/level.
	if(AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this))
	{
		MyPlayerCameraManager->StartCameraFade(0.f,1.f,2.f,FColor::Black);
	}
	Super::BeginDestroy();
	
}
