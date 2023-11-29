// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "GameFramework/Actor.h"
#include "ManualEnemyRespawn.generated.h"


UCLASS()
class GAS_CORE_API AManualEnemyRespawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManualEnemyRespawn();

	/* status reporting for enemy default. Informs when an enemy has died. */
	UFUNCTION(BlueprintCallable)
	void EnemyStatusReport();

	/* respawn all manually placed enemies that died when the player hits restart */
	UFUNCTION(BlueprintCallable)
	void EnemyRespawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* reference to all active enemies in the level */
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor* > FoundEnemies;

	//UClass* EnemyDefault;
	
};
