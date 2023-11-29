// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GAS_Core.h"
#include "URPGGameViewportClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostFocusSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGainedFocusSignature);


UCLASS()
class GAS_CORE_API URPGGameViewportClient : public UGameViewportClient
{
  GENERATED_BODY()
	
public:

	static URPGGameViewportClient& Get();

	UPROPERTY(BlueprintAssignable, Category="Lost Focus")
	FLostFocusSignature OnLostFocus;

	UPROPERTY(BlueprintAssignable, Category="Lost Focus")
	FGainedFocusSignature OnGainedFocus;

	void CheckIfForegroundWindow();

	virtual void LostFocus(FViewport* VP) override;

	virtual void ReceivedFocus(FViewport* VP) override;

	void MinimizeWindow();

};
