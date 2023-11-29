// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemPopUp_Widget.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API UItemPopUp_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite)
	FText InteractHoverMessage;

	UPROPERTY(BlueprintReadWrite)
	float FontSize;
};
