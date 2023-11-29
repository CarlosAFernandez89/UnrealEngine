// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonActivatableWidget.h"

#include "SolviControllerDisconnectedScreen.generated.h"


class UHorizontalBox;
class UObject;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SOLVI_API USolviControllerDisconnectedScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	void NativeOnActivated() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HBox_SwitchUser;
};
