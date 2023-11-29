// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveData/SaveGame_Inventory.h"
#include "SolviGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API USolviGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

	void Init_Inventory();

public:
	UPROPERTY()
	FString SlotName_Inventory;

	UPROPERTY()
	USaveGame_Inventory* SaveGame_Inventory;
};
