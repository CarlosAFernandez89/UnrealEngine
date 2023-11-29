// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"

#include "ItemDataComponent.h"
#include "Containers/InventoryContainer_LootBag.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/InventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Solvi/SolviGameInstance.h"
#include "Solvi/SolviGameModeBase.h"


// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent(): InventorySize(12), LookAtActor(nullptr), DisplayMessage(nullptr),
                                                        InteractRange_Actual(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InteractRange = 200.f;
}


void UInventorySystemComponent::AutoSaveInventory()
{
	SaveInventory();
}

// Called when the game starts
void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadInventory();
	SetupTraceLength();
	Content.SetNum(InventorySize);
	OnInventoryUpdated.AddUniqueDynamic(this, &UInventorySystemComponent::AutoSaveInventory);
	
}

void UInventorySystemComponent::Server_ItemInteract_Implementation(AActor* TargetActor)
{
	if(UItemDataComponent* ItemData = TargetActor->GetComponentByClass<UItemDataComponent>())
	{
		if(AActor* Owner = GetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is of class: %s"), *Owner->GetClass()->GetName());
			if(ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(Owner))
			{
				ItemData->InteractWith(PlayerCharacter);
			}
		}
	}
	else
	{
		if(ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(GetOwner()))
		{
			if(AController* Controller = PlayerCharacter->GetController())
			{
				TargetActor->SetOwner(Controller);
				OnLocal_Interact(TargetActor, PlayerCharacter);
			}
		}
	}
}

void UInventorySystemComponent::OnLocal_Interact(AActor* TargetActor, AActor* Interactor)
{

	if(AActor* Owner = GetOwner())
	{
		if (Owner->HasAuthority())
		{
			if (TargetActor && Interactor)
			{
				if(ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(Interactor))
				{
					if(AInventoryContainer* Container = Cast<AInventoryContainer>(TargetActor))
					{
						PlayerCharacter->PlayerHUD->ShowContainerInventory(Container->InventorySystemComponent);
					}
				}
			}
			else
			{
				// Handle the case where one or both actor references are invalid.
			}
		}
	}
}

void UInventorySystemComponent::MulticastUpdateInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void UInventorySystemComponent::Server_TransferSlots_Implementation(int32 SourceIndex, UInventorySystemComponent* SourceInventory, int32 DestinationIndex)
{
	TransferSlots(SourceIndex, SourceInventory, DestinationIndex);
}

void UInventorySystemComponent::Server_Remove_Implementation(int32 Index, bool bRemoveWholeStack, bool bIsConsumed)
{
	RemoveFromInventory(Index, bRemoveWholeStack, bIsConsumed);
}

void UInventorySystemComponent::Server_DropItem_Implementation(FName ItemID, int32 Quantity)
{
	if(Quantity > 1)
	{
		if(UWorld* const World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const TSubclassOf<AActor> ActorClassToSpawn = GetItemData(ItemID).ItemClass;
			const FVector SpawnLocation = GetDropLocation();
			const FRotator SpawnRotation = FRotator(0.f,0.f,0.f);

			if(AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClassToSpawn, SpawnLocation, SpawnRotation, SpawnParameters))
			{
				if(AInventoryItem* InventoryItem = Cast<AInventoryItem>(SpawnedActor))
				{
					InventoryItem->OnDropped();
				}
			}
		}
	}
	else
	{
		if(UWorld* const World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const TSubclassOf<AActor> ActorClassToSpawn = AInventoryContainer_LootBag::StaticClass();
			const FVector SpawnLocation = GetDropLocation();
			const FRotator SpawnRotation = FRotator(0.f,0.f,0.f);

			if(AInventoryContainer_LootBag* SpawnedActor = World->SpawnActor<AInventoryContainer_LootBag>(ActorClassToSpawn, SpawnLocation, SpawnRotation, SpawnParameters))
			{
				int32 QuantityRemaining;
				SpawnedActor->InventorySystemComponent->AddToInventory(ItemID, Quantity,QuantityRemaining);
			}
		}
	}
}

