// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"

UPossesableComponent::UPossesableComponent():UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    CurrentCooldown = 0.0f;
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
