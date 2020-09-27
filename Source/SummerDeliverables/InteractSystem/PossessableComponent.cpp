// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"

#include "AIController.h"
#include "PlayerGhostController.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "SummerDeliverables/PlayerPawn.h"

UPossesableComponent::UPossesableComponent():UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    CurrentCooldown = 0.0f;
    bIsDrainingStamina = true;
}

float UPossesableComponent::GetStamina() const
{
    APossessablePawn* owner = Cast<APossessablePawn>(GetOwner());
    if (owner->IsPossessing())
    {
        AController* controller = owner->GetController();
        if (controller != nullptr)
        {
            APlayerGhostController* ghost_controller = Cast<APlayerGhostController>(controller);
            if (ghost_controller)
                return ghost_controller->GetStamina();
            else 
                SCREENMSG("Controller isn't of APlayerGhostController");
        }
    }
    return 0.0f;
}

bool UPossesableComponent::SetStamina(float stamina_drain, bool b_is_relative)
{
    if (Cast<APossessablePawn>(GetOwner())->IsPossessing())
        return Cast<APlayerGhostController>(Cast<APawn>(GetOwner())->GetController())->SetStamina(stamina_drain, b_is_relative);
    return false;
}


void UPossesableComponent::Eject()
{
    APossessablePawn * tmp = Cast<APossessablePawn>(GetOwner());
    tmp->EndPossession();
}

void UPossesableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(CurrentCooldown>0)
        CurrentCooldown-=DeltaTime;
    else
        CurrentCooldown = 0;
}
