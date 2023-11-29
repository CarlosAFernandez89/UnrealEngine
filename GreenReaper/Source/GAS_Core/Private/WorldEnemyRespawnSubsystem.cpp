#include "WorldEnemyRespawnSubsystem.h"

#include <Actor.h>

#include "RPGCharacterBase.h"
#include "Kismet/GameplayStatics.h"

void UWorldEnemyRespawnSubsystem::AddEnemy(AActor* Enemy)
{
	FWorldEnemyInfo NewEnemy;
	NewEnemy.EnemyClass = Enemy->GetClass();
	NewEnemy.Location =  Enemy->GetActorLocation();
	NewEnemy.Rotation = Enemy->GetActorRotation();
	EnemiesToRespawn.Add(NewEnemy);
}

void UWorldEnemyRespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void UWorldEnemyRespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	EnemiesToRespawn.Empty();
}

void UWorldEnemyRespawnSubsystem::InitialGetEnemyPopulation()
{
	UGameplayStatics::GetAllActorsOfClass(this, ARPGCharacterBase::StaticClass(),ActorArrayTemp );
	for (AActor* Actor : ActorArrayTemp)
	{
		if(CastChecked<ARPGCharacterBase>(Actor)->bIsPlayer == false)
		{
			AddEnemy(Actor);
		}
	}
}

void UWorldEnemyRespawnSubsystem::CheckpointUpdateEnemiesToRespawn()
{
	EnemiesToRespawn.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, ARPGCharacterBase::StaticClass(),ActorArrayTemp );
	for (AActor* Actor : ActorArrayTemp)
	{
		if(CastChecked<ARPGCharacterBase>(Actor)->bIsPlayer == false)
		{
			AddEnemy(Actor);
		}
	}
}

void UWorldEnemyRespawnSubsystem::RespawnWorldEnemies()
{
	UGameplayStatics::GetAllActorsOfClass(this, ARPGCharacterBase::StaticClass(),ActorArrayTemp );

	for (auto Actor : ActorArrayTemp)
	{
		if(CastChecked<ARPGCharacterBase>(Actor)->bIsPlayer == false)
		{
			Actor->Destroy();
		}

	}
	
	for (int i = 0; i < EnemiesToRespawn.Num(); i++)
	{
		GetWorld()->SpawnActor<ACharacter>(EnemiesToRespawn[i].EnemyClass,
			EnemiesToRespawn[i].Location, EnemiesToRespawn[i].Rotation);
	}

}

