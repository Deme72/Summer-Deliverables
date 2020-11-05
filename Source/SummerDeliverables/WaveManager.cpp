// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*
	if((CriminalTier1Type) && !IsValid(CriminalTier1))
	{
		CriminalTier1 = NewObject<ABaseEnemyCharacter>(this , CriminalTier1Type);
	}
	*/
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnWave(0);
	CurrentWave = 1;
	TimeUntilNextWave = TimeBetweenWaves;
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeUntilNextWave -= DeltaTime;
	if (TimeUntilNextWave <= 0)
	{
		if (SpawnWave(CurrentWave))
		{
			CurrentWave += 1;
			TimeUntilNextWave = TimeBetweenWaves;
		}
		else
		{
			//victory conditions here
			UE_LOG(LogTemp, Warning, TEXT("No Spawns Left!"),5);
			TimeUntilNextWave = 9999;
		}
	}
}

// Spawn waves
bool AWaveManager::SpawnWave(int number)
{
	if (number >= Waves.Num())
		return false;
	FWave wave = Waves[number];
	class TSubclassOf<ABaseEnemyCharacter> EnemyToSpawn;
	for (int x = 0; x < wave.SpawnedObjects.Num(); x++)
	{
		//switch to get the appropriate enemy reference
		switch(wave.SpawnedObjects[x])
		{
			case EEnemyType::CriminalTier1:
				EnemyToSpawn = CriminalTier1Type;
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Unsupported Enemy Spawned!"),5);
				return false;
		}
		if (SpawnEnemy(EnemyToSpawn,Entrance->GetActorLocation(),200,Entrance->GetActorRotation()) == nullptr)
			UE_LOG(LogTemp, Warning, TEXT("Could not spawn enemy successfully!"),5);			
	}
	return true;
}

APawn* AWaveManager::SpawnEnemy(TSubclassOf<ABaseEnemyCharacter> EnemyToSpawn, FVector TargetLocation, float LocationSpread, FRotator TargetRotation)
{
	//make 25 attempts to spawn the enemy
	for (int spawnAttempts = 0; spawnAttempts < 25; spawnAttempts++)
	{
		TargetLocation += FVector(FMath::RandRange(-LocationSpread,LocationSpread),FMath::RandRange(-LocationSpread,LocationSpread),0);
		APawn* unitRef = UAIBlueprintHelperLibrary::SpawnAIFromClass(this,EnemyToSpawn,NULL,TargetLocation,TargetRotation);
		if (unitRef != nullptr)
			return unitRef;
	}
	return nullptr;
}


