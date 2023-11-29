// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Solvi/InventorySystem/IItemInteract.h"
#include "InventoryContainer.generated.h"

UCLASS()
class SOLVI_API AInventoryContainer : public AActor , public IItemInteract
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AInventoryContainer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventorySystemComponent> InventorySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual FText LookAt() const override;
	virtual void InteractWith(ASolviModularCharacter* PlayerCharacter) override;
	virtual void StopLookAt() const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float FontSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FText Interact_HoverMessage;
};
