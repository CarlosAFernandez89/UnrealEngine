// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IItemInteract.h"
#include "Components/ActorComponent.h"
#include "Solvi/Character/SolviModularCharacter.h"
#include "ItemDataComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLVI_API UItemDataComponent : public UActorComponent, public IItemInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemDataComponent();

	// IItemInteract start -------------------
	virtual FText LookAt() const override
	{
		return FText::FromName(ItemID.RowName);
	}
	
	virtual void StopLookAt() const override;
	virtual void InteractWith(ASolviModularCharacter* PlayerCharacter) override;
	// IItemInteract end -------------------

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle ItemID;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Quantity;
};
