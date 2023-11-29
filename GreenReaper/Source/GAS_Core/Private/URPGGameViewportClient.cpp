// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGGameViewportClient.h"
#include "RPGGameInstanceBase.h"
#include "Engine/GameEngine.h"

URPGGameViewportClient& URPGGameViewportClient::Get()
{
	URPGGameViewportClient* GameViewportClient = Cast<URPGGameViewportClient>(GEngine->GameViewport);

	if(GameViewportClient)
	{
		return *GameViewportClient;
	}
	else
	{
		return *NewObject<URPGGameViewportClient>();
	}
}

void URPGGameViewportClient::CheckIfForegroundWindow()
{
	
}

void URPGGameViewportClient::LostFocus(FViewport* VP)
{
	Super::LostFocus(Viewport);
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Lost Focus!"));	
	//
	//URPGGameInstanceBase* RPGGameInstance = Cast<URPGGameInstanceBase>(UGameplayStatics::GetGameInstance(this));
	//RPGGameInstance->OnLostFocus();
	//
	//OnLostFocus.Broadcast();
}

void URPGGameViewportClient::ReceivedFocus(FViewport* VP)
{
	Super::ReceivedFocus(Viewport);
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Recieved Focus!"));	
	//
	//URPGGameInstanceBase* RPGGameInstance = Cast<URPGGameInstanceBase>(UGameplayStatics::GetGameInstance(this));
	//RPGGameInstance->OnReceivedFocus();
	//OnGainedFocus.Broadcast();
}

void URPGGameViewportClient::MinimizeWindow()
{
	UGameEngine* gameEngine = Cast<UGameEngine>(GEngine);

	if (gameEngine)
	{
		TSharedPtr<SWindow> windowPtr = gameEngine->GameViewportWindow.Pin();
		SWindow *window = windowPtr.Get();

		if (window)
		{
			window->Minimize();
		}
	}
}