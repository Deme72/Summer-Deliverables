// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"

UPossesableComponent::UPossesableComponent()
{
    CurrentCooldown = 0.0f;
}

void UPossesableComponent::OnInteract_Implementation()
{
   APossessablePawn * Owner=  Cast<APossessablePawn>(GetOwner());
   check(Owner);
   bInUse=true;
}

void UPossesableComponent::EndInteract_Implementation()
{
    bInUse=false;
    APossessablePawn * Owner=  Cast<APossessablePawn>(GetOwner());
    check(Owner);
    if(Owner->IsPawnControlled())
        Owner->EndPossession();
}

void UPossesableComponent::TickComponent(float deltaTime)
{
    if(CurrentCooldown>0)
        CurrentCooldown-=deltaTime;
    else
        CurrentCooldown = 0;
}