// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"
#include "MyInputConfigData.h"
#include "MyPlayerCameraManager.h"
#include "MyPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DES350Project/GameplayAbility/MyAbilitySystemComponent.h"
#include "DES350Project/Utility/ObjectGetter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Hero/MyHeroCharacter.h"
#include "HUD/MyDamageTextWidgetComponent.h"
#include "HUD/MyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
AMyPlayerController::AMyPlayerController(): WorldTimerManager(nullptr),SprintSpeed(900.f),
											DefaultWalkSpeed(0),bFreeCamera(true),
											bIsUsingWorldCamera(false),PlayerCharacter(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetArmLengthAlpha = 100.0f;
	WantedTargetArmLength = 300.0f;
	MinTargetArmLength = 150.f;
	MaxTargetArmLength = 800.f;
	StartRotator = FRotator(ForceInit);
	TargetRotator = FRotator(ForceInit);
}

// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	
	if(UMyAbilitySystemComponent* ASC = Cast<UMyAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent()))
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

// Called every frame
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPlayerController::ShowHUD()
{
	const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	AMyHUD* HUD = PC->GetHUD<AMyHUD>();
	HUD->ShowMyHUD();
}

void AMyPlayerController::HideHUD()
{
	const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	AMyHUD* HUD = PC->GetHUD<AMyHUD>();
	HUD->HideMyHUD();
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	if(PlayerCharacter->CanMove())
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
	
		if(bIsUsingWorldCamera)
		{
			if(!WorldTimerManager->IsTimerActive(MovementDirection_TimerHandle))
			{
				YawRotation.Yaw = PlayerCameraManager->GetCameraRotation().Yaw;
			}
			else
			{
				TargetRotator = PlayerCameraManager->GetCameraRotation();
			}
		}
		else
		{
			YawRotation =
				FRotator(
				0,
				UKismetMathLibrary::SelectFloat(GetControlRotation().Yaw,PlayerCharacter->GetCapsuleComponent()->GetComponentRotation().Yaw,bFreeCamera),
				0);
		}
	
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
 
		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			//SelectFloat allows it to change between a free camera to a tank style camera movement.
			// In this case, it doesn't allow moving back due to no animations for said logic.
			const float Scale =
				UKismetMathLibrary::SelectFloat(
				MoveValue.Y,
				FMath::Clamp(MoveValue.Y,0.f,1.f),
				bFreeCamera
				);
		
			PlayerCharacter->AddMovementInput(ForwardDirection, Scale);
		}
 
		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			//SelectFloat allows it to change between a free camera to a tank style camera movement.
			// In this case, it lowers the sensitivity of turning for easier player controls.
			const float Scale =
				UKismetMathLibrary::SelectFloat(
				MoveValue.X,
				MoveValue.X * 0.1f,
				bFreeCamera
				);
		
			PlayerCharacter->AddMovementInput(RightDirection, Scale);
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	if(!bIsUsingWorldCamera)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
 
		if (LookValue.X != 0.f)
		{
			PlayerCharacter->AddControllerYawInput(LookValue.X);
		}
 
		if (LookValue.Y != 0.f)
		{
			PlayerCharacter->AddControllerPitchInput(LookValue.Y);
		}
	}
}

void AMyPlayerController::Sprint_Start(const FInputActionValue& Value)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	PlayerCharacter->bIsSprinting = true;
	WorldTimerManager->UnPauseTimer(SprintCameraLogic_TimerHandle);
}

void AMyPlayerController::Sprint_End(const FInputActionValue& Value)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	PlayerCharacter->bIsSprinting = false;
}

void AMyPlayerController::SprintCameraLogic()
{
	if(PlayerCharacter->bIsSprinting && !bIsUsingWorldCamera)
	{
		if(PlayerCharacter->GetVelocity().Length() >= 0.1f)
		{
			AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this);
			MyPlayerCameraManager->SprintCameraState(115.f);
		}
		else
		{
			AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this);
			MyPlayerCameraManager->SprintCameraState(MyPlayerCameraManager->DefaultFOV);
		}
	}
	
	if(!PlayerCharacter->bIsSprinting && !bIsUsingWorldCamera)
	{
		AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this);
		MyPlayerCameraManager->SprintCameraState(MyPlayerCameraManager->DefaultFOV);

		if(UKismetMathLibrary::NearlyEqual_FloatFloat(PlayerCharacter->GetCameraComponent()->FieldOfView, MyPlayerCameraManager->DefaultFOV))
		{
			WorldTimerManager->PauseTimer(SprintCameraLogic_TimerHandle);
		}
	}

	if(PlayerCharacter->bIsSprinting && bIsUsingWorldCamera)
	{
		AMyPlayerCameraManager* MyPlayerCameraManager = UObjectGetter::GetMyPlayerCameraManager(this);
		MyPlayerCameraManager->SprintCameraState(MyPlayerCameraManager->DefaultFOV);
	}
}

