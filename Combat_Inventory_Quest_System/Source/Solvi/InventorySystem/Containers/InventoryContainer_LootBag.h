// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainer.h"
#include "InventoryContainer_LootBag.generated.h"

UCLASS()
class SOLVI_API AInventoryContainer_LootBag : public AInventoryContainer
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventoryContainer_LootBag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
