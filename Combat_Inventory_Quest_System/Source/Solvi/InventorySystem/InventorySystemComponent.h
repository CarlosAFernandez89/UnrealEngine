// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IItemInteract.h"
#include "Components/ActorComponent.h"
#include "Solvi/Character/SolviModularCharacter.h"
#include "InventorySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UGameplayEffect;
class UCommonUserWidget;

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name = FText();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description = FText();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Thumbnail = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ItemClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 StackSize = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> ItemEffect;
	
};

USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemID = FName();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity = -1;
};

USTRUCT(BlueprintType)
struct FCurrencyStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Gold = -1;
};

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class SOLVI_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventorySystemComponent();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION()
	void AutoSaveInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OnLocal_Interact(AActor* TargetActor, AActor* Interactor);

	UFUNCTION(Server, Reliable)
	void Server_ItemInteract(AActor* TargetActor);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateInventory();

	UFUNCTION(Server, Reliable)
	void Server_TransferSlots(int32 SourceIndex, UInventorySystemComponent* SourceInventory, int32 DestinationIndex);

	UFUNCTION(Server, Reliable)
	void Server_Remove(int32 Index, bool bRemoveWholeStack, bool bIsConsumed);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(FName ItemID, int32 Quantity);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(FName ItemID);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool AddToInventory(FName ItemID, int32 Quantity, int32& QuantityRemaining);

	UFUNCTION()
	void RemoveFromInventory(int32 Index, bool bRemoveWholeStack, bool bIsConsumed);

	UFUNCTION()
	void InteractTrace(float DeltaTime);

	UFUNCTION()
	bool FindSlot(FName ItemID, int32& Index);

	UFUNCTION()
	int32 GetMaxStackSize(FName ItemID);

	UFUNCTION()
	void AddToStack(int32 Index, int32 Quantity);

	UFUNCTION()
	void AnyEmptySlotsAvailable(bool& HasEmptySlots, int32& EmptyIndex);

	UFUNCTION()
	bool CreateNewStack(FName ItemID, int32 Quantity);

	UFUNCTION()
	void SetupTraceLength();

	UFUNCTION()
	void TransferSlots(int32 SourceIndex, UInventorySystemComponent* SourceInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintPure)
	FItemStruct GetItemData(FName ItemID);
	FItemStruct GetDataTableRow_ItemData(FName RowName);

	UFUNCTION()
	FVector GetDropLocation();

	UFUNCTION()
	void ConsumeItem(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool QueryInventory(FName ItemID, int32 Quantity, int32& OutQuantity);

	UFUNCTION()
	int32 GetQuantity();

	UFUNCTION()
	void SaveInventory();

	UFUNCTION()
	void LoadInventory();


	UPROPERTY()
	int32 InventorySize;

	UPROPERTY()
	TArray<FSlotStruct> Content;
	
	UPROPERTY()
	AActor* LookAtActor;
	
	UPROPERTY()
	UCommonUserWidget* DisplayMessage;

	UPROPERTY()
	FCurrencyStruct Currency;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InteractRange;

	UPROPERTY()
	float InteractRange_Actual;
	
};
