// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#include "Components/WidgetComponent.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "Solvi/InventorySystem/ItemDataComponent.h"
#include "Solvi/InventorySystem/Widget/ItemPopUp_Widget.h"



// Sets default values
AInventoryItem::AInventoryItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialize the root component first
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Initialize the pop-up widget and attach it to the root component
	PopUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryWidget"));
	PopUpWidget->SetupAttachment(RootComponent);

	// Initialize the mesh and attach it to the root component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);

	// Initialize the item data component and attach it to the root component
	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));

	FontSize = 12.f;
	Interact_HoverMessage = FText(FText::FromString(TEXT("Press E to Interact")));

}

FText AInventoryItem::LookAt() const
{
	const FItemStruct ItemStruct = GetDataTableRow_ItemData(ItemDataComponent->ItemID.RowName);
	if(ItemStruct.Name.IsEmpty())
	{
		return FText::FromString(TEXT("DataTable has not been set for this item."));
	}
	if(PopUpWidget)
	{
		PopUpWidget->SetHiddenInGame(false,false);
	}
	return FText::Format(FText::FromString(TEXT("Pick up {0}")), ItemStruct.Name);
}

void AInventoryItem::StopLookAt() const
{
	if(PopUpWidget)
	{
		PopUpWidget->SetHiddenInGame(true,false);
	}
}

void AInventoryItem::InteractWith(ASolviModularCharacter* PlayerCharacter)
{
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	if(UItemPopUp_Widget* Widget = Cast<UItemPopUp_Widget>(PopUpWidget->GetWidget()))
	{
		Widget->FontSize = FontSize;
		Widget->InteractHoverMessage = Interact_HoverMessage;
	}
	
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventoryItem::OnDropped()
{
}

// Function to get a row from the DataTable
FItemStruct AInventoryItem::GetDataTableRow_ItemData(const FName RowName) const
{
	// Get a reference to the DataTable asset
	static const FString ContextString(TEXT("DataTable Context")); // Provide a context string

	if (TObjectPtr<const UDataTable> DataTable = ItemDataComponent->ItemID.DataTable)
	{
		// Use the FindRow function to retrieve the row by name
		if (FItemStruct* Row = DataTable->FindRow<FItemStruct>(RowName, ContextString, true))
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
	return FItemStruct();
}
