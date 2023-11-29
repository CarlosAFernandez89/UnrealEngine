// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBase.h"

#include "QuestLogComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AQuestBase::AQuestBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AQuestBase::OnObjectiveIDHeard(const FString& ObjectiveID, const int32 Value)
{
	if(int32* num =  CurrentObjectiveProgress.Find(ObjectiveID))
	{
		FQuestObjectiveDetailsStruct QuestObjectiveDetailsStruct = GetObjectiveDataByID(ObjectiveID);

		int32 NewValue = Value + *num;
		if(NewValue <= QuestObjectiveDetailsStruct.Quantity)
		{
			CurrentObjectiveProgress.Add(ObjectiveID, NewValue);

			IsObjectiveComplete(ObjectiveID);

			OnObjectiveHeardDelegate.Broadcast();
		}
		else
		{
			CurrentObjectiveProgress.Add(ObjectiveID, UKismetMathLibrary::Clamp(NewValue, 0, QuestObjectiveDetailsStruct.Quantity));
			OnObjectiveHeardDelegate.Broadcast();
		}
	}
}

void AQuestBase::GetQuestDetails()
{
	QuestDetails = GetDataTableRow_ItemData(QuestID);

	CurrentStageDetails = QuestDetails.Stages[CurrentStage];
	
	CurrentObjectiveProgress.Empty();

	for (FQuestObjectiveDetailsStruct Element : CurrentStageDetails.Objectives)
	{
		CurrentObjectiveProgress.Add(Element.ObjectiveID);
	}
}

FQuestObjectiveDetailsStruct AQuestBase::GetObjectiveDataByID(const FString& ObjectiveID)
{
	for (FQuestObjectiveDetailsStruct Element : CurrentStageDetails.Objectives)
	{
		if(Element.ObjectiveID == ObjectiveID)
		{
			return Element;
		}
	}

	return FQuestObjectiveDetailsStruct();
}

void AQuestBase::IsObjectiveComplete(const FString& ObjectiveID)
{
	const FQuestObjectiveDetailsStruct ObjectiveDetailsStruct = GetObjectiveDataByID(ObjectiveID);
	
	if(ObjectiveDetailsStruct.Quantity >= *CurrentObjectiveProgress.Find(ObjectiveID))
	{
		CreateWidget(GetWorld(), Widget_QuestNotification->GetClass())->AddToViewport();
	}

	if(AreAllObjectivesComplete())
	{
		++CurrentStage;
		
		if(QuestDetails.Stages.Num() < CurrentStage)
		{
			GetQuestDetails();
		}
		else
		{
			bIsComplete = true;

			if(QuestDetails.bAutoComplete)
			{
				if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
				{
					if(UQuestLogComponent* QuestLogComponent =  PlayerCharacter->QuestLogComponent)
					{
						QuestLogComponent->TurnInQuest(QuestID);
					}
				}
			}
		}
	}
}

bool AQuestBase::AreAllObjectivesComplete()
{
	for (FQuestStageDetailsStruct Element : QuestDetails.Stages)
	{
		for (FQuestObjectiveDetailsStruct Element2 : Element.Objectives)
		{
			FQuestObjectiveDetailsStruct Temp = GetObjectiveDataByID(Element2.ObjectiveID);
			if(int32* number = CurrentObjectiveProgress.Find(Element2.ObjectiveID))
			{
				if(Temp.Quantity < *number)
				{
					return false;
				}
			}

		}
	}
	return true;
}


// Called when the game starts or when spawned
void AQuestBase::BeginPlay()
{
	Super::BeginPlay();
	GetQuestDetails();

	if(ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(UGameplayStatics::GetPlayerCharacter(this,0)))
	{
		PlayerCharacter->OnObjectiveIDCalled.AddUniqueDynamic(this, &AQuestBase::OnObjectiveIDHeard);
	}

	 bIsComplete = AreAllObjectivesComplete();
	
}

// Function to get a row from the DataTable
FQuestDetailsStruct AQuestBase::GetDataTableRow_ItemData(const FName RowName)
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