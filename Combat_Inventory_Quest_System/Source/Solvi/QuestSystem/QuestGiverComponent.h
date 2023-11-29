// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IQuestSystemInteract.h"
#include "QuestBase.h"
#include "Components/ActorComponent.h"
#include "QuestGiverComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLVI_API UQuestGiverComponent : public UActorComponent, public IQuestSystemInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestGiverComponent();

protected:

private:
	static FQuestDetailsStruct GetDataTableRow_QuestDetails(FName RowName);

public:
	// IQuestSystemInteract
	virtual void LookAt() const override;
	virtual FString InteractWith() override;
	// ~IQuestSystemInteract


	UFUNCTION()
	void DisplayQuest() const;

	UFUNCTION()
	void DisplayRewards() const;

	UFUNCTION()
	bool ArePrerequisiteQuestsDone(const FQuestDetailsStruct& QuestDetails) const;

	UPROPERTY(BlueprintReadWrite)
	FDataTableRowHandle QuestData;
};
