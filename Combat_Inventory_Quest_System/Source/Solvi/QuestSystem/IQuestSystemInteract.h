#pragma once

#include "CoreMinimal.h"
#include "IQuestSystemInteract.generated.h"

UINTERFACE(MinimalAPI)
class UQuestSystemInteract: public UInterface
{
	GENERATED_BODY()
};

class SOLVI_API IQuestSystemInteract
{
	GENERATED_BODY()

public:
	// Declare pure virtual functions that implementing classes must override.
	virtual void LookAt() const = 0;
	virtual FString InteractWith() = 0;
};