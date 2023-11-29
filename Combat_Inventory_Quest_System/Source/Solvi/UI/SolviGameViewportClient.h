// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "SolviGameViewportClient.generated.h"

class UGameInstance;
class UObject;

UCLASS(BlueprintType)
class SOLVI_API USolviGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	USolviGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};