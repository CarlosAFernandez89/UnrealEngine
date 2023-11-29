// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiverComponent.h"

#include "QuestBase.h"
#include "QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/QuestGiver_Widget.h"
#include "Widget/QuestRewards_Widget.h"


// Sets default values for this component's properties
UQuestGiverComponent::UQuestGiverComponent()
{
	// Assign the data table to the row handle if it's found
	if (const ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/QuestSystem/Quest_Data.Quest_Data")); DataTableAsset.Succeeded())
	{
		QuestData.DataTable = DataTableAsset.Object;
		// Here you would also assign the RowName if you have a specific row you want to reference
		// QuestData.RowName = FName("YourRowName");
	}
}

void UQuestGiverComponent::LookAt() const
{
	
}

FString UQuestGiverComponent::InteractWith()
{
	if(const ASolviModularCharacter* PC = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		if(UQuestLogComponent* QLC = PC->QuestLogComponent)
		{
			bool bTurnedIn;
			if(!QLC->QueryActiveQuests(QuestData.RowName) && !QLC->QueryCompletedQuest(QuestData.RowName,bTurnedIn))
			{
				const FQuestDetailsStruct QuestDetails = GetDataTableRow_QuestDetails(QuestData.RowName);
				if(QuestDetails.bIsMainQuest)
				{
					QLC->AddNewQuest(QuestData.RowName);

					DisplayQuest();
					return UKismetSystemLibrary::GetDisplayName(GetOwner());
				}

			}
			else
			{
				if(const AQuestBase* QuestBase = QLC->GetQuestActor(QuestData.RowName))
				{
					if(QuestBase->bIsComplete && !QuestBase->bIsTurnedIn)
					{
						DisplayRewards();
						return UKismetSystemLibrary::GetDisplayName(GetOwner());
					}
				}
			}
		}
	}
	return FString();
}

void UQuestGiverComponent::DisplayQuest() const
{
	const FQuestDetailsStruct QuestDetails = GetDataTableRow_QuestDetails(QuestData.RowName);
	if(const ASolviModularCharacter* PC = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		if(UQuestLogComponent* Qlc =  PC->QuestLogComponent)
		{
			for (FName Element : Qlc->CompletedQuests)
			{
				if(Element == QuestData.RowName)
				{
					//QuestAlreadyCompleted.
					return;
				}
			}

			if(!ArePrerequisiteQuestsDone(QuestDetails))
			{
				//Haven't done prereqs
				return;
			}

			if(UQuestGiver_Widget* Widget = CreateWidget<UQuestGiver_Widget>(GetWorld(), UQuestGiver_Widget::StaticClass()))
			{
				Widget->QuestDetails = QuestDetails;
				Widget->QuestID = QuestData.RowName;
				return;
			}
		}
	}
}

void UQuestGiverComponent::DisplayRewards() const
{
	const FQuestDetailsStruct QuestDetails = GetDataTableRow_QuestDetails(QuestData.RowName);
	if(UQuestRewards_Widget* Widget = CreateWidget<UQuestRewards_Widget>(GetWorld(), UQuestRewards_Widget::StaticClass()))
	{
		Widget->QuestDetails = QuestDetails;
		Widget->QuestID = QuestData.RowName;
		Widget->AddToViewport();
	}
}

bool UQuestGiverComponent::ArePrerequisiteQuestsDone(const FQuestDetailsStruct& QuestDetails) const
{
	int32 CompletedPreReqs = QuestDetails.PreRequisiteDays.Num();

	if(const ASolviModularCharacter* PC = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		if(UQuestLogComponent* QLC = PC->QuestLogComponent)
		{
			for (FName Element : QLC->CompletedQuests)
			{
				if(QuestDetails.PreRequisiteDays.Contains(Element))
				{
					++CompletedPreReqs;
				}
			}
		}
	}

	return CompletedPreReqs >= QuestDetails.PreRequisiteDays.Num();
	
}


// Function to get a row from the DataTable
FQuestDetailsStruct UQuestGiverComponent::GetDataTableRow_QuestDetails(const FName RowName)
{
	// Get a reference to the DataTable asset
	static const FString ContextString(TEXT("DataTable Context")); // Provide a context string

	if (const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/QuestSystem/Quest_Data.QuestData'")))
	{
		// Use the FindRow function to retrieve the row by name
		if (FQuestDetailsStruct* Row = DataTable->FindRow<FQuestDetailsStruct>(RowName, ContextString, true))
		{
			return *Row;
		}
		else
		{
			// Handle the case where the row with the specified name was not found
		}
	}
	else
	{
		// Handle the case where the DataTable asset could not be loaded
	}

	// Return a default or invalid row if needed
	return FQuestDetailsStruct();
}
