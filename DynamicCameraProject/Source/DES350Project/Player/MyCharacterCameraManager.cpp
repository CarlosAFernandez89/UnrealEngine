// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterCameraManager.h"

#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"
#include "MyInputConfigData.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"



UMyCharacterCameraManager::UMyCharacterCameraManager(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), PlayerController(nullptr), PlayerCharacter(nullptr), CameraComponent(nullptr),
	SpringArmComponent(nullptr),EISubsystem(nullptr),TimerManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

	bDrawDebugLines = true;
	CameraInterpSpeed = 3.f;
	bAtLeastOneHit = false;
	bCanChangeMovementModes = false;
	bIsCameraManagerActive = false;
	Wanted_Camera_Location = FVector(0.f, 0.f, 0.f);

	SideRays.Initialize(6, 200.f);
	CeilingRays.Initialize(4, 150.f);
	FloorRays.Initialize(1, 125.f);
	RoofRays.Initialize(1, 125.f);
	InclineRays.Initialize(1, 250.f);
	DeclineRays.Initialize(1,250.f);

	Enable_Logic_Timer = 1.5f;
	Enable_Logic_Timer_Counter = Enable_Logic_Timer;
	Idle_Timer = 0.5f;
	Idle_Timer_Counter = Idle_Timer;
}


// Called when the game starts
void UMyCharacterCameraManager::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	if(Owner && Owner->HasActiveCameraComponent())
	{
		//Get all necessary objects
		CameraComponent = CastChecked<UCameraComponent>(Owner->GetComponentByClass(UCameraComponent::StaticClass()));
		SpringArmComponent = CastChecked<USpringArmComponent>(Owner->GetComponentByClass(USpringArmComponent::StaticClass()));
		PlayerCharacter = CastChecked<AMyCharacter>(Owner);
		PlayerController = CastChecked<AMyPlayerController>(Owner->GetController());
		EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		check(EISubsystem)

		//Initialized timers
		TimerManager = &GetOwner()->GetWorld()->GetTimerManager();
		TimerManager->SetTimer(RaycastTimerHandle,this,&UMyCharacterCameraManager::CameraRaycasts, 0.1, true);
		TimerManager->SetTimer(CameraMoveTimerHandle, this, &UMyCharacterCameraManager::MoveCamera, FApp::GetDeltaTime(), true);
		TimerManager->SetTimer(IdleTimerHandle, this, &UMyCharacterCameraManager::IdleTimerCountdown, FApp::GetDeltaTime(), true);


	}
}


// Called every frame
void UMyCharacterCameraManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CameraManagerTick();

}

bool UMyCharacterCameraManager::IsPlayerMovingMouse()
{
	const FVector2D LookValue = EISubsystem->GetPlayerInput()->GetActionValue(PlayerCharacter->InputActions->InputLook).Get<FVector2d>();
	
	if(LookValue.X == 0.0f && LookValue.Y == 0.0f) // if mouse is NOT being moved...
	{
		Enable_Logic_Timer_Counter -= GetWorld()->GetDeltaSeconds(); //countdown to enable logic
		if(Enable_Logic_Timer_Counter < 0.0f ) //countdown over
		{			
			return false;
		}
		return true;
	}
	Enable_Logic_Timer_Counter = Enable_Logic_Timer;
	return true;
}


void UMyCharacterCameraManager::CameraRaycasts()
{
	if(PlayerController->GetViewTarget() == PlayerCharacter)
	{
		DrawRays(SideRays, 60.f, FVector(0.f,0.f,1.f),50.f);
		DrawRays(CeilingRays, 0.f,FVector(0.f,0.f,1.f), 90, 75.f);
		DrawRays(InclineRays, 0.f, FVector(0.f,0.f,1.f), 180.f, -200.f);
		DrawRays(DeclineRays, 180.f, FVector(0.f,0.f,1.f), 180.f, -200);
		DrawRays(RoofRays, 0.f, FVector(0.f,0.f,0.f), 0.f, 0.f, FColor::Orange, ERayDirection::Up);
		DrawRays(FloorRays, 0.f, FVector(0.f,0.f,0.f), 0.f, 0.f, FColor::Orange, ERayDirection::Down);
	}
}

void UMyCharacterCameraManager::MoveCamera()
{
	if(PlayerController->GetViewTarget() == PlayerCharacter)
	{
		// Player isn't moving, no need to do anything.
		// Unless we want fancy Idle Camera Movement.
		if(IsPlayerIdle()) { return; } 
	
		Calculate_Camera_Movement();

		SpringArmComponent->TargetOffset  = UKismetMathLibrary::VInterpTo(
			SpringArmComponent->TargetOffset,
			Wanted_Camera_Location,
			FApp::GetDeltaTime(),
			3.0f
			);

		CameraLookAtPlayer();
	}
}

