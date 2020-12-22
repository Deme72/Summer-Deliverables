// Fill out your copyright notice in the Description page of Project Settings.


#include "Telephone.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Movement/Movement.h"

UTelephone::UTelephone():UUtilityPossessableComponent()
{
}

///Possesses switchboardPawn
void UTelephone::MoveButton_Implementation()
{
    APlayerGhostController* PlayerConRef = GetCurrentPlayer();
    if(PlayerConRef != nullptr && ConnectedSwitchboard != nullptr)
        PlayerConRef->Possess(ConnectedSwitchboard);
    EndInteractInternal();
}



