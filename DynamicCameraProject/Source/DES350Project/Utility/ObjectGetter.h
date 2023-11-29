// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectGetter.generated.h"

class AMyHeroCharacter;
class AMyPlayerCameraManager;
class AMyPlayerController;
class AMyCharacter;
/**
 * 
 */
UCLASS()
class DES350PROJECT_API UObjectGetter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * Getter function to get AMyPlayerController.
	 * NULL check is required.
	 */
	UFUNCTION(BlueprintPure,meta = (WorldContext = "WorldContext"))
	static AMyPlayerController* GetMyPlayerController(const UObject* MyWorldContext, int PlayerIndex = 0);

	/*
	* Getter function to get AMyCharacter.
	* NULL check is required.
	*/
	UFUNCTION(BlueprintPure,meta = (WorldContext = "WorldContext"))
	static AMyHeroCharacter* GetMyPlayerCharacter(const UObject* MyWorldContext, int PlayerIndex = 0);

	UFUNCTION(BlueprintPure, meta=(WorldContext = "WorldContext"))
	static AMyPlayerCameraManager* GetMyPlayerCameraManager(const UObject* MyWorldContext, int PlayerIndex = 0);
};
