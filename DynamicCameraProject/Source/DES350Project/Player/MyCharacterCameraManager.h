// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterCameraManager.generated.h"

USTRUCT()
struct FCameraHitResults
{
	GENERATED_BODY()

	UPROPERTY()
	int Quantity;
	UPROPERTY()
	float Length;
	TArray<FHitResult> HitResults;

	void Initialize(const int _Quantity, const float _Length)
	{
		this->Quantity = _Quantity;
		this->Length = _Length;
		for(int i = 0; i < _Quantity; ++i)
		{
			FHitResult TempHit(ForceInit);
			this->HitResults.Add(TempHit);
		}
	}
};

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Positive = 0 UMETA(DisplayName = "POSITIVE"),
	Negative = 1  UMETA(DisplayName = "NEGATIVE")
};

UENUM(BlueprintType)
enum class ERayDirection : uint8
{
	None = 0 UMETA(DisplayName = "NONE"),
	Up = 1 UMETA(DisplayName = "UP"),
	Down = 2 UMETA(DisplayName = "DOWN"),
	Left = 3 UMETA(DisplayName = "LEFT"),
	Right = 4 UMETA(DisplayName = "RIGHT")
};

class USpringArmComponent;
class UCameraComponent;
class UEnhancedInputLocalPlayerSubsystem;
class AMyCharacter;
class AMyPlayerController;

UCLASS()
class DES350PROJECT_API UMyCharacterCameraManager : public UActorComponent
{
	GENERATED_BODY()
	
private:
#pragma region PRIVATE_STUFF

	UPROPERTY()
	AMyPlayerController* PlayerController;
	UPROPERTY()
	AMyCharacter* PlayerCharacter;
	UPROPERTY()
	UCameraComponent* CameraComponent;
	UPROPERTY()
	USpringArmComponent* SpringArmComponent;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EISubsystem;
	// Set timer manager at BeginPlay to avoid constantly getting it
	FTimerManager* TimerManager;
	
	UPROPERTY()
	bool bIsCameraManagerActive;

	/*
	 * Using a timer to avoid drawing rays every frame,
	 * instead we draw them every 0.x seconds.
	 */
	UPROPERTY()
	FTimerHandle RaycastTimerHandle;
	UPROPERTY()
	FTimerHandle CameraMoveTimerHandle;
	
	

	/*
	 * Check if the player is moving its mouse.
	 * If the player is moving its mouse we dont want
	 * to change any of its controls to avoid player
	 * confusion.
	 */
	bool IsPlayerMovingMouse();
	UPROPERTY()
	float Enable_Logic_Timer;
	UPROPERTY()
	float Enable_Logic_Timer_Counter;

	
	void CameraRaycasts();
	void MoveCamera();
	bool IsPlayerIdle();
	void RestartIdleTimer();
	void IdleTimerCountdown();
	void CameraManagerTick();

	UPROPERTY()
	float Idle_Timer;
	UPROPERTY()
	float Idle_Timer_Counter;
	UPROPERTY()
	FTimerHandle IdleTimerHandle;
	UPROPERTY()
	bool bCanChangeMovementModes;

	// FVector to store the location the SpringArmComponent->TargetLocation will move after all the HitResults have been evaluated.
	UPROPERTY()
	FVector Wanted_Camera_Location;
	
	void DrawRays(FCameraHitResults& Ref,
	              float StartAngle = 0.f,
	              const FVector& RotateAxis = FVector(0.f, 0.f, 0.f),
	              float AngleIncrement = 0.f,
	              float EndpointOffset = 0.f,
	              FColor RayColor = FColor::Green,
	              ERayDirection RayDirection = ERayDirection::None
	              ) const;
	
	void DoLineTrace_Camera(FHitResult& HitResult,
	                        const float Distance = 0.0f,
	                        const float AngleOffset = 0.0f,
	                        const FColor DebugLineColor = FColor::Black,
	                        const FVector& RotateAxis = FVector(0.f,0.f,0.f),
	                        const float EndpointZOffset = 0.f,
	                        ERayDirection RayDirection = ERayDirection::None
	                        ) const;

	// Average all the hit locations and get the optimal camera location.
	void Calculate_Camera_Movement();
	void AddHitResultsImpactLocations(TArray<FVector>& Points, FCameraHitResults& Struct, float Clamp_Min = 0.f, float Clamp_Max = 1.f, EMoveDirection MoveDirection = EMoveDirection::Negative );
	static bool DidHitResultGetAHit(FCameraHitResults& Struct);
	void CalculateBestCameraLocation(TArray<FVector> Points);

	UPROPERTY()
	bool bAtLeastOneHit;

	// Point the camera towards the start of the SpringArmComponent.
	void CameraLookAtPlayer() const;
	
	UPROPERTY()
	FCameraHitResults SideRays;
	UPROPERTY()
	FCameraHitResults CeilingRays;
	UPROPERTY()
	FCameraHitResults FloorRays;
	UPROPERTY()
	FCameraHitResults RoofRays;
	UPROPERTY()
	FCameraHitResults InclineRays;
	UPROPERTY()
	FCameraHitResults DeclineRays;
	
#pragma endregion
	
public:
	explicit UMyCharacterCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FORCEINLINE bool IsCameraManagerActive() const { return bIsCameraManagerActive; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bDrawDebugLines;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float CameraInterpSpeed;
};