void UInventorySystemComponent::Server_ConsumeItem_Implementation(FName ItemID)
{
	if(UKismetMathLibrary::ClassIsChildOf(GetOwner()->GetClass(),ASolviModularCharacter::StaticClass()))
	{
		if(UKismetSystemLibrary::IsValidClass(GetItemData(ItemID).ItemEffect))
		{
			if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(GetOwner()))
			{
				if(const UGameplayEffect* GameplayEffectClass = Cast<UGameplayEffect>(GetItemData(ItemID).ItemEffect->GetClass()))
				{
					const FGameplayEffectContextHandle ContextHandle;
					PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffectClass,1, ContextHandle);
				}
			}
			
		}
		else
		{
			//No item effect was added in data table.
		}
	}
	else
	{
		
	}
}

// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	InteractTrace(DeltaTime);
}

bool UInventorySystemComponent::AddToInventory(FName ItemID, int32 Quantity, int32& QuantityRemaining)
{
	int32 Local_QuantityRemaining = Quantity;
	bool Local_HasFailed = false;
	while (Local_QuantityRemaining > 0 && !Local_HasFailed)
	{
		int32 SlotIndex;
		if(FindSlot(ItemID, SlotIndex))
		{
			AddToStack(SlotIndex, 1);
			--Local_QuantityRemaining;
			OnInventoryUpdated.Broadcast();
		}
		else
		{
			bool bAnyEmptySlots = false;
			int32 EmptyIndex = -1;
			AnyEmptySlotsAvailable(bAnyEmptySlots, EmptyIndex);

			if(bAnyEmptySlots)
			{
				if(CreateNewStack(ItemID, 1))
				{
					--Local_QuantityRemaining;
					OnInventoryUpdated.Broadcast();
				}
				else
				{
					Local_HasFailed = true;
				}
			}
			else
			{
				Local_HasFailed = true;
			}
		}
	}

	OnInventoryUpdated.Broadcast();

	return !Local_HasFailed;
}

void UInventorySystemComponent::RemoveFromInventory(int32 Index, bool bRemoveWholeStack, bool bIsConsumed)
{
	const FName Local_Item = Content[Index].ItemID;
	const int32 Local_Quantity = Content[Index].Quantity;

	if(bRemoveWholeStack || Local_Quantity == 1)
	{
		Content[Index].ItemID = FName("");
		Content[Index].Quantity = 0;
		if(bIsConsumed)
		{
			//TODO: Logic for when a stack of items is consumed.
		}
		else
		{
			Server_DropItem(Local_Item, Local_Quantity);

			//TODO: Logic for when a stack of items is dropped on the ground.
		}
	}
	else
	{
		Content[Index].Quantity -= 1;

		if(bIsConsumed)
		{
			//TODO: Logic for when a single item is consumed.

		}
		else
		{
			Server_DropItem(Local_Item, 1);
			//TODO: Logic for when a single item is dropped on the ground.

		}
	}
}

void UInventorySystemComponent::InteractTrace(float DeltaTime)
{
	if(APlayerCameraManager* PlayerCameraManager =  UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		FVector StartLocation =
			PlayerCameraManager->GetCameraLocation() + ((PlayerCameraManager->GetActorForwardVector() * InteractRange_Actual) * 0.5);

		FVector EndLocation =
			PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * InteractRange_Actual;

		if(UWorld* const World = GetWorld())
		{
			FHitResult HitResult;
			ECollisionChannel TraceChannel = ECC_GameTraceChannel2;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(GetOwner());

			if(World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel, QueryParams))
			{
				//DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, DeltaTime, 0, 0.1f);
				
				if(HitResult.GetActor() != LookAtActor)
				{
					if(LookAtActor->IsValidLowLevel())
					{
						if(AInventoryItem* Item = Cast<AInventoryItem>(LookAtActor))
						{
							Item->StopLookAt();
						}
						else if(AInventoryContainer* Container = Cast<AInventoryContainer>(LookAtActor))
						{
							Container->StopLookAt();
						}

						if(AInventoryItem* Item = Cast<AInventoryItem>(LookAtActor))
						{
							FText Message = Item->LookAt();
							
							// Draw a point at the hit location to show where the line trace hit the actor.
							DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red, false, DeltaTime);

							// Optionally, draw a string at the hit location to display the LookAt message.
							GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message.ToString());
						}
						else if(AInventoryContainer* Container = Cast<AInventoryContainer>(LookAtActor))
						{
							FText Message = Container->LookAt();
							
							// Draw a point at the hit location to show where the line trace hit the actor.
							DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red, false, DeltaTime);

							// Optionally, draw a string at the hit location to display the LookAt message.
							GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message.ToString());
						}
						
						LookAtActor = HitResult.GetActor();
					}
					else
					{
						LookAtActor = HitResult.GetActor();
						if(AInventoryItem* Item = Cast<AInventoryItem>(LookAtActor))
						{
							FText Message = Item->LookAt();
							// Draw a point at the hit location to show where the line trace hit the actor.
							DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red, false, 2.f);

							// Optionally, draw a string at the hit location to display the LookAt message.
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message.ToString());
						}
						else if(AInventoryContainer* Container = Cast<AInventoryContainer>(LookAtActor))
						{
							FText Message = Container->LookAt();
							// Draw a point at the hit location to show where the line trace hit the actor.
							DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red, false, 2.f);

							// Optionally, draw a string at the hit location to display the LookAt message.
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message.ToString());
						}
					}
				}
			}
			else
			{
				//DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, DeltaTime, 0, 0.1f);
				//Clear look at actor since we aren't looking at anything.
				if(LookAtActor)
				{
					if(AInventoryItem* Item = Cast<AInventoryItem>(LookAtActor))
					{
						Item->StopLookAt();
					}
					else if(AInventoryContainer* Container = Cast<AInventoryContainer>(LookAtActor))
					{
						Container->StopLookAt();
					}

					LookAtActor = nullptr;
				}
			}
		}
		
	}
}

