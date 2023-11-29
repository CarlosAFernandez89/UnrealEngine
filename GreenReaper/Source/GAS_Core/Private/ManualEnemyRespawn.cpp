// Fill out your copyright notice in the Description page of Project Settings.


#include "ManualEnemyRespawn.h"

#include <string>

#include "RPGCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManualEnemyRespawn::AManualEnemyRespawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//EnemyDefault = FindObject<UClass>(nullptr, TEXT("/Game/Blueprints/NPC/EnemyDefault"), true);

}

void AManualEnemyRespawn::EnemyStatusReport()
{
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPGCharacterBase::StaticClass(), FoundEnemies);
	
}

void AManualEnemyRespawn::EnemyRespawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, UTF8_TO_TCHAR(std::to_string(FoundEnemies.Max()).c_str()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for(int i = 0; i < FoundEnemies.Max(); i++)
	{
		if(Cast<ARPGCharacterBase>(FoundEnemies[i])->GetHealth() == 0)
		{
			Cast<ARPGCharacterBase>(FoundEnemies[i])->isDead = true;
		}
		if(Cast<ARPGCharacterBase>(FoundEnemies[i])->isDead == true)
		{
			auto SpawnedActor = GetWorld()->SpawnActor<AActor>(Cast<ARPGCharacterBase>(FoundEnemies[i])->AStartingPos, FoundEnemies[i]->GetActorRotation(), SpawnInfo);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Cast<ARPGCharacterBase>(FoundEnemies[i])->AStartingPos.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, SpawnedActor->GetName());
		}
	}
	FoundEnemies.Empty();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UTF8_TO_TCHAR(std::to_string(FoundEnemies.Max()).c_str()));
}

// Called when the game starts or when spawned
void AManualEnemyRespawn::BeginPlay()
{
	Super::BeginPlay();

	//TArray<AActor* > TempFoundEnemies;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPGCharacterBase::StaticClass(), FoundEnemies);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("sdjfhjsdhfs"));
	//FoundEnemies = TempFoundEnemies;
}

// Called every frame
void AManualEnemyRespawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UTF8_TO_TCHAR(std::to_string(FoundEnemies.Max()).c_str()));
}

