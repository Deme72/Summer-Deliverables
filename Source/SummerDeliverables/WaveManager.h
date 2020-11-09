// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"
UENUM(BlueprintType)
enum EEnemyType
{
	CriminalTier1,
	CriminalTier2,
	CriminalTier3,
	LeaderTier1,
	LeaderTier2,
	LeaderTier3,
	GhostHunterTier1,
	GhostHunterTier2,
	GhostHunterTier3
};

USTRUCT(BlueprintType)
struct FWave {
	GENERATED_BODY()
	//a list of enum objects for each enemy we want to spawn
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<TEnumAsByte<EEnemyType>> SpawnedObjects;
};

UCLASS()
class SUMMERDELIVERABLES_API AWaveManager : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AWaveManager();

	/// Class to use for the tier 1 criminal enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> CriminalTier1Type;
	/// Class to use for the tier 2 criminal enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> CriminalTier2Type;
	/// Class to use for the tier 3 criminal enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> CriminalTier3Type;
	/// Class to use for the follower/minion enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> MinionType;
	/// Class to use for the tier 1 leader enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> LeaderTier1Type;
	/// Class to use for the tier 2 leader enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> LeaderTier2Type;
	/// Class to use for the tier 3 leader enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> LeaderTier3Type;
	/// Class to use for the tier 1 ghost hunter enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> GhostHunterTier1Type;
	/// Class to use for the tier 2 ghost hunter enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> GhostHunterTier2Type;
	/// Class to use for the tier 3 ghost hunter enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClassReferences")
	class TSubclassOf<ABaseEnemyCharacter> GhostHunterTier3Type;
	/// All waves that will be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWave> Waves;
	//the time between waves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenWaves = 30;
	//the entrance that enemies spawn at
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor * Entrance;
	//the current wave
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int CurrentWave = 0;
	//time until the next wave
	float TimeUntilNextWave;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// Spawns this wave, returns true if the wave was successfully spawned, otherwise returns false
	virtual bool SpawnWave(int number);
	/// Attempts to spawn an enemy at the target location, returns true if they were successfully spawned
	virtual APawn* SpawnEnemy(TSubclassOf<ABaseEnemyCharacter> EnemyToSpawn, FVector TargetLocation, float LocationSpread, FRotator TargetRotation = FRotator::ZeroRotator);
	/// Checks if there are waves remaining
	bool AreWavesRemaining() {return CurrentWave < Waves.Num();}

	///This function counts how many enemies exist for use by the game mode to check the win condition
	int NumEnemies();
};
