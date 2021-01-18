// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    bInUse = false;
}

void UInteractableComponent::OnInteractInternal()
{
    bInUse = true;
    OnInteract();
}

void UInteractableComponent::EndInteractInternal()
{
    bInUse = false;
    EndInteract();
}

