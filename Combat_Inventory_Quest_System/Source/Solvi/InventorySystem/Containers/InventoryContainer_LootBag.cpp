// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryContainer_LootBag.h"


// Sets default values
AInventoryContainer_LootBag::AInventoryContainer_LootBag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInventoryContainer_LootBag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryContainer_LootBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

