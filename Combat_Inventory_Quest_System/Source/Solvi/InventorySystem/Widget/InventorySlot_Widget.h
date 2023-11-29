// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "InventorySlot_Widget.generated.h"

class UInventorySystemComponent;
class UInventoryActionMenu_Widget;
/**
 * 
 */
UCLASS()
class SOLVI_API UInventorySlot_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventoryActionMenu_Widget* ActionMenu;

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	

	UPROPERTY(BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite)
	int32 ContentIndex;

	UPROPERTY(BlueprintReadWrite)
	UInventorySystemComponent* InventorySystemComponent;
	
};
