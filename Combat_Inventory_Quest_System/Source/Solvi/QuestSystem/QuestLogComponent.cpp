// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLogComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/QuestTracker_Widget.h"


// Sets default values for this component's properties
UQuestLogComponent::UQuestLogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AQuestBase* UQuestLogComponent::AddNewQuest(const FName QuestID)
{
	if(CanPlayerReceiveQuest(QuestID))
	{
		if(!CurrentActiveQuests.Contains(QuestID))
		{
			CurrentActiveQuests.Add(QuestID);
		}

		if(UWorld* const World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const TSubclassOf<AActor> ActorClassToSpawn = AQuestBase::StaticClass();
			const FVector SpawnLocation = FVector();
			const FRotator SpawnRotation = FRotator();

			if(AQuestBase* SpawnedActor = World->SpawnActor<AQuestBase>(ActorClassToSpawn, SpawnLocation, SpawnRotation, SpawnParameters))
			{
				SpawnedActor->QuestID = QuestID;
				if(!CurrentQuests.Contains(SpawnedActor))
				{
					CurrentQuests.Add(SpawnedActor);
				}
				return SpawnedActor;
			}
		}
	}

	return nullptr;
}

void UQuestLogComponent::CompleteQuest(const FName QuestID)
{
	if(!CompletedQuests.Contains(QuestID))
	{
		CompletedQuests.Add(QuestID);
	}

	CurrentActiveQuests.Remove(QuestID);

	if(AQuestBase* QuestActor = GetQuestActor(QuestID))
	{
		QuestActor->bIsTurnedIn = true;
		OnQuestCompletedDelegate.Broadcast(QuestActor);
	}
}

bool UQuestLogComponent::QueryActiveQuests(const FName QuestID) const
{
	return CurrentActiveQuests.Contains(QuestID);
}

void UQuestLogComponent::TrackQuest(AQuestBase* QuestActor)
{
	if(Tracker)
	{
		Tracker->UpdateQuestTracker(QuestActor);
		Tracker->AddToViewport(-1);
	}
	else
	{
		if(UQuestTracker_Widget* MyWidget = CreateWidget<UQuestTracker_Widget>(GetWorld(), UQuestTracker_Widget::StaticClass()))
		{
			MyWidget->QuestActor = QuestActor;
			Tracker = MyWidget;
			MyWidget->AddToViewport(-1);
		}
	}
}

AQuestBase* UQuestLogComponent::GetQuestActor(const FName QuestID)
{
	for (AQuestBase* Element : CurrentQuests)
	{
		if(Element->QuestID == QuestID)
		{
			return Element;
		}
	}
	return nullptr;
}

void UQuestLogComponent::TurnInQuest(const FName QuestID)
{
	if(const AQuestBase* QuestActor = GetQuestActor(QuestID))
	{
		GiveXPReward(QuestActor->CurrentStageDetails.XP_Reward);
		GiveItemReward(QuestActor->CurrentStageDetails.ItemRewards);
		GiveCurrencyRewards(QuestActor->CurrentStageDetails.Rewards);
		//TODO: GiveGameplayEffectReward(QuestActor->CurrentStageDetails.GameplayEffect);

		CompleteQuest(QuestID);
	}
}

bool UQuestLogComponent::QueryCompletedQuest(const FName QuestID, bool& bTurnedIn)
{
	if(const AQuestBase* QuestActor = GetQuestActor(QuestID))
	{
		bTurnedIn = QuestActor->bIsTurnedIn;
		return CompletedQuests.Contains(QuestID);
	}
	else
	{
		bTurnedIn = true;
		return CompletedQuests.Contains(QuestID);
	}
	
}

bool UQuestLogComponent::CanPlayerReceiveQuest(const FName QuestID) const
{
	return !(CurrentActiveQuests.Contains(QuestID) || CompletedQuests.Contains(QuestID));
}

void UQuestLogComponent::GiveXPReward(int32 XP_Reward)
{
	
}

void UQuestLogComponent::GiveItemReward(TMap<FName, int32> ItemRewards) const
{
	TArray<FName> OutKeys;
	ItemRewards.GetKeys(OutKeys);
	for (FName OutKey : OutKeys)
	{
		if(const int32* Quantity =  ItemRewards.Find(OutKey))
		{
			if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
			{
				int32 QuantityRemaining;
				if(!PlayerCharacter->InventorySystemComponent->AddToInventory(OutKey, *Quantity, QuantityRemaining))
				{
					//Failed to add to inventory.
				}
			}
		}
		else
		{
			// Couldn't find OutKey in itemRewardMap
		}
	}
}

void UQuestLogComponent::GiveCurrencyRewards(const FCurrencyStruct CurrencyRewards) const
{
	if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		PlayerCharacter->InventorySystemComponent->Currency.Gold += CurrencyRewards.Gold;
		//TODO: Add the other currency rewards here
		
		PlayerCharacter->InventorySystemComponent->SaveInventory();
	}

}

void UQuestLogComponent::GiveGameplayEffectReward(UGameplayEffect* GameplayEffect)
{
	//TODO: DO this
}
