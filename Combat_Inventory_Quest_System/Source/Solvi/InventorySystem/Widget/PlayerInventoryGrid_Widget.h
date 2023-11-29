// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "PlayerInventoryGrid_Widget.generated.h"

class UInventorySlot_Widget;
class UWrapBox;
/**
 * 
 */
UCLASS()
class SOLVI_API UPlayerInventoryGrid_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UInventorySlot_Widget> InventorySlot_Widget;

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(UInventorySystemComponent* InventorySystemComponent);

	UFUNCTION(BlueprintCallable)
	void PopulateInventory();

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
	
	UPROPERTY(BlueprintReadWrite)
	UInventorySystemComponent* InventorySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	UWrapBox* WB_Grid;
};
