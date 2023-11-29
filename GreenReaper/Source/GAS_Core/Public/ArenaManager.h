#pragma once

#include "CoreMinimal.h"
#include "Arena.h"
#include "GameFramework/Actor.h"
#include "ArenaManager.generated.h"

UCLASS(Blueprintable)
class GAS_CORE_API AArenaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaManager();
	
	/* Arenas get grabbed on begin play*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AArena*> Arenas;
	UPROPERTY(BlueprintReadWrite)
	AArena* CurrentArena;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

private:
	

};
