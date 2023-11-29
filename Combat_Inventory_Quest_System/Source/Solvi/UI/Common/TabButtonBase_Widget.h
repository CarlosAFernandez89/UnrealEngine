// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TabListWidgetBase_Widget.h"
#include "Solvi/UI/Foundation/ButtonBase_Widget.h"

#include "TabButtonBase_Widget.generated.h"

class UCommonLazyImage;
class UObject;
struct FFrame;
struct FSlateBrush;

UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class SOLVI_API UTabButtonBase_Widget : public UButtonBase_Widget, public ISolviTabButtonInterface
{
	GENERATED_BODY()

public:

	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FSolviTabDescriptor& TabLabelInfo) override;

private:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
