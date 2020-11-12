// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverPawn.h"

ALeverPawn::ALeverPawn()
{
    StartLinepos1 = CreateDefaultSubobject<USceneComponent>(TEXT("StartLinePos1"));
    StartLinepos2 = CreateDefaultSubobject<USceneComponent>(TEXT("StartLinePos2"));
    EndLinepos1 = CreateDefaultSubobject<USceneComponent>(TEXT("EndLinePos1"));
    EndLinepos2 = CreateDefaultSubobject<USceneComponent>(TEXT("EndLinePos2"));
    StartLinepos1->SetupAttachment(GetRootComponent());
    StartLinepos2->SetupAttachment(GetRootComponent());
    EndLinepos1->SetupAttachment(GetRootComponent());
    EndLinepos2->SetupAttachment(GetRootComponent());
}
