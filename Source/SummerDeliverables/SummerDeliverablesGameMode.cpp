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

	//SCREENMSG("BeginPlay intialized gamemode as SummerDeliverablesGameMode.cpp");
	///This timer handles when to call SpawnStamina()
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASummerDeliverablesGameMode::SpawnStamina,
                                    FMath::RandRange(SpawnTimerMin, SpawnTimerMax), true);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundActors);
	EnemiesRemaining = 0;
	if(FoundActors.Num() != 0)
		EnemiesRemaining = Cast<AWaveManager>(FoundActors[0])->NumEnemies();
	TArray<AActor*> FoundTreasure;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TreasureRef, FoundTreasure);
	TreasureRemaining = FoundTreasure.Num();
}

void ASummerDeliverablesGameMode::SpawnStamina()
{
	float RandX = FMath::RandRange(SpawnXMin, SpawnXMax);
	float RandY = FMath::RandRange(SpawnYMin, SpawnYMax);

	FVector SpawnPosition = FVector(RandX, RandY, SpawnZ);
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
	EnemiesRemaining--;
	TreasureRemaining -= value;
	V_LOGI("Enemies", EnemiesRemaining);
	V_LOGI("Treasure", TreasureRemaining);
	if(TreasureRemaining <= 0)
	{
		//Lose
		V_LOG("You Lose!");
	}
	else if( EnemiesRemaining <= 0)
	{
		//Win
		V_LOG("You Win!");
	}
}
