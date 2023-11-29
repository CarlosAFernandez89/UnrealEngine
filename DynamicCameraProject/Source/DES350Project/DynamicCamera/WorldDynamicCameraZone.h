// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/Input/SSpinBox.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "WorldDynamicCameraZone.generated.h"

class UArrowComponent;
class USplineComponent;
class UBoxComponent;
class UCameraComponent;


UENUM(BlueprintType)
enum class ECameraMovementType : uint8
{
	DistanceFromStart = 0 UMETA(DisplayName = "Distance From Start"),
	ClosestPointFromCharacter = 1  UMETA(DisplayName = "Closest Point From Character")
};

UENUM(BlueprintType)
enum class ESplineMovementDirection : uint8
{
	Forward = 0 UMETA(DisplayName = "Forward"),
	Reverse = 1  UMETA(DisplayName = "Reverse")
};

UCLASS()
class DES350PROJECT_API AWorldDynamicCameraZone : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ArrowComponent;
	
public:
	// Sets default values for this actor's properties
	AWorldDynamicCameraZone();

	UFUNCTION()
	void SwitchCameraView(AActor* NewViewTarget, float BlendTime = 1.f, EViewTargetBlendFunction BlendFunction = VTBlend_Linear, float BlendExp = 0.f) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(BlueprintReadWrite)
	float _BlendTime;

	// OverlapStart Event. Only does logic when possessed player overlaps.
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// OverlapEnd Event. Only does logic when possessed player overlaps.
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//Pointer to possessed pawn actor.
	UPROPERTY()
	AActor* OverlappedActor;
	FVector OverlappedActor_EnterLocation;
	// Gets the distance from the start location (overalaped actor OverlapBegin location)
	UFUNCTION()
	float GetDistanceFromEnterLocation();
	

	FTimerManager* TimerManager;

	// Camera movement behaviour setter.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="DynamicCamera")
	ECameraMovementType CameraMovementType;

	//Timer handle for CameraLookAt logic.
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerHandle_CameraLookAt;
	// Interpolation speed for CameraLookAt
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="DynamicCamera")
	float CameraLookAt_InterpSpeed;
	// Camera Look At logic.
	UFUNCTION()
	void CameraLook();
	void InterpolateCameraWorldRotation(const FRotator& Target) const;
	// If the camera should look at the player or the ArrowComponent.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DynamicCamera")
	bool bLookAtPlayer;

	// Timer Handle for camera interpolation along the spline component.
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerHandle_CameraMoveAlongSpline;
	// Sets the start location of the camera depending where the actor enters the collider from.
	UPROPERTY()
	ESplineMovementDirection SplineMovementDirection;
	// Interpolation speed for CameraMoveAlongSpline
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="DynamicCamera")
	float CameraMoveAlongSpline_InterpSpeed;
	// Camera Move Along Spline logic.
	UFUNCTION()
	void CameraMoveAlongSpline();
	void InterpolateCameraWorldLocation(const FVector& Target) const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="DynamicCamera")
	bool bHideHUD;

	// Prints a warning if the player ever goes out of camera view.
	UFUNCTION()
	void PlayerOutOfViewWarning() const;

#pragma region SLATE
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	TWeakPtr<SWindow> PopUpWindowWeakPtr;
#pragma endregion
	
};

class SWorldDynamicCameraZone final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWorldDynamicCameraZone) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, AWorldDynamicCameraZone* InYourClassInstance)
	{
		ClassInstance = InYourClassInstance;
		
		for(int32 Option = 0; Option < static_cast<int32>(ECameraMovementType::ClosestPointFromCharacter) + 1; ++Option)
		{
			CameraMovementsEnumOptions.Add(MakeShared<ECameraMovementType>(static_cast<ECameraMovementType>(Option)));
		}
		
		ChildSlot
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("CameraLookAt_InterpSpeed"))
					.Justification(ETextJustify::Left)
					.MinDesiredWidth(150)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SNew(SSpinBox<float>)
					.Value_Lambda([this]()->float
					{
						return ClassInstance->CameraLookAt_InterpSpeed;
					})
					.OnValueChanged_Lambda([&](float NewValue)
					{
						ClassInstance->CameraLookAt_InterpSpeed = NewValue;
					})
					.MinDesiredWidth(50)
					.Justification(ETextJustify::Right)
					.EnableWheel(true)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("CameraMoveAlongSpline_InterpSpeed"))
					.Justification(ETextJustify::Left)
					.MinDesiredWidth(150)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SNew(SSpinBox<float>)
					.Value_Lambda([this]()->float
					{
						return ClassInstance->CameraMoveAlongSpline_InterpSpeed;
					})
					.OnValueChanged_Lambda([&](float NewValue)
					{
						ClassInstance->CameraMoveAlongSpline_InterpSpeed = NewValue;
					})
					.MinDesiredWidth(50)
					.Justification(ETextJustify::Right)
					.EnableWheel(true)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Camera Movement Type"))
					.Justification(ETextJustify::Left)
					.MinDesiredWidth(150)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SNew(SComboBox<TSharedPtr<ECameraMovementType>>)
					.OptionsSource(&CameraMovementsEnumOptions)
					.OnGenerateWidget_Lambda([this](const TSharedPtr<ECameraMovementType>& InOption)->TSharedRef<SWidget>
					{
						const UEnum* EnumPtr = FindFirstObjectSafe<UEnum>(TEXT("ECameraMovementType"));
						if (!EnumPtr) return SNew(STextBlock).Text(FText::FromString("Invalid"));
						return SNew(STextBlock).Text(FText::FromString(EnumPtr->GetNameStringByIndex(static_cast<int32>(*InOption))));
					})
					.OnSelectionChanged_Lambda([this](TSharedPtr<ECameraMovementType> NewSelection, ESelectInfo::Type SelectInfo)
					{
						CurrentMovementValue = *NewSelection;
					})
					.InitiallySelectedItem(CameraMovementsEnumOptions[0])
					[
					SNew(STextBlock)
					.Text_Lambda([this]()->FText
					{
						const UEnum* EnumPtr = FindFirstObjectSafe<UEnum>(TEXT("ECameraMovementType"));
						if (!EnumPtr) return FText::FromString("Invalid");
						return FText::FromString(EnumPtr->GetNameStringByIndex(static_cast<int32>(CurrentMovementValue)));
					})
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Look at Player?"))
					.Justification(ETextJustify::Left)
					.MinDesiredWidth(150)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SNew(SCheckBox)
					.OnCheckStateChanged_Lambda([&](ECheckBoxState NewState)
					{
						ClassInstance->bLookAtPlayer = (NewState == ECheckBoxState::Checked);
					})
					.IsChecked(ClassInstance->bLookAtPlayer ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Hide HUD?"))
					.Justification(ETextJustify::Left)
					.MinDesiredWidth(150)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SNew(SCheckBox)
					.OnCheckStateChanged_Lambda([&](ECheckBoxState NewState)
					{
						ClassInstance->bHideHUD = (NewState == ECheckBoxState::Checked);
					})
					.IsChecked(ClassInstance->bHideHUD ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				]
			]
		];
	}


private:
	AWorldDynamicCameraZone* ClassInstance = nullptr;
	TArray<TSharedPtr<ECameraMovementType>> CameraMovementsEnumOptions;
	ECameraMovementType CurrentMovementValue = ECameraMovementType::DistanceFromStart;
};