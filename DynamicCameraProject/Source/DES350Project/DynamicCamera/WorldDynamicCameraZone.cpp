// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamicCameraZone.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "../Utility/ObjectGetter.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "DES350Project/Player/MyPlayerController.h"
#include "DES350Project/Player/MyCharacter.h"
#include "DES350Project/Player/Hero/MyHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AWorldDynamicCameraZone::AWorldDynamicCameraZone() : CameraMovementType(ECameraMovementType::DistanceFromStart), CameraLookAt_InterpSpeed(2.f),
													bLookAtPlayer(true), SplineMovementDirection(ESplineMovementDirection::Forward), CameraMoveAlongSpline_InterpSpeed(2.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(SceneComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	if(BoxCollider)
	{
		BoxCollider->SetupAttachment(SceneComponent);
		BoxCollider->SetHiddenInGame(false);
		BoxCollider->SetGenerateOverlapEvents(true);
		BoxCollider->SetBoxExtent(FVector(100.f,100.f,100.f),false);
		BoxCollider->SetCollisionProfileName(TEXT("Trigger"),false);
		BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWorldDynamicCameraZone::OnOverlapBegin);
		BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AWorldDynamicCameraZone::OnOverlapEnd);
	}
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	if(CameraComponent)
	{
		CameraComponent->SetupAttachment(SceneComponent);
	}

	_BlendTime = 0.5f;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	if(SplineComponent)
	{
		SplineComponent->SetupAttachment(SceneComponent);
		CameraComponent->SetWorldTransform(SplineComponent->GetComponentTransform());
	}

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	if(ArrowComponent)
	{
		ArrowComponent->SetupAttachment(SceneComponent);
		ArrowComponent->SetWorldScale3D(FVector(3.f,3.f,3.f));
	}

	bHideHUD = false;
}

void AWorldDynamicCameraZone::SwitchCameraView(AActor* NewViewTarget, float BlendTime,
	EViewTargetBlendFunction BlendFunction, float BlendExp) const
{
	if(AMyPlayerController* PC = UObjectGetter::GetMyPlayerController(this))
	{
		PC->SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunction, BlendExp,true);
	}
}

// Called when the game starts or when spawned
void AWorldDynamicCameraZone::BeginPlay()
{
	Super::BeginPlay();
	TimerManager = &UGameplayStatics::GetPlayerController(this, 0)->GetWorldTimerManager();
	if(TimerManager)
	{
		TimerManager->SetTimer(TimerHandle_CameraLookAt,this,&AWorldDynamicCameraZone::CameraLook, FApp::GetDeltaTime(), true);
		TimerManager->PauseTimer(TimerHandle_CameraLookAt);
		TimerManager->SetTimer(TimerHandle_CameraMoveAlongSpline, this, &AWorldDynamicCameraZone::CameraMoveAlongSpline, FApp::GetDeltaTime(), true);
		TimerManager->PauseTimer(TimerHandle_CameraMoveAlongSpline);
	}
}


void AWorldDynamicCameraZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, TEXT("On Component Overlap Begin!"));

		AMyHeroCharacter* Character = UObjectGetter::GetMyPlayerCharacter(this);
		
		
		if(const AActor* Actor = CastChecked<AActor>(Character); OtherActor == Actor )
		{
			OverlappedActor = OtherActor;
			OverlappedActor_EnterLocation = OverlappedActor->GetActorLocation();
			
			const FVector LastPointLocation = SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);
			const FVector FirstPointLocation = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
			
			if(UKismetMathLibrary::Vector_Distance(LastPointLocation, OverlappedActor_EnterLocation) < UKismetMathLibrary::Vector_Distance(FirstPointLocation, OverlappedActor_EnterLocation))
			{
				SplineMovementDirection = ESplineMovementDirection::Reverse;
				CameraComponent->SetWorldLocation(LastPointLocation);
				CameraComponent->SetWorldRotation(Character->GetCameraComponent()->GetComponentRotation());
			}
			else
			{
				SplineMovementDirection = ESplineMovementDirection::Forward;
				CameraComponent->SetWorldLocation(FirstPointLocation);
				CameraComponent->SetWorldRotation(Character->GetCameraComponent()->GetComponentRotation());
			}
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Green, FString::Printf(TEXT("My Location is: %s"), *OverlappedActor_EnterLocation.ToString()));
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Green, FString::Printf(TEXT("Excaled Box Extent is: %s"), *BoxCollider->GetScaledBoxExtent().ToString()));

			UObjectGetter::GetMyPlayerController(this)->ToggleWorldCameraSettings();
			SwitchCameraView(this,_BlendTime,VTBlend_Linear);
			TimerManager->UnPauseTimer(TimerHandle_CameraLookAt);
			TimerManager->UnPauseTimer(TimerHandle_CameraMoveAlongSpline);
			if(bHideHUD)
			{
				UObjectGetter::GetMyPlayerController(this)->HideHUD();
			}
		}
	}
}

void AWorldDynamicCameraZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && (OtherActor != this) && OtherComp && OverlappedActor)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, TEXT("On Component Overlap End!"));
		
		if(OtherActor == OverlappedActor)
		{
			OverlappedActor = nullptr;
			UObjectGetter::GetMyPlayerController(this)->ToggleWorldCameraSettings();
			if(AMyCharacter* Character = UObjectGetter::GetMyPlayerCharacter(this))
			{
				SwitchCameraView(Character, _BlendTime, VTBlend_Linear);
			}
			TimerManager->PauseTimer(TimerHandle_CameraLookAt);
			TimerManager->PauseTimer(TimerHandle_CameraMoveAlongSpline);
		}

		if(bHideHUD)
		{
			UObjectGetter::GetMyPlayerController(this)->ShowHUD();
		}
	}
}

float AWorldDynamicCameraZone::GetDistanceFromEnterLocation()
{
	return UKismetMathLibrary::Vector_Distance(OverlappedActor_EnterLocation, OverlappedActor->GetActorLocation());
}

void AWorldDynamicCameraZone::CameraLook()
{
	if(OverlappedActor)
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, FApp::GetDeltaTime(), FColor::Red, TEXT("CameraLookAt!"));
		FRotator LookAtRotation;
		if(bLookAtPlayer)
		{
			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraComponent->GetComponentLocation(), OverlappedActor->GetActorLocation());
		}
		else
		{
			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraComponent->GetComponentLocation(), ArrowComponent->GetComponentLocation());

#if WITH_EDITOR
			DrawDebugSphere(GetWorld(), ArrowComponent->GetComponentLocation(), 50.f, 8, FColor::Red, false, FApp::GetDeltaTime());
			PlayerOutOfViewWarning();
#endif
			
		}

		InterpolateCameraWorldRotation(LookAtRotation);
	}
}

void AWorldDynamicCameraZone::InterpolateCameraWorldRotation(const FRotator& Target) const
{
	CameraComponent->SetWorldRotation(
	UKismetMathLibrary::RInterpTo(
		CameraComponent->GetComponentRotation(),
		Target,
		FApp::GetDeltaTime(),
		CameraLookAt_InterpSpeed)
		);
}

void AWorldDynamicCameraZone::CameraMoveAlongSpline()
{
	if(OverlappedActor)
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, FApp::GetDeltaTime(), FColor::Red, TEXT("CameraMoveAlongSpline!"));

		FVector WantedLocation = FVector(0.f,0.f,0.f);
		switch (CameraMovementType)
		{
		case ECameraMovementType::DistanceFromStart:
			{
				float Distance = UKismetMathLibrary::NormalizeToRange(GetDistanceFromEnterLocation(), 0.f, BoxCollider->GetScaledBoxExtent().Y * 2.f);
				
				switch (SplineMovementDirection)
				{
				case ESplineMovementDirection::Forward:
					{
						Distance = Distance * SplineComponent->GetSplineLength();
						WantedLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
						break;
					}
				case ESplineMovementDirection::Reverse:
					{
						Distance = SplineComponent->GetSplineLength() - Distance * SplineComponent->GetSplineLength();
						WantedLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
						break;
					}
				default: ;
				}
				break;
			}
		case ECameraMovementType::ClosestPointFromCharacter:
			{
				WantedLocation = SplineComponent->FindLocationClosestToWorldLocation(OverlappedActor->GetActorLocation(),ESplineCoordinateSpace::World);
				break;
			}
		default: ;
		}
		InterpolateCameraWorldLocation(WantedLocation);
	}
}

void AWorldDynamicCameraZone::InterpolateCameraWorldLocation(const FVector& Target) const
{
	CameraComponent->SetWorldLocation(
	UKismetMathLibrary::VInterpTo(
		CameraComponent->GetComponentLocation(),
		Target,
		FApp::GetDeltaTime(),
		CameraMoveAlongSpline_InterpSpeed)
		);
}

void AWorldDynamicCameraZone::PlayerOutOfViewWarning() const
{
	if(const AMyPlayerController* PC = UObjectGetter::GetMyPlayerController(this))
	{
		FVector2D ScreenLocation = FVector2D(0.f,0.f);
		int32 ViewportSizeX = 0;
		int32 ViewportSizeY = 0;
		PC->ProjectWorldLocationToScreen(OverlappedActor->GetActorLocation(),ScreenLocation);
		PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
		if(constexpr float Margin = 100.f; ScreenLocation.X < Margin || ScreenLocation.Y < Margin || ScreenLocation.X > ViewportSizeX - Margin || ScreenLocation.Y > ViewportSizeY - Margin)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, FApp::GetDeltaTime(), FColor::Yellow, TEXT("Character is going out of view! Modify spline to keep character in view at all times."));
		}
	}
}

void AWorldDynamicCameraZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AWorldDynamicCameraZone,CameraLookAt_InterpSpeed ))
	{
		if(!PopUpWindowWeakPtr.IsValid())
		{
			const TSharedRef<SWindow> PopUpWindow = SNew(SWindow)
			.Title(FText::FromString(UKismetSystemLibrary::GetDisplayName(this)))
			.ClientSize(FVector2D(350,200))
			.SupportsMinimize(false).SupportsMaximize(false)
			.IsTopmostWindow(true)
			[
				SNew(SWorldDynamicCameraZone, this)
			];
			FSlateApplication::Get().AddWindow(PopUpWindow);
			PopUpWindowWeakPtr = PopUpWindow;
		}
	}
}

