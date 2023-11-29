// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerInventory_Widget.h"

#include "ContainerInventoryGrid_Widget.h"
#include "PlayerInventoryGrid_Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Solvi/Character/SolviModularCharacter.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"

void UContainerInventory_Widget::NativePreConstruct()
{	
	Super::NativePreConstruct();

	if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		if(UInventorySystemComponent* ISC = Cast<UInventorySystemComponent>(PlayerCharacter->GetComponentByClass(UInventorySystemComponent::StaticClass())))
		{
			PlayerInventory = ISC;
			PlayerInventoryGrid->DisplayInventory(PlayerInventory);
			ContainerInventoryGrid->DisplayInventory(ContainerInventory);
		}
	}
}

void UContainerInventory_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UContainerInventory_Widget::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0))
	{
		const FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}