bool UMyCharacterCameraManager::IsPlayerIdle()
{
	// if player is not moving and the idle counter isn't on already...
	if(UKismetMathLibrary::NearlyEqual_FloatFloat(PlayerCharacter->GetVelocity().Length(), 0.f)
		&& !TimerManager->IsTimerActive(IdleTimerHandle))
	{
		RestartIdleTimer();
		TimerManager->UnPauseTimer(IdleTimerHandle);
	}

	if(!UKismetMathLibrary::NearlyEqual_FloatFloat(PlayerCharacter->GetVelocity().Length(), 0.f)
		&& TimerManager->IsTimerActive(IdleTimerHandle))
	{
		RestartIdleTimer();
		TimerManager->PauseTimer(IdleTimerHandle);
	}
	
	bCanChangeMovementModes = Idle_Timer_Counter <= 0.f;
	return Idle_Timer_Counter <= 0.f;
}

void UMyCharacterCameraManager::RestartIdleTimer()
{
	Idle_Timer_Counter = Idle_Timer;
}

void UMyCharacterCameraManager::IdleTimerCountdown()
{
	Idle_Timer_Counter -= FApp::GetDeltaTime();
}

void UMyCharacterCameraManager::CameraManagerTick()
{
	if(PlayerController->GetViewTarget() == PlayerCharacter)
	{
		if(!IsPlayerMovingMouse())
		{
			if(!TimerManager->IsTimerActive(RaycastTimerHandle) )
			{
				bIsCameraManagerActive = true;
				TimerManager->UnPauseTimer(RaycastTimerHandle);
			}
			return;
		}

		bIsCameraManagerActive = false;
		TimerManager->PauseTimer(RaycastTimerHandle);
	}
}

void UMyCharacterCameraManager::DrawRays(FCameraHitResults& Ref,
				  float StartAngle,
				  const FVector& RotateAxis,
				  float AngleIncrement,
				  float EndpointOffset,
				  FColor RayColor,
				  ERayDirection RayDirection
				  ) const
{
	for(int i = 0; i < Ref.Quantity; ++i)
	{
		DoLineTrace_Camera(Ref.HitResults[i],Ref.Length, StartAngle, RayColor, RotateAxis, EndpointOffset,RayDirection);
		StartAngle += AngleIncrement;
	}
}

void UMyCharacterCameraManager::DoLineTrace_Camera(FHitResult& HitResult,
                                                   const float Distance ,
                                                   const float AngleOffset,
                                                   const FColor DebugLineColor,
                                                   const FVector& RotateAxis,
                                                   const float EndpointZOffset,
                                                   ERayDirection RayDirection) const
{
	
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = FVector(0.f,0.f,0.f);

	switch (RayDirection)
	{
	case ERayDirection::None:
		{
			FVector Direction = CameraComponent->GetComponentTransform().GetRotation().GetForwardVector();
			
			Direction.Z = 0.f;
			if(RotateAxis != FVector(0.f,0.f,0.f))
			{
				Direction = UKismetMathLibrary::RotateAngleAxis(Direction, AngleOffset, RotateAxis );
			}
			TraceEnd = TraceStart + (Direction * Distance);
			break;
		}
	case ERayDirection::Up:
		{
			FVector Direction = CameraComponent->GetComponentTransform().GetRotation().GetUpVector();
			if(RotateAxis != FVector(0.f,0.f,0.f))
			{
				Direction = UKismetMathLibrary::RotateAngleAxis(Direction, AngleOffset, RotateAxis );
			}
			TraceEnd = TraceStart + (Direction * Distance);
			break;
		}
	case ERayDirection::Down:
		{
			FVector Direction = CameraComponent->GetComponentTransform().GetRotation().GetUpVector();
			if(RotateAxis != FVector(0.f,0.f,0.f))
			{
				Direction = UKismetMathLibrary::RotateAngleAxis(Direction, AngleOffset, RotateAxis );
			}
			TraceEnd = TraceStart + (Direction * -Distance);
			break;
		}
	case ERayDirection::Left:
		{
			FVector Direction = CameraComponent->GetComponentTransform().GetRotation().GetRightVector();
			if(RotateAxis != FVector(0.f,0.f,0.f))
			{
				Direction = UKismetMathLibrary::RotateAngleAxis(Direction, AngleOffset, RotateAxis );
			}
			TraceEnd = TraceStart + (Direction * Distance);
			break;
		}
	case ERayDirection::Right:
		{
			FVector Direction = CameraComponent->GetComponentTransform().GetRotation().GetRightVector();
			if(RotateAxis != FVector(0.f,0.f,0.f))
			{
				Direction = UKismetMathLibrary::RotateAngleAxis(Direction, AngleOffset, RotateAxis );
			}
			TraceEnd = TraceStart + (Direction * -Distance);
			break;
		}
	default:
		{
			break;
		};
	}
	

	if(EndpointZOffset != 0.f)
	{
		TraceEnd.Z = FMath::Abs(TraceEnd.Z + EndpointZOffset);
	}
	
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("CQP_Camera")));
	Params.AddIgnoredActor(PlayerCharacter);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,ECC_Camera,Params);

	if(bDrawDebugLines)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Red : DebugLineColor, false, 0.5f );
	}

}

