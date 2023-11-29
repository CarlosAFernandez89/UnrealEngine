// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SolviLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// USolviLoadingScreenSubsystem

USolviLoadingScreenSubsystem::USolviLoadingScreenSubsystem()
{
}

void USolviLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> USolviLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}

