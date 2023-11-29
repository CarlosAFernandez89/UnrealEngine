// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SolviWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> USolviWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
