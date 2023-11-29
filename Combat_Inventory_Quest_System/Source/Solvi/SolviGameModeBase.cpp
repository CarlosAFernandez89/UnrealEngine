// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void ASolviGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LoadLevelFromSaveFile();

}

void ASolviGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayStatics::SaveGameToSlot(SaveGame_Level, SlotName_Level,0);
	
	Super::EndPlay(EndPlayReason);
}

void ASolviGameModeBase::LoadLevelFromSaveFile()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	SlotName_Level = LevelName.Append("_Data");

	if(UGameplayStatics::DoesSaveGameExist(SlotName_Level,0))
	{
		if(USaveGame_Level* SaveGame = Cast<USaveGame_Level>(UGameplayStatics::LoadGameFromSlot(SlotName_Level, 0)))
		{
			SaveGame_Level = SaveGame;

			UGameplayStatics::SaveGameToSlot(SaveGame_Level, SlotName_Level, 0);

			if(UWorld* const World = GetWorld())
			{
				for (FItemSaveStruct Element : SaveGame_Level->ActorsAdded)
				{
					FActorSpawnParameters SpawnParameters;
					SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

					const TSubclassOf<AActor> ActorClassToSpawn = Element.Actor->GetClass();
					const FVector SpawnLocation = Element.Transform.GetLocation();
					const FRotator SpawnRotation = Element.Transform.GetRotation().Rotator();

					if(AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClassToSpawn, SpawnLocation, SpawnRotation, SpawnParameters))
					{
						//Don't need to do anything. Just spawning saved objects into world.
					}
				}
			}

			for (TSoftObjectPtr<AActor> Element : SaveGame_Level->ActorsRemoved)
			{
				if(Element.IsValid())
				{
					if(AActor* ActorToDelete = Element.LoadSynchronous())
					{
						ActorToDelete->Destroy();
					}
				}
			}
		}
	}
	else
	{
		SaveGame_Level = Cast<USaveGame_Level>(UGameplayStatics::CreateSaveGameObject(SaveGame_Level->GetClass()));
		UGameplayStatics::SaveGameToSlot(SaveGame_Level, SlotName_Level,0);
	}
}

void ASolviGameModeBase::RespawnActor(APawn* InPawn, ACharacter* InCharacter, FTransform InSpawnTransform)
{
	APlayerController* Local_Controller =  Cast<APlayerController>(InPawn->GetController());

	InPawn->Destroy();

	if(Local_Controller)
	{
		SpawnActor(InPawn->GetClass(), InSpawnTransform, Local_Controller);
	}
}

void ASolviGameModeBase::SpawnActor(TSubclassOf<APawn> PawnClass, FTransform SpawnTransform,
                                    APlayerController* PlayerController)
{
	if(UWorld* const World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		const FVector SpawnLocation = SpawnTransform.GetLocation();
		const FRotator SpawnRotation = SpawnTransform.GetRotation().Rotator();

		if(APawn* SpawnedActor = World->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, SpawnParameters))
		{
			PlayerController->Possess(SpawnedActor);
		}
	}
}

void ASolviGameModeBase::TryRespawnPawn(APawn* Pawn, FTransform SpawnTransform)
{
	RespawnActor(Pawn, nullptr, SpawnTransform);
}
