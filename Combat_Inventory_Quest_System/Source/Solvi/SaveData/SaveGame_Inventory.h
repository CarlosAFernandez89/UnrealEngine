// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "SaveGame_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API USaveGame_Inventory : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FSlotStruct> InventoryContent;
	
	UPROPERTY()
	FCurrencyStruct InventoryCurrency;
};
