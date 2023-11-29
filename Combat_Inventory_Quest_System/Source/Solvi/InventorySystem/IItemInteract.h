#pragma once

#include "CoreMinimal.h"
#include "IItemInteract.generated.h"


class ASolviModularCharacter;

UINTERFACE(MinimalAPI)
class UItemInteract: public UInterface
{
	GENERATED_BODY()
};

class SOLVI_API IItemInteract
{
	GENERATED_BODY()

public:
	// Declare pure virtual functions that implementing classes must override.
	virtual FText LookAt() const = 0;
	virtual void InteractWith(ASolviModularCharacter* PlayerCharacter) = 0;
	virtual void StopLookAt() const = 0;
};