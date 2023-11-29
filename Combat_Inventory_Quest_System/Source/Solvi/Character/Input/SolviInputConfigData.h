// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "SolviInputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API USolviInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMove;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ItemInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* OpenInventory;
};
