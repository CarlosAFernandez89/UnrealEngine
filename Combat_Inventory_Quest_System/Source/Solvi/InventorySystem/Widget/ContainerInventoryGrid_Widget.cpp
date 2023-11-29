// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerInventoryGrid_Widget.h"

#include "InventorySlot_Widget.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"

void UContainerInventoryGrid_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UContainerInventoryGrid_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	PopulateInventory();
}

void UContainerInventoryGrid_Widget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerInventoryGrid_Widget::DisplayInventory(UInventorySystemComponent* InInventorySystemComponent)
{
	InventorySystemComponent = InInventorySystemComponent;
	WB_Grid->ClearChildren();
	PopulateInventory();
}

void UContainerInventoryGrid_Widget::PopulateInventory()
{
	if(InventorySystemComponent)
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(this,0);
		for (int i = 0; i< InventorySystemComponent->Content.Num(); ++i)
		{
			if(UInventorySlot_Widget* Widget =  CreateWidget<UInventorySlot_Widget>(Controller, InventorySlot_Widget->GetClass()))
			{
				Widget->ItemID = InventorySystemComponent->Content[i].ItemID;
				Widget->Quantity = InventorySystemComponent->Content[i].Quantity;
				Widget->InventorySystemComponent = InventorySystemComponent;

				WB_Grid->AddChild(Widget);
			}
		}
		InventorySystemComponent->OnInventoryUpdated.AddUniqueDynamic(this, &UContainerInventoryGrid_Widget::UpdateInventory);
	}
}

void UContainerInventoryGrid_Widget::UpdateInventory()
{
	WB_Grid->ClearChildren();
	PopulateInventory();
}
