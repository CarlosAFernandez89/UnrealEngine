// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Solvi/Character/SolviModularCharacter.h"
#include "Solvi/InventorySystem/IItemInteract.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "InventoryItem.generated.h"

UCLASS()
class SOLVI_API AInventoryItem : public AActor, public IItemInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventoryItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemDataComponent> ItemDataComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> PopUpWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// IItemInteract start -------------------
	virtual FText LookAt() const override;
	virtual void StopLookAt() const override;
	virtual void InteractWith(ASolviModularCharacter* PlayerCharacter) override;
	// IItemInteract end -------------------

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnDropped();

	UFUNCTION()
	FItemStruct GetDataTableRow_ItemData(FName RowName) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float FontSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FText Interact_HoverMessage;
};
