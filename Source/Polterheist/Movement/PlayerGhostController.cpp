// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGhostController.h"

#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Core/Core.h"
#include "Polterheist/External/DefinedDebugHelpers.h"
#include "Polterheist/HUD/HUDWidget.h"

APlayerGhostController::APlayerGhostController() :APlayerController()
{
    MaxStamina = 100.0f;

    // TODO: either expose this to be set in the editor OR update the directory path
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Polterheist/Character/Player/BP_Ghosty"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        PawnClass = PlayerPawnBPClass.Class;
    }
    StaminaRegen = 0.5f;
    LivingTime = 0.0f;
    //PlayerCameraManagerClass = AGhostCameraManager::StaticClass();

    //PlayerStatistics = FPlayerGhostStatistics{};

    Invisibility = false;
    InvisTimer = 20.0f;
}

bool APlayerGhostController::SetStamina(float delta_stamina, bool b_is_relative)
{
    if (Invisibility == false)
    {
        if (b_is_relative)
        {
            CurrentStamina = FMath::Max( FMath::Min(CurrentStamina + delta_stamina, MaxStamina), 0.0f);
        }
        else
        {
            CurrentStamina = delta_stamina;
        }
    }

    // Update the stamina widget
    if (HUDOverlay)
    {
        HUDOverlay->UpdateStaminaBar(GetStaminaPercent_C());
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

float APlayerGhostController::GetStaminaPercent_C() const
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
    if (Invisibility)
    {
        InvisTimer-=DeltaSeconds;
        if (InvisTimer <= 0.0f)
        {
            Invisibility = false;
        }
    }
    
}

APlayerPawn* APlayerGhostController::CreatePlayerPawn(FVector spawn_location) 
{
    FActorSpawnParameters params = *new FActorSpawnParameters();
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn this player's HUD
    if (HUDOverlayAsset)
    {
        HUDOverlay = Cast<UHUDWidget>(CreateWidget<UUserWidget>(this, HUDOverlayAsset));
        if (HUDOverlay)
        {
            HUDOverlay->AddToViewport();
            HUDOverlay->SetVisibility(ESlateVisibility::Visible);
            V_LOG("PC Created HUD widget");
        }
    }

    return Cast<APlayerPawn>(GetWorld()->SpawnActor(PawnClass, &spawn_location, 0, params));
}

void APlayerGhostController::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentStamina = MaxStamina;
}

void APlayerGhostController::OnUnPossess()
{
    LastPossessedPawn = Cast<APossessablePawn>(GetPawn());
}

void APlayerGhostController::SetInvisibility(bool value)
{
    Invisibility = value;
}



