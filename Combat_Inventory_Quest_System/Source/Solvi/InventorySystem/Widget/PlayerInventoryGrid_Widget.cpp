// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryGrid_Widget.h"

#include "InventorySlot_Widget.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"

void UPlayerInventoryGrid_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UPlayerInventoryGrid_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	PopulateInventory();

}

void UPlayerInventoryGrid_Widget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerInventoryGrid_Widget::DisplayInventory(UInventorySystemComponent* InInventorySystemComponent)
{
	InventorySystemComponent = InInventorySystemComponent;
	WB_Grid->ClearChildren();
	PopulateInventory();
}

void UPlayerInventoryGrid_Widget::PopulateInventory()
{
	if(InventorySystemComponent)
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(this,0);
		for (int i = 0; i< InventorySystemComponent->Content.Num(); ++i)
		{
			if(UInventorySlot_Widget* Widget =  CreateWidget<UInventorySlot_Widget>(Controller, InventorySlot_Widget))
			{
				Widget->ItemID = InventorySystemComponent->Content[i].ItemID;
				Widget->Quantity = InventorySystemComponent->Content[i].Quantity;
				Widget->InventorySystemComponent = InventorySystemComponent;

				WB_Grid->AddChild(Widget);
			}
		}
		InventorySystemComponent->OnInventoryUpdated.AddUniqueDynamic(this, &UPlayerInventoryGrid_Widget::UpdateInventory);
	}
}

void UPlayerInventoryGrid_Widget::UpdateInventory()
{
	WB_Grid->ClearChildren();
	PopulateInventory();
}
