// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USolviGameInstance::Init()
{
	Super::Init();

	Init_Inventory();
	
}

void USolviGameInstance::Init_Inventory()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName_Inventory, 0))
	{
		SaveGame_Inventory = Cast<USaveGame_Inventory>(UGameplayStatics::LoadGameFromSlot(SlotName_Inventory, 0));
		UGameplayStatics::SaveGameToSlot(SaveGame_Inventory, SlotName_Inventory, 0);
	}
	else
	{
		SaveGame_Inventory = Cast<USaveGame_Inventory>(UGameplayStatics::CreateSaveGameObject(SaveGame_Inventory->GetClass()));
		UGameplayStatics::SaveGameToSlot(SaveGame_Inventory, SlotName_Inventory, 0);
	}
}
