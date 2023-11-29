// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviTextRenderActor.h"

#include "Components/TextRenderComponent.h"


// Sets default values
ASolviTextRenderActor::ASolviTextRenderActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRootComponent;
	
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRenderComponent->SetupAttachment(DefaultRootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(DefaultRootComponent);
	
}


// Called when the game starts or when spawned
void ASolviTextRenderActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASolviTextRenderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

