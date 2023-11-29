// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AArenaManager::AArenaManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	Super::BeginPlay();


	// Add all actors of type AArena* to
	// the array so designers dont have to do it.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArena::StaticClass(),FoundActors);
	for (const auto FoundActor : FoundActors)
	{
		AArena* It = Cast<AArena>(FoundActor);
		if(It)
		{
			Arenas.Add(It);
		}
	}
	
}

// Called every frame
void AArenaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

