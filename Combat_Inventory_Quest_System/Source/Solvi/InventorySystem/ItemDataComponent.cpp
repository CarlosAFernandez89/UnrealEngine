// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataComponent.h"

#include "InventorySystemComponent.h"


// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent(): Quantity(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Inventory/Item_Data")))
	{
		ItemID.DataTable = ItemDataTable;
	}
}


void UItemDataComponent::StopLookAt() const
{
}

void UItemDataComponent::InteractWith(ASolviModularCharacter* PlayerCharacter)
{
	if(UInventorySystemComponent* ISC = PlayerCharacter->InventorySystemComponent)
	{
		int32 QuantityRemaining;
		if(ISC->AddToInventory(ItemID.RowName, Quantity, QuantityRemaining))
		{
			GetOwner()->Destroy();
		}
	}
}

// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetReplicates(true);
	
}