void UMyCharacterCameraManager::Calculate_Camera_Movement()
{
	if(PlayerController->GetViewTarget() == PlayerCharacter)
	{
		bAtLeastOneHit = false;
		if(TimerManager->IsTimerActive(RaycastTimerHandle)) // if we are actually processing the raycasts
			{
			TArray<FVector> Points;

			AddHitResultsImpactLocations(Points, SideRays, SideRays.Length * 0.1f, SideRays.Length);

			AddHitResultsImpactLocations(Points, RoofRays, 0.f, RoofRays.Length, EMoveDirection::Positive);

			if(DidHitResultGetAHit(RoofRays) == false)
			{
				AddHitResultsImpactLocations(Points, CeilingRays, 0.f,CeilingRays.Length);
			}

			AddHitResultsImpactLocations(Points, FloorRays, 0.f, FloorRays.Length);

			//if(DidHitResultGetAHit(FloorRays) == false)
			{
				AddHitResultsImpactLocations(Points, InclineRays, 0.f, InclineRays.Length);

				AddHitResultsImpactLocations(Points, DeclineRays, 0.f, DeclineRays.Length);
			}

			CalculateBestCameraLocation(Points);
			}
	
		if(bAtLeastOneHit == false)
		{
			Wanted_Camera_Location = FVector(0.f,0.f,0.f); // No hit, take it back to default.
		}
	}
}

void UMyCharacterCameraManager::AddHitResultsImpactLocations(TArray<FVector>& Points, FCameraHitResults& Struct,
	float Clamp_Min, float Clamp_Max, EMoveDirection MoveDirection)
{
	for (int i = 0; i < Struct.HitResults.Num(); ++i)
	{
		if(Struct.HitResults[i].bBlockingHit)
		{
			if(Struct.HitResults[i].GetActor()->ActorHasTag("CameraIgnore")) // Ignore actors with this tag
			{
				break;
			}
			bAtLeastOneHit = true;
			
			//Get the direction of the raycast (from ray end to end start)
			const FRotator Direction = UKismetMathLibrary::FindLookAtRotation(Struct.HitResults[i].Location, CameraComponent->GetComponentLocation());

			//Calculate the distance the camera should move. Distance between the hit location and the trace end location.
			float DistanceToMove = UKismetMathLibrary::Vector_Distance(Struct.HitResults[i].Location, Struct.HitResults[i].TraceEnd);

			DistanceToMove = FMath::Clamp(DistanceToMove, Clamp_Min, Clamp_Max);
			FVector WantedWorldLocation = FVector(0.f,0.f,0.f);
			
			// Clamp the distance and set its direction.
			switch (MoveDirection)
			{
			case EMoveDirection::Positive:
				{
					// Calculate new location. From raycast hit location towards raycast start location * distance.
					WantedWorldLocation =  Struct.HitResults[i].Location + (UKismetMathLibrary::GetForwardVector(Direction) *  DistanceToMove);
				}
			case EMoveDirection::Negative:
				{
					// Calculate new location. From raycast hit location towards raycast start location * distance.
					WantedWorldLocation =  Struct.HitResults[i].Location - (UKismetMathLibrary::GetForwardVector(Direction) *  DistanceToMove);
				}
				default: ;
			}

			// convert these coordinates into  "relative space" for the SpringArm TargetOffset.
			Points.Add(Struct.HitResults[i].Location - WantedWorldLocation);
		}
	}
}

bool UMyCharacterCameraManager::DidHitResultGetAHit(FCameraHitResults& Struct)
{
	for(int i = 0; i < Struct.HitResults.Num();++i)
	{
		if(Struct.HitResults[i].bBlockingHit)
		{
			return true;
		}
	}
	return false;
}

void UMyCharacterCameraManager::CalculateBestCameraLocation(TArray<FVector> Points)
{
	// Calculate the best possible location given all the ray collisions.
	if(!Points.IsEmpty()) // if at least one point was stored
	{
		FVector Endpoint = FVector(0.f,0.f,0.f);
		for (const FVector Point : Points)
		{
			Endpoint.X += Point.X;
			Endpoint.Y += Point.Y;
			Endpoint.Z += Point.Z;
		}
		//Set the location the SpringArm will interp to.
		Wanted_Camera_Location = UKismetMathLibrary::Divide_VectorFloat(Endpoint, Points.Num());
	}
}

void UMyCharacterCameraManager::CameraLookAtPlayer() const
{
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
																CameraComponent->GetComponentLocation(),
																PlayerCharacter->GetActorLocation() + SpringArmComponent->GetRelativeLocation()
																);
	const FRotator RelativeRotation = UKismetMathLibrary::InverseTransformRotation(CameraComponent->GetComponentTransform(), LookAtRotation);

	CameraComponent->SetRelativeRotation(
		UKismetMathLibrary::RInterpTo(
			CameraComponent->GetRelativeRotation(),
			RelativeRotation,
			FApp::GetDeltaTime(),
			CameraInterpSpeed)
			);
}

