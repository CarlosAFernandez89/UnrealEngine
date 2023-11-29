// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectGetter.h"

#include "DES350Project/Player/MyCharacter.h"
#include "DES350Project/Player/MyPlayerCameraManager.h"
#include "DES350Project/Player/MyPlayerController.h"
#include "DES350Project/Player/Hero/MyHeroCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController* UObjectGetter::GetMyPlayerController(const UObject* MyWorldContext, int PlayerIndex)
{
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MyWorldContext, PlayerIndex))
	{
		if(AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			return MyPlayerController;
		}
	}
	return nullptr;
}

AMyHeroCharacter* UObjectGetter::GetMyPlayerCharacter(const UObject* MyWorldContext, int PlayerIndex)
{
	if(ACharacter* Character = UGameplayStatics::GetPlayerCharacter(MyWorldContext,PlayerIndex))
	{
		if(AMyHeroCharacter* MyCharacter = Cast<AMyHeroCharacter>(Character))
		{
			return MyCharacter;
		}
	}
	return nullptr;
}

AMyPlayerCameraManager* UObjectGetter::GetMyPlayerCameraManager(const UObject* MyWorldContext, int PlayerIndex)
{
	if(APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(MyWorldContext, PlayerIndex))
	{
		if(AMyPlayerCameraManager* MyPlayerCameraManager = Cast<AMyPlayerCameraManager>(PlayerCameraManager))
		{
			return MyPlayerCameraManager;
		}
	}
	return nullptr;
}
