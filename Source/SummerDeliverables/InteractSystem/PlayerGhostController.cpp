// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGhostController.h"
#include "PossessablePawn.h"
#include "../PlayerPawn.h"
#include "Chaos/ChaosPerfTest.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"


APlayerGhostController::APlayerGhostController() :APlayerController()
{
    MaxStamina = 100.0f;
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerPawn_BP"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        PawnClass = PlayerPawnBPClass.Class;
    }
}

bool APlayerGhostController::SetStamina(float stamina_drain, bool b_is_relative)
{
    if (b_is_relative)
    {
        CurrentStamina = FMath::Max( FMath::Min(CurrentStamina - stamina_drain, MaxStamina), 0.0f);
    }
    else
    {
        CurrentStamina = stamina_drain;
    }
    if (CurrentStamina == 0.0f)
    {
        return true;
    }
    return false;
}

APlayerPawn* APlayerGhostController::CreatePlayerPawn(const FVector spawn_location) const
{
    FActorSpawnParameters params = *new FActorSpawnParameters();
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    return Cast<APlayerPawn>(GetWorld()->SpawnActor(APlayerPawn::StaticClass(), &spawn_location, 0, params));
}

void APlayerGhostController::BeginPlay()
{
    MaxStamina = 200.0f;
    CurrentStamina = MaxStamina;
    SCREENMSG("BeginPlay Initialized a PlayerGhostController");
}


