// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGameModeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class SOLVI_API IGameModeInterface
{
	GENERATED_BODY()

public:
	virtual void TryRespawnPawn(APawn* Pawn, FTransform SpawnTransform) = 0;
};
