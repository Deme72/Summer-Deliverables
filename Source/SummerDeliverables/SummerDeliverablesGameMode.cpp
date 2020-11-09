// Copyright Epic Games, Inc. All Rights Reserved.

#include "SummerDeliverablesGameMode.h"

#include "DefinedDebugHelpers.h"
#include "SummerDeliverablesCharacter.h"
#include "WaveManager.h"
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
	//Blueprint'/Game/Blueprints/BP_ActualController.BP_ActualController'
	//static ConstructorHelpers::FClassFinder<APawn> GhostControllerBPClass(TEXT("/Game/Blueprints/BP_ActualController"));
	//if (GhostControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = GhostControllerBPClass.Class;
	//}
}

void ASummerDeliverablesGameMode::BeginPlay()
{
	Super::BeginPlay();

	SCREENMSG("BeginPlay intialized gamemode as SummerDeliverablesGameMode.cpp");
	///This timer handles when to call SpawnStamina()
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASummerDeliverablesGameMode::SpawnStamina,
                                    FMath::RandRange(SpawnTimerMin, SpawnTimerMax), true);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundActors);
	Enemies_Remaining = 0;
	if(FoundActors.Num() != 0)
		Enemies_Remaining = Cast<AWaveManager>(FoundActors[0])->NumEnemies();
	TArray<AActor*> FoundTreasure;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TreasureRef, FoundTreasure);
	Treasure_Remaining = FoundTreasure.Num();
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

void ASummerDeliverablesGameMode::CheckWinCon(float value)
{
	Enemies_Remaining--;
	Treasure_Remaining -= value;
	V_LOGI("Enemies", Enemies_Remaining);
	V_LOGI("Treasure", Treasure_Remaining);
	if(Treasure_Remaining <= 0)
	{
		//Lose
		V_LOG("You Lose!");
	}
	else if( Enemies_Remaining <= 0)
	{
		//Win
		V_LOG("You Win!");
	}
}
