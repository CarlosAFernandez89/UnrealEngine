// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryActionMenu_Widget.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API UInventoryActionMenu_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	

	UPROPERTY(BlueprintReadWrite)
	int32 Index;
};
