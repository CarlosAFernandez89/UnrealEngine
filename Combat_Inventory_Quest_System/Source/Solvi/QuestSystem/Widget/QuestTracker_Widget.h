// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "QuestTracker_Widget.generated.h"

class AQuestBase;

/**
 * 
 */
UCLASS()
class SOLVI_API UQuestTracker_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void UpdateQuestTracker(AQuestBase* QuestActor);

	UPROPERTY()
	AQuestBase* QuestActor;
};
