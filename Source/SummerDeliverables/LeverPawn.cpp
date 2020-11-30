// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverPawn.h"

ALeverPawn::ALeverPawn()
{
    StartLinePos1 = CreateDefaultSubobject<USceneComponent>(TEXT("StartLinePos1"));
    StartLinePos2 = CreateDefaultSubobject<USceneComponent>(TEXT("StartLinePos2"));
    EndLinePos1 = CreateDefaultSubobject<USceneComponent>(TEXT("EndLinePos1"));
    EndLinePos2 = CreateDefaultSubobject<USceneComponent>(TEXT("EndLinePos2"));
    StartLinePos1->SetupAttachment(GetRootComponent());
    StartLinePos2->SetupAttachment(GetRootComponent());
    EndLinePos1->SetupAttachment(GetRootComponent());
    EndLinePos2->SetupAttachment(GetRootComponent());
}
