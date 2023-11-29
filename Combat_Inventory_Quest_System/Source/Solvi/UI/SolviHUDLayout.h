// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SolviActivatableWidget.h"

#include "SolviHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Solvi HUD Layout", Category = "Solvi|HUD"))
class SOLVI_API USolviHUDLayout : public USolviActivatableWidget
{
	GENERATED_BODY()

public:

	USolviHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
