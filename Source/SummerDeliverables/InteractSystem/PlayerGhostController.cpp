// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGhostController.h"

#include <string>

#include "PossessablePawn.h"
#include "../PlayerPawn.h"
#include "Blueprint/UserWidget.h"
#include "Chaos/ChaosPerfTest.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"


APlayerGhostController::APlayerGhostController() :APlayerController()
{
    MaxStamina = 100.0f;
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerPawn_BP"));
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Ghosty"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        PawnClass = PlayerPawnBPClass.Class;
    }
    StaminaRegen = 0.5f;
    LivingTime = 0.0f;
}

bool APlayerGhostController::SetStamina(float delta_stamina, bool b_is_relative)
{
    if (b_is_relative)
    {
        CurrentStamina = FMath::Max( FMath::Min(CurrentStamina + delta_stamina, MaxStamina), 0.0f);
    }
    else
    {
        CurrentStamina = delta_stamina;
    }
    if (CurrentStamina == 0.0f)
    {
        return true;
    }
    return false;
}

float APlayerGhostController::GetStaminaPercent() const
{
    return CurrentStamina / MaxStamina;
}

void APlayerGhostController::Tick(float DeltaSeconds)
{
    SetStamina(StaminaRegen*DeltaSeconds);
    LivingTime += DeltaSeconds;
    if (LivingTime > 2.0f)
    {
        //std::string msg = "PlayerStamina = " + std::to_string(CurrentStamina);
        //SCREENMSG(msg.c_str());
        LivingTime -= 2.0f;
    }
    
}

APlayerPawn* APlayerGhostController::CreatePlayerPawn(FVector spawn_location) 
{
    FActorSpawnParameters params = *new FActorSpawnParameters();
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Cast<APlayerPawn>(GetWorld()->SpawnActor(PawnClass, &spawn_location, 0, params));
}

void APlayerGhostController::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentStamina = MaxStamina;
    //if(!HUDClass)
    //    SCREENMSG("no hud :(");
    /*PlayerHUD = CreateWidget(this, HUDClass);
    if(PlayerHUD != nullptr)
    {
        SCREENMSG("HUD thingy");
        PlayerHUD->AddToPlayerScreen();
    }*/
    SCREENMSG("BeginPlay Initialized a PlayerGhostController");
}

void APlayerGhostController::OnUnPossess()
{
    LastPossessedPawn = Cast<APossessablePawn>(GetPawn());
}


