// Fill out your copyright notice in the Description page of Project Settings.


#include "Switchboard.h"
#include "math.h"
#include "SwitchboardPawn.h"
#include "InteractSystem/PlayerGhostController.h"

USwitchboard::USwitchboard():UUtilityPossessableComponent()
{
    currRoomSelection = 0;
    maxRooms = 3;
    bIsNewInput = true;
}

void USwitchboard::IncrementCurrRoomSelection(float val)
{
    Cast<ASwitchboardPawn>(GetOwner())->RoomText[currRoomSelection]->SetTextRenderColor(FColor::Black);
    currRoomSelection += copysign(1, val);
    if(currRoomSelection > maxRooms-1)
    {
        currRoomSelection = 0;
    }
    else if(currRoomSelection < 0)
    {
        currRoomSelection = maxRooms-1;
    }
    Cast<ASwitchboardPawn>(GetOwner())->RoomText[currRoomSelection]->SetTextRenderColor(FColor::Emerald);
    bIsNewInput = false;
}

void USwitchboard::MoveRightAxis_Implementation(float Axis)
{
    if(Axis != 0.0f && bIsNewInput)
    {
        IncrementCurrRoomSelection(Axis);
    }
    if(Axis == 0.0f && !bIsNewInput)
    {
        bIsNewInput = true;
    }
}

void USwitchboard::MoveButton_Implementation()
{
    APlayerGhostController* PlayerConRef = GetCurrentPlayer();
    if(PlayerConRef != nullptr && Network[currRoomSelection] != nullptr)
    {
        PlayerConRef->Possess(Network[currRoomSelection]);
    }
}