bool UInventorySystemComponent::FindSlot(FName ItemID, int32& Index)
{
	for(int Idx = 0; Idx < Content.Num(); ++Idx)
	{
		if(Content[Idx].ItemID == ItemID)
		{
			if(Content[Idx].Quantity < GetMaxStackSize(ItemID))
			{
				Index = Idx;
				return true;
			}
		}
	}

	Index = -1;
	return false;
}

int32 UInventorySystemComponent::GetMaxStackSize(FName ItemID)
{
	const FItemStruct OutRow = GetDataTableRow_ItemData(ItemID);
	return OutRow.StackSize;
}

void UInventorySystemComponent::AddToStack(int32 Index, int32 Quantity)
{
	Content[Index] = FSlotStruct(Content[Index].ItemID, Content[Index].Quantity + Quantity);
}

void UInventorySystemComponent::AnyEmptySlotsAvailable(bool& HasEmptySlots, int32& EmptyIndex)
{
	for (int32 Index = 0; Index < Content.Num(); ++Index)
	{
		const FSlotStruct& Element = Content[Index];
		if(Element.Quantity <= 0)
		{
			HasEmptySlots = true;
			EmptyIndex = Index;
			return;
		}
	}

	HasEmptySlots = false;
	EmptyIndex = -1;
}

bool UInventorySystemComponent::CreateNewStack(FName ItemID, int32 Quantity)
{
	bool bHasEmptySlots = false;
	int32 EmptyIndex = -1;
	AnyEmptySlotsAvailable(bHasEmptySlots, EmptyIndex);

	if(bHasEmptySlots)
	{
		Content[EmptyIndex] = FSlotStruct(ItemID, Quantity);
		return true;
	}
	
	return false;
}

void UInventorySystemComponent::SetupTraceLength()
{
	if(const ASolviModularCharacter* PlayerCharacter = Cast<ASolviModularCharacter>(GetOwner()))
	{
		InteractRange_Actual = PlayerCharacter->GetCameraBoom()->TargetArmLength + InteractRange;
	}
}

void UInventorySystemComponent::TransferSlots(int32 SourceIndex, UInventorySystemComponent* SourceInventory,
	int32 DestinationIndex)
{
	const FSlotStruct Local_SlotContent =  SourceInventory->Content[SourceIndex];

	if(DestinationIndex >= 0)
	{
		if(Local_SlotContent.ItemID == Content[DestinationIndex].ItemID)
		{
			const int32 MaxStackSize = GetMaxStackSize(Local_SlotContent.ItemID);
			int32 ItemQuantity = Local_SlotContent.Quantity + Content[DestinationIndex].Quantity;
			ItemQuantity -= MaxStackSize;

			ItemQuantity = UKismetMathLibrary::Clamp(ItemQuantity, 0, MaxStackSize);
			
			SourceInventory->Content[SourceIndex].Quantity = ItemQuantity;

			if(ItemQuantity > 0)
			{
				SourceInventory->Content[SourceIndex].ItemID = Local_SlotContent.ItemID;
			}
			else
			{
				SourceInventory->Content[SourceIndex].ItemID = FName();
			}

			Content[DestinationIndex].ItemID = Local_SlotContent.ItemID;
			ItemQuantity = UKismetMathLibrary::Clamp(Content[DestinationIndex].Quantity + Local_SlotContent.Quantity, 0, MaxStackSize);
			Content[DestinationIndex].Quantity = ItemQuantity;

			MulticastUpdateInventory();
			SourceInventory->MulticastUpdateInventory();
		}
		else
		{
			SourceInventory->Content[SourceIndex] = Content[DestinationIndex];
			Content[DestinationIndex] = Local_SlotContent;
			MulticastUpdateInventory();
			SourceInventory->MulticastUpdateInventory();
			AutoSaveInventory();
			
		}
	}
}

