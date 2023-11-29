// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHeroCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameplayTagsManager.h"
#include "Camera/CameraComponent.h"
#include "DES350Project/GameplayAbility/MyAbilitySystemComponent.h"
#include "DES350Project/GameplayAbility/Abilities/MyAbilitySet.h"
#include "DES350Project/GameplayAbility/Input/MyEnhancedInputComponent.h"
#include "DES350Project/Player/MyCharacterCameraManager.h"
#include "DES350Project/Player/MyInputConfigData.h"
#include "DES350Project/Player/MyPlayerController.h"
#include "DES350Project/Player/MyPlayerState.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AMyHeroCharacter::AMyHeroCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AMyHeroCharacter::AMyHeroCharacter(const FObjectInitializer& ObjectInitializer)
{
	MyCharacterCameraManager = CreateDefaultSubobject<UMyCharacterCameraManager>(TEXT("DynamicCamera"));

	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 350.0f;
	SpringArmComponent->CameraRotationLagSpeed = 5.f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMyHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UMyEnhancedInputComponent* PEI = Cast<UMyEnhancedInputComponent>(PlayerInputComponent);
	check(PEI)
	
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	TArray<uint32> BindHandles;
	
	PEI->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

}

void AMyHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UMyAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		
		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());

		// End respawn specific things


		AddStartupEffects();

		AddCharacterAbilities();

		AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
		if (PC)
		{
			PC->ShowHUD();
		}

		if(AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
		}
	}
}

void AMyHeroCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AMyHeroCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void AMyHeroCharacter::FinishDying()
{
	Super::FinishDying();
}



