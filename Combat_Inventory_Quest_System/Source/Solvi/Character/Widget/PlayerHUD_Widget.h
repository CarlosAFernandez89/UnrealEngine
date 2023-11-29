// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Solvi/InventorySystem/Widget/ContainerInventory_Widget.h"
#include "PlayerHUD_Widget.generated.h"

class UInventorySystemComponent;
/**
 * 
 */
UCLASS()
class SOLVI_API UPlayerHUD_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void ShowContainerInventory(UInventorySystemComponent* ContainerInventory);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCommonUserWidget* MenuWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UContainerInventory_Widget>  ContainerInventory_Widget;
};
