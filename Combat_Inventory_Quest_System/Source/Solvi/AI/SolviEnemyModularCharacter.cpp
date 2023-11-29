// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviEnemyModularCharacter.h"


// Sets default values
ASolviEnemyModularCharacter::ASolviEnemyModularCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASolviEnemyModularCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASolviEnemyModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASolviEnemyModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

