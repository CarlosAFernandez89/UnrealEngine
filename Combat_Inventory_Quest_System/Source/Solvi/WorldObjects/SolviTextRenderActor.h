// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "SolviTextRenderActor.generated.h"

UCLASS()
class SOLVI_API ASolviTextRenderActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASolviTextRenderActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> DefaultRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTextRenderComponent> TextRenderComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	FText TextValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	float TextSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	FColor TextRenderColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	UMaterialInterface* MeshMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	FVector StaticMeshScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	TEnumAsByte<EHorizTextAligment> HorizontalAlignment;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	TEnumAsByte<EVerticalTextAligment> VerticalAlignment;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Default|Values")
	FVector TextLocationOffset;
	
};
