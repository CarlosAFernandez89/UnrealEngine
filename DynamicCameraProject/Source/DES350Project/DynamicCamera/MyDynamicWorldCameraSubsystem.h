// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyDynamicWorldCameraSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DES350PROJECT_API UMyDynamicWorldCameraSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void BeginDestroy() override;
};
