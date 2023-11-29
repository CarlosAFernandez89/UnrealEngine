// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "SaveGame_Level.generated.h"

USTRUCT(BlueprintType)
struct FContainerStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FSlotStruct> Content;

	UPROPERTY()
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FItemSaveStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftClassPtr<AActor> Actor;

	UPROPERTY()
	FTransform Transform;
};


UCLASS()
class SOLVI_API USaveGame_Level : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TSoftObjectPtr<AActor>> ActorsRemoved;

	UPROPERTY()
	TArray<FItemSaveStruct> ActorsAdded;

	UPROPERTY()
	TMap<TSoftClassPtr<UInventorySystemComponent>,FContainerStruct> ContainersAdded;
};
