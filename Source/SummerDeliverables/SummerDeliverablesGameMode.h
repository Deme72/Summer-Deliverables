// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SummerDeliverablesGameMode.generated.h"

UCLASS(minimalapi)
class ASummerDeliverablesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASummerDeliverablesGameMode();

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
	float Spawn_Z = 500.0f;

	///The x max boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float Spawn_X_Max;

	///the x min boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float Spawn_X_Min;

	///the y max boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float Spawn_Y_Max;

	///the y min boundary of the spawn location
	UPROPERTY(EditAnywhere)
	float Spawn_Y_Min;

	// ===================
	// ===== METHODS =====
	// ===================
private:
protected:
public:
	///This function spawns the stamina pickups
	void SpawnStamina();
};



