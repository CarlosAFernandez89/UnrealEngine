// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGameModeInterface.h"
#include "GameFramework/GameMode.h"
#include "SaveData/SaveGame_Level.h"
#include "SolviGameModeBase.generated.h"

class USolviPawnData;

/**
 * Post login event, triggered when a player or bot joins the game as well as after seamless and non seamless travel
 *
 * This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSolviGameModePlayerInitialized, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

UCLASS()
class SOLVI_API ASolviGameModeBase : public AGameMode, public  IGameModeInterface
{
	GENERATED_BODY()



public:

	UPROPERTY()
	USaveGame_Level* SaveGame_Level;

	UPROPERTY()
	FString SlotName_Level;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void LoadLevelFromSaveFile();

	void RespawnActor(APawn* InPawn, ACharacter* InCharacter = nullptr, FTransform InSpawnTransform = FTransform());

	UFUNCTION(BlueprintCallable)
	void SpawnActor(TSubclassOf<APawn> PawnClass, FTransform SpawnTransform, APlayerController* PlayerController);

	virtual void TryRespawnPawn(APawn* Pawn, FTransform SpawnTransform) override;
};
