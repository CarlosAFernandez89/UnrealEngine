// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviModularCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameUIManagerSubsystem.h"
#include "PrimaryGameLayout.h"
#include "SolviCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Algo/Sort.h"
#include "Input/SolviInputConfigData.h"
#include "Kismet/GameplayStatics.h"
#include "Solvi/SolviGameplayTags.h"
#include "Solvi/InventorySystem/InventorySystemComponent.h"
#include "Solvi/QuestSystem/QuestLogComponent.h"


ASolviModularCharacter::ASolviModularCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 350.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventorySystemComponent"));
	QuestLogComponent = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogComponent"));


}

// Called when the game starts or when spawned
void ASolviModularCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add input mapping context to player
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* LPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			//Add all MappingContexts here
			LPS->AddMappingContext(InputMapping,0);
		}
	}

	
}

void ASolviModularCharacter::Move(const FInputActionValue& Value)
{
	if(!CanPlayerMove())
	{
		return;
	}
	
	const FVector2D MoveValue = Value.Get<FVector2D>();

	FRotator YawRotation = FRotator(0.f,GetControlRotation().Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(MoveValue.Y != 0.f)
	{
		AddMovementInput(ForwardDirection, MoveValue.Y);
	}

	if(MoveValue.X != 0.f)
	{
		AddMovementInput(RightDirection, MoveValue.X);
	}
	
}

void ASolviModularCharacter::Look(const FInputActionValue& Value)
{
	if(IsLockedOn() || CanPlayerMoveCamera())
	{
		return;
	}
		
	const FVector2D LookValue = Value.Get<FVector2D>();
 
	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}
 
	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y);
	}
}


// Called every frame
void ASolviModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASolviModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this , &ASolviModularCharacter::Move);
	EIC->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this , &ASolviModularCharacter::Look);
	EIC->BindAction(InputActions->ItemInteract, ETriggerEvent::Triggered, this, &ASolviModularCharacter::IA_ItemInteract);
	EIC->BindAction(InputActions->OpenInventory, ETriggerEvent::Triggered,this,&ASolviModularCharacter::IA_OpenInventory);

}

AActor* ASolviModularCharacter::SwitchTarget(const TArray<AActor*>& Actors,
	ACharacter* Character, AActor* SelectedActor, ESwitchTargetDirection Direction)
{
	// Check if there's only one actor (or none), in which case there are no actors to the left or right
	if (Actors.Num() <= 1) {
		return nullptr;
	}
	
	// Get the Reference Vector, which is the player's camera world forward vector
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) {
		return nullptr;
	}
	FVector ReferenceVector = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();


	// Sort Using Algo::Sort
	TArray<AActor*> SortedActors = Actors;
	Algo::Sort(SortedActors, [&Character, &ReferenceVector](const AActor* A, const AActor* B) {
		FVector VectorToA = A->GetActorLocation() - Character->GetActorLocation();
		FVector VectorToB = B->GetActorLocation() - Character->GetActorLocation();

		float AngleA = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ReferenceVector.GetSafeNormal(), VectorToA.GetSafeNormal())));
		float AngleB = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ReferenceVector.GetSafeNormal(), VectorToB.GetSafeNormal())));

		return AngleA < AngleB;
	});

	AActor* DesiredActor = SelectedActor; // Default to current actor in case there's no actor to the left or right.
	for (int32 i = 0; i < SortedActors.Num(); ++i)
	{
		if (SortedActors[i] == SelectedActor)
		{
			if (Direction == ESwitchTargetDirection::Right && i < SortedActors.Num() - 1)
			{
				DesiredActor = SortedActors[i + 1];
			}
			else if (Direction == ESwitchTargetDirection::Left && i > 0)
			{
				DesiredActor = SortedActors[i - 1];
			}
			break;
		}
	}

	return DesiredActor;
}

bool ASolviModularCharacter::CanPlayerMove()
{
	return !HasMatchingGameplayTag(SolviGameplayTags::FindTagByString("Status.Movement.Blocked"));
}

bool ASolviModularCharacter::IsLockedOn()
{
	return HasMatchingGameplayTag(SolviGameplayTags::FindTagByString("Status.LockedOnTarget"));
}

bool ASolviModularCharacter::CanPlayerMoveCamera()
{
	return UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor;
}

void ASolviModularCharacter::IA_ItemInteract()
{
	if(InventorySystemComponent->LookAtActor)
	{
		InventorySystemComponent->Server_ItemInteract_Implementation(InventorySystemComponent->LookAtActor);
	}
}

void ASolviModularCharacter::IA_OpenInventory()
{
	OpenInventory();
}
