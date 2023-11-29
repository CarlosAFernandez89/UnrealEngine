#pragma once

#include "NativeGameplayTags.h"

namespace SolviGameplayTags
{
	SOLVI_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

		// Declare all of the custom native tags that Lyra will use
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	SOLVI_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	SOLVI_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	SOLVI_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
}
