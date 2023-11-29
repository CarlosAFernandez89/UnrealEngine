// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD_Widget.h"

#include "Solvi/InventorySystem/Widget/ContainerInventory_Widget.h"

void UPlayerHUD_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UPlayerHUD_Widget::ShowContainerInventory(UInventorySystemComponent* InContainerInventory)
{
	if(MenuWidget)
	{
		MenuWidget->RemoveFromParent();
	}

	if(UCommonUserWidget* Widget = CreateWidget<UCommonUserWidget>(this, ContainerInventory_Widget))
	{
		UContainerInventory_Widget* CW = Cast<UContainerInventory_Widget>(Widget);
		if(CW)
		{
			CW->ContainerInventory = InContainerInventory;
			MenuWidget = CW;
			MenuWidget->AddToViewport();
		}

	}
}
