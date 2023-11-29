// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SolviWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "SolviWidgetFactory_Class.generated.h"

class UObject;
class UUserWidget;

UCLASS()
class SOLVI_API USolviWidgetFactory_Class : public USolviWidgetFactory
{
	GENERATED_BODY()

public:
	USolviWidgetFactory_Class() { }

	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};
