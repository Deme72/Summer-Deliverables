// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PolterheistGameMode.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API APolterheistGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	APolterheistGameMode();

	virtual void BeginPlay() override;

	/// A reference to the stamina pickup actor that only effects the player picking it up
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerStamina;

	///A reference to the stamina pickup actor that effects all players
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TeamStamina;

	///The shortest amount of time in seconds that a stamina pickup will wait to spawn
	UPROPERTY(EditAnywhere)
	float SpawnTimerMin = 2;

	///The longest amount of time in seconds that a stamina pickup will wait to spawn
	UPROPERTY(EditAnywhere)
	float SpawnTimerMax = 2.5;

	///The spawn height of the stamina pickups
	float SpawnZ = 500.0f;

	///The x max boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float SpawnXMax;

	///the x min boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float SpawnXMin;

	///the y max boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float SpawnYMax;

	///the y min boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float SpawnYMin;
	
	//Reference for Treasure
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TreasureRef;
	
	///How many enemies are left to scare
	int EnemiesRemaining;

	///Keeps track of how much treasure is left
	int TreasureRemaining;
	
	// ===================
	// ===== METHODS =====
	// =================== 
	private:
	protected:
	public:
	///This function spawns the stamina pickups
	void SpawnStamina();

	///The enemies will call this on death to decrement actors & Handles win condition
	void CheckWinCon(float value = 0);
};
