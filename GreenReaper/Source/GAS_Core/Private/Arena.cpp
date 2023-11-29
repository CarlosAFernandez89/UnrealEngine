// Fill out your copyright notice in the Description page of Project Settings.
#include "Arena.h"

#include "RPGCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();

	//Create Overlap Events for BoxComponent
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AArena::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AArena::OnOverlapEnd);


	for (FEnemyStruct& Enemy : EnemyType)
	{
		const int Count = Enemy.SpawnInfo.NumberOfEnemies;
		Enemy.ArrayHandleStruct.TDelegate.AddDefaulted(Count);
		Enemy.ArrayHandleStruct.THandle.AddDefaulted(Count);
		Enemy.SpawnInfo.InRate =
			Enemy.SpawnInfo.TimeBetweenSpawns
			* Enemy.SpawnInfo.NumberOfEnemies
			+ InitialSpawnDelayTime;
		

	}
}

// Sets default values
AArena::AArena(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bSpawnEnemies = true;
	
	SceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(SceneComponent);
	
	BillboardComponent = ObjectInitializer.CreateDefaultSubobject<UBillboardComponent>(this,TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(SceneComponent);


}

void AArena::SpawnStart()
{
	for (FEnemyStruct& Enemy : EnemyType)
	{
		if(Enemy.SpawnInfo.SpawnRadius == 0.0f)
		{
			SpawnEnemies(Enemy);
		}
		else
		{
			const float EnemyRadius = Enemy.Enemy.GetDefaultObject()->GetCapsuleComponent()->GetScaledCapsuleRadius();
			const float Ratio = EnemyRadius / (Enemy.SpawnInfo.SpawnRadius - EnemyRadius);
			const float Angle = FMath::Abs(FMath::Asin(Ratio) * 180.f)/ PI;
			const float NumberOfEnemiesAllowed = 360.f / (2 * FMath::Floor(Angle));
			
			if(NumberOfEnemiesAllowed < Enemy.SpawnInfo.NumberOfEnemies)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
					FString::Printf(TEXT("NUMBER OF ENEMIES ALLOWED: %f"), NumberOfEnemiesAllowed));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			FString::Printf(TEXT("SPAWN RADIUS IS TOO SMALL. STOP CRASHING THE GAME :MADGE: !")));
				return;
			}
			else
			{
				SpawnEnemiesWithRadius(Enemy);
			}
		}
	}
}


void AArena::SpawnEnemiesWithRadius(FEnemyStruct& EnemyStruct)
{
	for (int i = 0; i < EnemyStruct.SpawnInfo.NumberOfEnemies; i++)
	{
		if(EnemyStruct.SpawnInfo.SpawnLocationObjectRef != nullptr)
		{
			FVector SpawnLocation = EnemyStruct.SpawnInfo.SpawnLocationObjectRef->GetActorLocation();
			const FRotator SpawnRotation = EnemyStruct.SpawnInfo.SpawnLocationObjectRef->GetActorRotation();

			for(int j = 0; j < EnemyStruct.SpawnInfo.NumberOfEnemies; j++)
			{
				const float RandomX = FMath::FRandRange(-EnemyStruct.SpawnInfo.SpawnRadius, EnemyStruct.SpawnInfo.SpawnRadius);
				const float RandomY = FMath::FRandRange(-EnemyStruct.SpawnInfo.SpawnRadius, EnemyStruct.SpawnInfo.SpawnRadius);


				SpawnLocation.X += RandomX;
				SpawnLocation.Y += RandomY;

				const TSubclassOf<ACharacter> EnTypeTemp = EnemyStruct.Enemy;
				EnemyStruct.ArrayHandleStruct.TDelegate[j].BindUObject(this, &AArena::Spawn, SpawnLocation, SpawnRotation, EnTypeTemp);
				GetWorld()->GetTimerManager().SetTimer(
					EnemyStruct.ArrayHandleStruct.THandle[j],
					EnemyStruct.ArrayHandleStruct.TDelegate[j],
					0.1f + EnemyStruct.SpawnInfo.SpawnDelay,
					false);

			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
						FString::Printf(
							TEXT("NO SPAWN LOCATION OBJECT REF SELECTED")));
		}

	}



}

void AArena::SpawnEnemies(FEnemyStruct& EnemyStruct)
{
	for (int i = 0; i < EnemyStruct.SpawnInfo.NumberOfEnemies; i++)
	{
		if(EnemyStruct.SpawnInfo.SpawnLocationObjectRef != nullptr)
		{
			const FVector SpawnLocation = EnemyStruct.SpawnInfo.SpawnLocationObjectRef->GetActorLocation();
			const FRotator SpawnRotation = EnemyStruct.SpawnInfo.SpawnLocationObjectRef->GetActorRotation();

			for(int j = 0; j < EnemyStruct.ArrayHandleStruct.TDelegate.Num(); j++)
			{
				const TSubclassOf<ACharacter> EnTypeTemp = EnemyStruct.Enemy;
				EnemyStruct.ArrayHandleStruct.TDelegate[j].BindUObject(this, &AArena::Spawn, SpawnLocation, SpawnRotation, EnTypeTemp);
				GetWorld()->GetTimerManager().SetTimer(
					EnemyStruct.ArrayHandleStruct.THandle[j],
					EnemyStruct.ArrayHandleStruct.TDelegate[j],
					EnemyStruct.SpawnInfo.TimeBetweenSpawns * j + 0.1f + EnemyStruct.SpawnInfo.SpawnDelay,
					false);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
						FString::Printf(
							TEXT("NO SPAWN LOCATION OBJECT REF SELECTED")));
		}

	}

}

void AArena::Spawn(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ACharacter> Type)
{
	UClass* SpawnClass = Type->GetAuthoritativeClass();
	const AActor* SpawnActor = GetWorld()->SpawnActor(
						SpawnClass,
						&SpawnLocation,
						&SpawnRotation
						);

	if(SpawnActor != nullptr)
	{
		OnSpawnSucceed_Implementation(SpawnLocation, SpawnRotation, Type);
	}
	
}





// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSpawnEnemies == false)
	{
		CheckArenaEnemiesAlive_Implementation();
	}

}

TArray<AActor*> AArena::GetAllEnemiesInArena_Implementation()
{
	TArray<AActor*> Array;
	BoxComponent->GetOverlappingActors(Array,ARPGCharacterBase::StaticClass());
	for(int i = 0; i < Array.Num(); i++)
	{
		if(Cast<ARPGCharacterBase>(Array[i])->bIsPlayer)
		{
			Array.RemoveAt(i);
		}
	}
	return Array;
}


void AArena::CheckArenaEnemiesAlive_Implementation()
{
}


void AArena::OnArenaStart_Implementation()
{

	// Check to not double spawn when coming back into the collider
	SpawnStart();

	
}



void AArena::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AArena::OnOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
}


void AArena::OnArenaReset_Implementation()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}