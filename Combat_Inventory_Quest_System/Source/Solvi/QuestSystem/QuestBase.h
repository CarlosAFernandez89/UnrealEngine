// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "QuestBase.generated.h"

UENUM(BlueprintType)
enum EObjectiveType : uint8
{
	Location UMETA(DisplayName = "Location"),
	Kill UMETA(DisplayName = "Kill"),
	Interact UMETA(DisplayName = "Interact"),
	Collect UMETA(DisplayName = "Collect")
};

USTRUCT(BlueprintType)
struct FQuestObjectiveDetailsStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText ObjectiveName;

	UPROPERTY(BlueprintReadWrite)
	FText Description;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EObjectiveType> ObjectiveType;

	UPROPERTY(BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOptional;
	
};

USTRUCT(BlueprintType)
struct FQuestStageDetailsStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText StageName;

	UPROPERTY(BlueprintReadWrite)
	FText Description;

	UPROPERTY(BlueprintReadWrite)
	TArray<FQuestObjectiveDetailsStruct> Objectives;

	UPROPERTY(BlueprintReadWrite)
	int32 XP_Reward;

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, int32> ItemRewards;

	UPROPERTY(BlueprintReadWrite)
	FCurrencyStruct Rewards;

	//TODO: UPROPERTY(BlueprintReadWrite)
	//TODO: UGameplayEffect GameplayEffectRewards;

	FQuestStageDetailsStruct& operator=(const FQuestStageDetailsStruct& Other)
	{
		if (this != &Other)
		{
			// Copy individual members from Other to this
			this->StageName = Other.StageName;
			this->Description = Other.Description;
			this->Objectives = Other.Objectives;
			this->XP_Reward = Other.XP_Reward;
			this->ItemRewards = Other.ItemRewards;
			this->Rewards = Other.Rewards;
			// TODO: this->GameplayEffectRewards = Other.GameplayEffectRewards;
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FQuestDetailsStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText QuestName;

	UPROPERTY(BlueprintReadWrite)
	FText LogDescription;
	
	UPROPERTY(BlueprintReadWrite)
	FText TrackingDescription;

	UPROPERTY(BlueprintReadWrite)
	bool bIsMainQuest;

	UPROPERTY(BlueprintReadWrite)
	TArray<FQuestStageDetailsStruct> Stages;

	UPROPERTY(BlueprintReadWrite)
	bool bAutoAccept;

	UPROPERTY(BlueprintReadWrite)
	bool bAutoComplete;

	UPROPERTY(BlueprintReadWrite)
	TArray<FName> PreRequisiteDays;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveHeardSignature);


UCLASS()
class SOLVI_API AQuestBase : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AQuestBase();

	UFUNCTION(BlueprintCallable)
	virtual void OnObjectiveIDHeard(const FString& ObjectiveID, int32 Value);

	UFUNCTION()
	void GetQuestDetails();

	UFUNCTION()
	FQuestObjectiveDetailsStruct GetObjectiveDataByID(const FString& ObjectiveID);

	UFUNCTION()
	void IsObjectiveComplete(const FString& ObjectiveID);

	UFUNCTION()
	bool AreAllObjectivesComplete();
	
	
	UPROPERTY(BlueprintAssignable)
	FOnObjectiveHeardSignature OnObjectiveHeardDelegate;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FQuestDetailsStruct GetDataTableRow_ItemData(FName RowName);

public:
	UPROPERTY(BlueprintReadWrite)
	FName QuestID;

	UPROPERTY()
	FQuestDetailsStruct QuestDetails;

	UPROPERTY()
	FQuestStageDetailsStruct CurrentStageDetails;

	UPROPERTY()
	int32 CurrentStage;

	UPROPERTY()
	TMap<FString, int32> CurrentObjectiveProgress;

	UPROPERTY()
	bool bIsComplete;

	UPROPERTY()
	bool bIsTurnedIn;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UUserWidget> Widget_QuestNotification;
	
};