void AMyPlayerController::ToggleMovementMode(const FInputActionValue& Value)
{
	bFreeCamera = !bFreeCamera;
	PlayerCharacter->GetSpringArmComponent()->bUsePawnControlRotation = bFreeCamera;
}

void AMyPlayerController::ToggleWorldCameraSettings()
{
	if(WorldTimerManager)
	{
		if(WorldTimerManager->IsTimerActive(MovementDirection_TimerHandle))
		{
			WorldTimerManager->PauseTimer(MovementDirection_TimerHandle);
		}
		else
		{
			WorldTimerManager->UnPauseTimer(MovementDirection_TimerHandle);
			StartRotator = GetControlRotation();
		}
	}
	
	bIsUsingWorldCamera = !bIsUsingWorldCamera;
	PlayerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = bIsUsingWorldCamera;
}

void AMyPlayerController::InterpolateMovementDirections()
{
	StartRotator= UKismetMathLibrary::RInterpTo(StartRotator,TargetRotator, FApp::GetDeltaTime(), 1.f);
	YawRotation.Yaw = StartRotator.Yaw;
	if(UKismetMathLibrary::NearlyEqual_FloatFloat(YawRotation.Yaw, TargetRotator.Yaw))
	{
		WorldTimerManager->PauseTimer(MovementDirection_TimerHandle);
	}
}


bool AMyPlayerController::IsPlayerMovingMouse()
{
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	const FVector2D LookValue = Subsystem->GetPlayerInput()->GetActionValue(PlayerCharacter->InputActions->InputLook).Get<FVector2d>();
	
	if(LookValue.X == 0.0f && LookValue.Y == 0.0f)
	{
		return false;
	}
	return true;
}

FInputActionValue AMyPlayerController::GetInputActionValue(const UInputAction* ForAction)
{
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	return Subsystem->GetPlayerInput()->GetActionValue(ForAction).Get<FVector2d>();
}


void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = Cast<AMyHeroCharacter>(InPawn);
	WantedTargetArmLength = PlayerCharacter->GetSpringArmComponent()->TargetArmLength;
	WorldTimerManager = &GetWorldTimerManager();
	if(WorldTimerManager)
	{
		WorldTimerManager->SetTimer(CameraZoomTimerHandle, this, &AMyPlayerController::CameraZoomLerp, FApp::GetDeltaTime(), true);
		WorldTimerManager->SetTimer(MovementDirection_TimerHandle, this, &AMyPlayerController::InterpolateMovementDirections, FApp::GetDeltaTime(), true);
		WorldTimerManager->SetTimer(SprintCameraLogic_TimerHandle, this, &AMyPlayerController::SprintCameraLogic, FApp::GetDeltaTime(), true);
		WorldTimerManager->PauseTimer(SprintCameraLogic_TimerHandle);
	}

	DefaultWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	ShowHUD();
	

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if(PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
	
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ShowHUD();
}

void AMyPlayerController::CameraZoom(const FInputActionValue& Value)
{

	const float ZoomValue = Value.Get<float>();

	if(ZoomValue == 0.0f) { return; }

	//Set new WantedTargetArmLength depending on input direction.
	if(ZoomValue < 0.01)
	{
		WantedTargetArmLength = PlayerCharacter->GetSpringArmComponent()->TargetArmLength + TargetArmLengthAlpha;

	}
	else
	{
		WantedTargetArmLength = PlayerCharacter->GetSpringArmComponent()->TargetArmLength - TargetArmLengthAlpha;
	}

	//Clamp min/max TargetArmLength 
	WantedTargetArmLength = FMath::Clamp(WantedTargetArmLength, MinTargetArmLength, MaxTargetArmLength);

	//Start interpolation timer if it isn't on.
	if(!WorldTimerManager->IsTimerActive(CameraZoomTimerHandle))
	{
		WorldTimerManager->UnPauseTimer(CameraZoomTimerHandle);
	}
}

void AMyPlayerController::CameraZoomLerp()
{
	
	PlayerCharacter->GetSpringArmComponent()->TargetArmLength =
			UKismetMathLibrary::FInterpTo(
			PlayerCharacter->GetSpringArmComponent()->TargetArmLength,
			WantedTargetArmLength,
			FApp::GetDeltaTime(),
			5.0f);

	GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Black, FString::Printf(TEXT("Interpolating TargetArmLength...")));

	//Pause timer once it has reached its desired TargetArmLength
	if(FMath::IsNearlyEqual(PlayerCharacter->GetSpringArmComponent()->TargetArmLength, WantedTargetArmLength, 1.0f))
	{
		GetWorldTimerManager().PauseTimer(CameraZoomTimerHandle);
	}
}

AMyHUD* AMyPlayerController::GetHUD()
{
	return static_cast<AMyHUD*>(MyHUD);
}

void AMyPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AMyCharacter* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		UMyDamageTextWidgetComponent* DamageText = NewObject<UMyDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool AMyPlayerController::ShowDamageNumber_Validate(float DamageAmount, AMyCharacter* TargetCharacter)
{
	return true;
}

void AMyPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
	}
}

bool AMyPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

