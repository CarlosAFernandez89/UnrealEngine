// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ContainerInventory_Widget.generated.h"

class UPlayerInventoryGrid_Widget;
class UContainerInventoryGrid_Widget;
class UInventorySystemComponent;
/**
 * 
 */
UCLASS()
class SOLVI_API UContainerInventory_Widget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	UInventorySystemComponent* PlayerInventory;
	
	UPROPERTY(BlueprintReadWrite)
	UInventorySystemComponent* ContainerInventory;

	UPROPERTY(BlueprintReadWrite)
	UContainerInventoryGrid_Widget* ContainerInventoryGrid;

	UPROPERTY(BlueprintReadWrite)
	UPlayerInventoryGrid_Widget* PlayerInventoryGrid;
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
