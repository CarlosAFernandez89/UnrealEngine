// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "UObject/Object.h"

#include "SolviWidgetFactory.generated.h"

template <class TClass> class TSubclassOf;

class UUserWidget;
struct FFrame;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class SOLVI_API USolviWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	USolviWidgetFactory() { }

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};
