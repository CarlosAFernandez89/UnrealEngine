#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "WorldEnemyRespawnSubsystem.generated.h"

USTRUCT()
struct FWorldEnemyInfo
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TSubclassOf<ACharacter> EnemyClass;
	
	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

UCLASS()
class UWorldEnemyRespawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void AddEnemy(AActor* Enemy);
	
	
	UFUNCTION(BlueprintCallable)
	void InitialGetEnemyPopulation();
	
	UFUNCTION(BlueprintCallable)
	void CheckpointUpdateEnemiesToRespawn();

	UFUNCTION(BlueprintCallable)
	void RespawnWorldEnemies();
	
	// End USubsystem
	
private:
	// All my variables
	UPROPERTY()
	TArray<AActor*> ActorArrayTemp;

	UPROPERTY()
	TArray<FWorldEnemyInfo> EnemiesToRespawn;
	
};