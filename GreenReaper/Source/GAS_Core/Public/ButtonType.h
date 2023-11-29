// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ButtonType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EButtonType : uint8
{
	Basic,
	Back,
	LevelLoad,
	LevelQuit
};
