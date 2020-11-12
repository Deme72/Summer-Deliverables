// Copyright Epic Games, Inc. All Rights Reserved.

#include "SummerDeliverablesGameMode.h"

#include "DefinedDebugHelpers.h"
#include "SummerDeliverablesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "InteractSystem/PlayerGhostController.h"

ASummerDeliverablesGameMode::ASummerDeliverablesGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerPawn_BP"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Ghosty"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerGhostController::StaticClass();
}

void ASummerDeliverablesGameMode::BeginPlay()
{
	Super::BeginPlay();

	SCREENMSG("BeginPlay intialized gamemode as SummerDeliverablesGameMode.cpp");
	///This timer handles when to call SpawnStamina()
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASummerDeliverablesGameMode::SpawnStamina,
                                    FMath::RandRange(SpawnTimerMin, SpawnTimerMax), true);
}

void ASummerDeliverablesGameMode::SpawnStamina()
{
	float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
	float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

	FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	if(FMath::RandBool())
	{
		GetWorld()->SpawnActor(PlayerStamina, &SpawnPosition, &SpawnRotation);
	}
	else
	{
		GetWorld()->SpawnActor(TeamStamina, &SpawnPosition, &SpawnRotation);
	}
    
}