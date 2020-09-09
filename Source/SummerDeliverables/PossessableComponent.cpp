// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"

UPossesableComponent::UPossesableComponent()
{
    CurrentCooldown = 0.0f;
}

void UPossesableComponent::OnInteract_Implementation()
{
   bInUse=true; //TODO: BAD GETS OVERWRITTEN IN BLUEPRINTS
}

void UPossesableComponent::EndInteract_Implementation()
{
    bInUse=false;//TODO: BAD GETS OVERWRITTEN IN BLUEPRINTS
}

void UPossesableComponent::TickComponent(float deltaTime)
{
    if(CurrentCooldown>0)
        CurrentCooldown-=deltaTime;
    else
        CurrentCooldown = 0;
}