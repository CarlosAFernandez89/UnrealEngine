// Fill out your copyright notice in the Description page of Project Settings.


#include "TabButtonBase_Widget.h"

#include "CommonLazyImage.h"
#include "Solvi/UI/Common/TabListWidgetBase_Widget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TabButtonBase_Widget)

class UObject;
struct FSlateBrush;

void UTabButtonBase_Widget::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UTabButtonBase_Widget::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UTabButtonBase_Widget::SetTabLabelInfo_Implementation(const FSolviTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}
