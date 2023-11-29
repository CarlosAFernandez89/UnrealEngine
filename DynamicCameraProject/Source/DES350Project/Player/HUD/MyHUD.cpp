// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>

AMyHUD::AMyHUD(): MyHUD(nullptr)
{
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMyHUD::ShowMyHUD()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	MyHUD = CreateWidget<UUserWidget>(PC, MyHUDClass);
	MyHUD->AddToViewport();
}

void AMyHUD::HideMyHUD()
{
	if(MyHUD)
	{
		MyHUD->RemoveFromParent();
		MyHUD = nullptr;
	}
}
