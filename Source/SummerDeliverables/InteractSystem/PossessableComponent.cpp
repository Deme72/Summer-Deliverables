// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"
#include "PlayerGhostController.h"

UPossesableComponent::UPossesableComponent()
{
    CurrentCooldown = 0.0f;
    bIsDrainingStamina = true;
}

float UPossesableComponent::GetStamina() const
{
    return Cast<APlayerGhostController>(Cast<APawn>(GetOwner())->GetController())->GetStamina();
}

bool UPossesableComponent::SetStamina(float stamina_drain, bool b_is_relative)
{
    return Cast<APlayerGhostController>(Cast<APawn>(GetOwner())->GetController())->SetStamina(stamina_drain, b_is_relative);
}


void UPossesableComponent::Eject()
{
    APossessablePawn * tmp = Cast<APossessablePawn>(GetOwner());
    tmp->EndPossession();
}

void UPossesableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if(CurrentCooldown>0)
        CurrentCooldown-=DeltaTime;
    else
        CurrentCooldown = 0;
}
