// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGGameModeBase.h"
#include "RPGGameStateBase.h"
#include "RPGPlayerControllerBase.h"

ARPGGameModeBase::ARPGGameModeBase()
{
	GameStateClass = ARPGGameStateBase::StaticClass();
	PlayerControllerClass = ARPGPlayerControllerBase::StaticClass();
	bGameOver = false;
}

void ARPGGameModeBase::ResetLevel() 
{
	K2_DoRestart();
}

bool ARPGGameModeBase::HasMatchEnded() const 
{
	return bGameOver;
}

void ARPGGameModeBase::GameOver()
{
	if (bGameOver == false)
	{
		K2_OnGameOver();
		bGameOver = true;
	}
}

#include "Framework/Application/NavigationConfig.h"

void ARPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetWidgetNavigation(false, false, false);
}

void ARPGGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SetWidgetNavigation(true, true, true);
}

void ARPGGameModeBase::SetWidgetNavigation(bool AllowAnalog, bool AllowKey, bool AllowTab)
{
	{
		FSlateApplication& SlateApplication = FSlateApplication::Get();
		FNavigationConfig& NavigationConfig = *SlateApplication.GetNavigationConfig();

		NavigationConfig.bAnalogNavigation = AllowAnalog;
		NavigationConfig.bKeyNavigation = AllowKey;
		NavigationConfig.bTabNavigation = AllowTab;
	}
}
