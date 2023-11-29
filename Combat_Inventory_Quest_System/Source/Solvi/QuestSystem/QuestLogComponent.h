// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestBase.h"
#include "Components/ActorComponent.h"
#include "QuestLogComponent.generated.h"

class UQuestTracker_Widget;
class UGameplayEffect;
struct FCurrencyStruct;
class AQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, class AQuestBase*, QuestBase);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLVI_API UQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestLogComponent();

	UFUNCTION()
	AQuestBase* AddNewQuest(FName QuestID);

	UFUNCTION()
	void CompleteQuest(FName QuestID);

	UFUNCTION()
	bool QueryActiveQuests(FName QuestID) const;

	UFUNCTION()
	void TrackQuest(AQuestBase* QuestActor);

	UFUNCTION()
	AQuestBase* GetQuestActor(FName QuestID);

	UFUNCTION()
	void TurnInQuest(FName QuestID);

	UFUNCTION()
	bool QueryCompletedQuest(FName QuestID, bool& bTurnedIn);

	UFUNCTION()
	bool CanPlayerReceiveQuest(FName QuestID) const;

	UFUNCTION()
	void GiveXPReward(int32 XP_Reward);

	UFUNCTION()
	void GiveItemReward(TMap<FName, int32> ItemRewards) const;

	UFUNCTION()
	void GiveCurrencyRewards(FCurrencyStruct CurrencyRewards) const;

	UFUNCTION()
	void GiveGameplayEffectReward(UGameplayEffect* GameplayEffect);


protected:


public:

	UPROPERTY()
	TArray<FName> CurrentActiveQuests;

	UPROPERTY()
	TArray<FName> CompletedQuests;

	UPROPERTY()
	TArray<FName> CurrentTrackedQuests;

	UPROPERTY()
	TArray<AQuestBase*> CurrentQuests;

	UPROPERTY()
	UQuestTracker_Widget* Tracker;

	UPROPERTY(BlueprintAssignable)
	FOnQuestCompleted OnQuestCompletedDelegate;

};
