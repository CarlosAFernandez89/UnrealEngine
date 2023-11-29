#include "RPGGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FRPGGameplayTags FRPGGameplayTags::GameplayTags;

void FRPGGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FRPGGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	AddTag(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jump input");
	AddTag(InputTag_Sprint, "InputTag.Sprint", "Sprint input");
	AddTag(InputTag_Attack, "InputTag.Attack", "Attack input.");
	AddTag(InputTag_HeavyAttack, "InputTag.HeavyAttack", "HeavyAttack input.");
	AddTag(InputTag_Dodge, "InputTag.Dodge", "Dodge input");

	
	AddTag(InputTag_Pause, "InputTag.Menu.Pause", "Menu Pause input.");

	AddTag(InputTag_Interact, "InputTag.Interact", "Interact input.");



}

void FRPGGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}