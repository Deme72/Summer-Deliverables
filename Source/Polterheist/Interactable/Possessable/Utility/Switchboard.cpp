// Fill out your copyright notice in the Description page of Project Settings.


#include "Switchboard.h"
#include "SwitchboardPawn.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Movement/Movement.h"

USwitchboard::USwitchboard():UUtilityPossessableComponent()
{
    CurrRoomSelection = 0;
    MaxRooms = 3;
    bIsNewInput = true;
}

void USwitchboard::IncrementCurrRoomSelection(float val)
{
    Cast<ASwitchboardPawn>(GetOwner())->RoomText[CurrRoomSelection]->SetTextRenderColor(FColor::Black);
    CurrRoomSelection += copysign(1, val);
    if(CurrRoomSelection > MaxRooms-1)
    {
        CurrRoomSelection = 0;
    }
    else if(CurrRoomSelection < 0)
    {
        CurrRoomSelection = MaxRooms-1;
    }
    Cast<ASwitchboardPawn>(GetOwner())->RoomText[CurrRoomSelection]->SetTextRenderColor(FColor::Emerald);
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
    if(PlayerConRef != nullptr && Network[CurrRoomSelection] != nullptr)
    {
        PlayerConRef->Possess(Network[CurrRoomSelection]);
    }
}
