// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryContainer.h"

#include "Components/WidgetComponent.h"
#include "Solvi/Character/Widget/PlayerHUD_Widget.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "Solvi/InventorySystem/Widget/ItemPopUp_Widget.h"


// Sets default values
AInventoryContainer::AInventoryContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Initialize the root component first
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	

	InventoryWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryWidget"));
	InventoryWidget->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);

	InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventorySystemComponent"));

	FontSize = 12.f;
	Interact_HoverMessage = FText(FText::FromString(TEXT("Press E to Interact")));
	
}

// Called when the game starts or when spawned
void AInventoryContainer::BeginPlay()
{
	Super::BeginPlay();

	if(UItemPopUp_Widget* Widget = Cast<UItemPopUp_Widget>(InventoryWidget->GetWidget()))
	{
		Widget->FontSize = FontSize;
		Widget->InteractHoverMessage = Interact_HoverMessage;
	}
	
}

FText AInventoryContainer::LookAt() const
{
	InventoryWidget->SetHiddenInGame(false, false);
	return FText();
}

void AInventoryContainer::InteractWith(ASolviModularCharacter* PlayerCharacter)
{
	//TODO: Do logic for interact in BP
	PlayerCharacter->PlayerHUD->ShowContainerInventory(InventorySystemComponent);
}

void AInventoryContainer::StopLookAt() const
{
	InventoryWidget->SetHiddenInGame(true, false);
}

