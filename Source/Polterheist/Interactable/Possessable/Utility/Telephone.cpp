// Fill out your copyright notice in the Description page of Project Settings.


#include "Telephone.h"
#include "InteractSystem/PlayerGhostController.h"

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



