// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviControllerDisconnectedScreen.h"
#include "Components/HorizontalBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SolviControllerDisconnectedScreen)

void USolviControllerDisconnectedScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	HBox_SwitchUser->SetVisibility(ESlateVisibility::Collapsed);

	//#if PLATFORM_HAS_STRICT_CONTROLLER_PAIRING
	//	if (FPlatformApplicationMisc::GetControllerIdForUnpairedControllers() != INDEX_NONE)
	//	{
	//		HBox_SwitchUser->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//	}
	//#endif
}

