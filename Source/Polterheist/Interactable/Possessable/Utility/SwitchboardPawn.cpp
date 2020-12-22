// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchboardPawn.h"
#include "Polterheist/Interactable/Interactable.h"

ASwitchboardPawn::ASwitchboardPawn():APossessablePawn()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    
    RoomText.Add(CreateDefaultSubobject<UTextRenderComponent>(TEXT("RoomText1")));
    RoomText.Add(CreateDefaultSubobject<UTextRenderComponent>(TEXT("RoomText2")));
    RoomText.Add(CreateDefaultSubobject<UTextRenderComponent>(TEXT("RoomText3")));
    for(auto& text: RoomText)
    {
        text->SetupAttachment(RootComponent);
        text->SetTextRenderColor(FColor::Black);
    }
    RoomText[0]->SetTextRenderColor(FColor::Emerald);
}

void ASwitchboardPawn::EndPossession()
{
    APlayerGhostController* PlayerConRef = Cast<APlayerGhostController>(GetOwner());
    if(PlayerConRef != nullptr && PlayerConRef->LastPossessedPawn != nullptr)
        PlayerConRef->Possess(PlayerConRef->LastPossessedPawn);
}
