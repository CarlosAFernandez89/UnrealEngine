// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CheatManager.h"
#include "Arena.generated.h"


USTRUCT(BlueprintType)
struct FArrayHandleStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FTimerHandle> THandle;
	TArray<FTimerDelegate> TDelegate;
};

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_BODY()

	FSpawnInfo():	SpawnLocationObjectRef(nullptr),
					TimeBetweenSpawns(1.0f),
					NumberOfEnemies(0),
					SpawnRadius(0.0f),
					InRate(0.f)
	{
	}
	
	/* Given an object as spawn reference. Spawn point? Can be offset by giving a value to SpawnLocation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SpawnLocationObjectRef;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TimeBetweenSpawns;

	/*Number of Enemies to Spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfEnemies;

	/*Radius is in centimeters, a radius is the distance from the center of a circle to its edge.
	* If the radius is too small to fit however many enemies you decided to spawn, nothing will happen.
	* This prevents the game from crashing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDelay;
	
	UPROPERTY(BlueprintReadOnly)
	float InRate;
};

USTRUCT(BlueprintType)
struct FEnemyStruct
{
	GENERATED_USTRUCT_BODY()

	FEnemyStruct(): SpawnInfo()
	{
	}

public:

	/*Enemy Type*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter> Enemy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSpawnInfo SpawnInfo;
	
	UPROPERTY()
	FArrayHandleStruct ArrayHandleStruct;
	
};

UCLASS(Blueprintable)
class GAS_CORE_API AArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArena(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY()
	USceneComponent* SceneComponent;
	
	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(BlueprintReadWrite)
	UBillboardComponent* BillboardComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyStruct> EnemyType;

	UFUNCTION()
	void SpawnStart();
	
	UFUNCTION()
	void SpawnEnemiesWithRadius(FEnemyStruct& EnemyStruct);


	/*######### Spawn enemies delay stuff ####################*/

	
	UFUNCTION()
	void SpawnEnemies(FEnemyStruct& EnemyStruct);

	UFUNCTION()
	void Spawn(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ACharacter> Type);
	
	/* Delay for first spawn after trigger*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpawnDelayTime;

	//#######################################################
	
	UPROPERTY(BlueprintReadWrite)
	bool bSpawnEnemies;

	/* Array that holds all spawned enemies*/

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> EnemiesInArena;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnArenaStart();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnArenaCleared();

	UFUNCTION(BlueprintNativeEvent)
	void CheckArenaEnemiesAlive();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnSucceed_Implementation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ACharacter> Type);

	UFUNCTION(BlueprintNativeEvent)
	TArray<AActor*> GetAllEnemiesInArena();

	UFUNCTION(BlueprintNativeEvent)
	void OnArenaReset();

};

