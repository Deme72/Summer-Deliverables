// Fill out your copyright notice in the Description page of Project Settings.


#include "PossesableComponent.h"

void UPossesableComponent::OnInteract_Implementation()
{
   bInUse=true;
    
    
}

void UPossesableComponent::EndInteract_Implementation()
{
    bInUse=false;
}
