// Fill out your copyright notice in the Description page of Project Settings.


#include "SolviAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Solvi/Character/SolviModularCharacter.h"
#include "Solvi/Character/SolviCharacterMovementComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(SolviAnimInstance)

USolviAnimInstance::USolviAnimInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void USolviAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult USolviAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void USolviAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void USolviAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ASolviModularCharacter* Character = Cast<ASolviModularCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	USolviCharacterMovementComponent* CharMoveComp = CastChecked<USolviCharacterMovementComponent>(Character->GetCharacterMovement());
	const FSolviCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}
