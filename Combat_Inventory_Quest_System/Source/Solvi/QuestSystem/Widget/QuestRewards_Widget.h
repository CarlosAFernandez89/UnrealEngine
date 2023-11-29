// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Solvi/QuestSystem/QuestBase.h"
#include "QuestRewards_Widget.generated.h"

/**
 * 
 */
UCLASS()
class SOLVI_API UQuestRewards_Widget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	FQuestDetailsStruct QuestDetails;
	
	UPROPERTY()
	FName QuestID;
};
