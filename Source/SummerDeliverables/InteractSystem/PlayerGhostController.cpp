// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGhostController.h"
#include "PossessablePawn.h"
#include "../PlayerPawn.h"

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

APlayerPawn* APlayerGhostController::CreatePlayerPawn(FVector spawn_location) const
{
    return dynamic_cast<APlayerPawn*>(GetWorld()->SpawnActor(PlayerPawn, &spawn_location));
}