FItemStruct UInventorySystemComponent::GetItemData(FName ItemID)
{
	return GetDataTableRow_ItemData(ItemID);
}

// Function to get a row from the DataTable
FItemStruct UInventorySystemComponent::GetDataTableRow_ItemData(const FName RowName)
{
	// Get a reference to the DataTable asset
	static const FString ContextString(TEXT("DataTable Context")); // Provide a context string

	if (const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Inventory/ItemDescription.ItemDescription'")))
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

FVector UInventorySystemComponent::GetDropLocation()
{
	const FVector StartLocation =
		(GetOwner()->GetActorLocation()) +
		(UKismetMathLibrary::RandomUnitVectorInConeInDegrees(
			GetOwner()->GetActorForwardVector(),
			30.f) * 150.f);

	const FVector EndLocation = StartLocation - FVector(0.f,0.f,500.f);

	if(UWorld* const World = GetWorld())
	{
		FHitResult HitResult;
		ECollisionChannel TraceChannel = ECC_Visibility;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if(World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel, QueryParams))
		{
			return HitResult.Location;
		}
	}

	 return FVector(0.f,0.f,0.f);
}

void UInventorySystemComponent::ConsumeItem(const int32 Index)
{
	FName Local_ItemID = Content[Index].ItemID;
	int32 Local_Quantity = Content[Index].Quantity;

	Server_Remove(Index, false, true);
	Server_ConsumeItem(Local_ItemID);

	MulticastUpdateInventory();
}

bool UInventorySystemComponent::QueryInventory(const FName ItemID, const int32 Quantity, int32& OutQuantity)
{
	int32 Local_RunningTotal = 0;
	for (FSlotStruct Element : Content)
	{
		if(Element.ItemID == ItemID)
		{
			Local_RunningTotal += Element.Quantity;
		}
	}

	OutQuantity = Local_RunningTotal;
	return Local_RunningTotal >= Quantity;
}

int32 UInventorySystemComponent::GetQuantity()
{
	int32 Local_Sum = 0;
	for (const FSlotStruct Element : Content)
	{
		if(Element.Quantity > 0)
		{
			Local_Sum += Element.Quantity;
		}
	}

	return Local_Sum;
}

void UInventorySystemComponent::SaveInventory()
{
	if(USolviGameInstance* SolviGameInstance = Cast<USolviGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if(UGameplayStatics::GetPlayerCharacter(this,0) == GetOwner())
		{
			SolviGameInstance->SaveGame_Inventory->InventoryContent = Content;
			SolviGameInstance->SaveGame_Inventory->InventoryCurrency = Currency;

			UGameplayStatics::SaveGameToSlot(SolviGameInstance->SaveGame_Inventory, SolviGameInstance->SlotName_Inventory, 0);
		}
		else
		{
			ASolviGameModeBase* SolviGameModeBase =  Cast<ASolviGameModeBase>(UGameplayStatics::GetGameMode(this));
		}
	}
}

void UInventorySystemComponent::LoadInventory()
{
	if(USolviGameInstance* SolviGameInstance = Cast<USolviGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if(GetOwner() == UGameplayStatics::GetPlayerCharacter(this,0))
		{
			Content = SolviGameInstance->SaveGame_Inventory->InventoryContent;
			Currency = SolviGameInstance->SaveGame_Inventory->InventoryCurrency;
		}
		else
		{
			if(ASolviGameModeBase* SolviGameModeBase = Cast<ASolviGameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				TSoftClassPtr<UInventorySystemComponent> ThisSoftClassPtr = TSoftClassPtr<UInventorySystemComponent>(this);
				Content = SolviGameModeBase->SaveGame_Level->ContainersAdded.Find(ThisSoftClassPtr)->Content;
			}
		}
	}
}
