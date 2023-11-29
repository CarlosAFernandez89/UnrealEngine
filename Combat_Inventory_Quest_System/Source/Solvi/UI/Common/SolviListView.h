// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonListView.h"

#include "SolviListView.generated.h"

class UUserWidget;
class ULocalPlayer;
class USolviWidgetFactory;

UCLASS(meta = (DisableNativeTick))
class SOLVI_API USolviListView : public UCommonListView
{
	GENERATED_BODY()

public:
	USolviListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

	protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<USolviWidgetFactory>> FactoryRules;
};
