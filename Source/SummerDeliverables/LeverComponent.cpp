// Fill out your copyright notice in the Description page of Project Settings.

#include "LeverComponent.h"
#include "SummerDeliverables/InteractSystem/PossessablePawn.h"

#include <corecrt_io.h>



#include "DrawDebugHelpers.h"
#include "LeverPawn.h"

ULeverComponent::ULeverComponent()
{
    bOpen = false;
    SlideBack = 0.0f;
    BeginRot =0.0f;
    TimerTillReset = 0.0f;
    bControlling = false;
    Lever_ref = Cast<ALeverPawn>(GetOwner());
    MaxYaw = 0.0f;
}

void ULeverComponent::BeginPlay()
{
    Super::BeginPlay();
    if (Lever_ref)
    {
        EndLinePos1 = Lever_ref->EndLinePos1->GetComponentLocation();
        EndLinePos2 = Lever_ref->EndLinePos2->GetComponentLocation();
        BeginRot = Lever_ref->GetActorRotation().Yaw;
    }
}

void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    MyDeltaTime = DeltaTime;
    if (bControlling)
    {
        DrawDebugLine(GetOwner()->GetWorld(),EndLinePos1,EndLinePos2,FColor::Red,false,0.0, 0, 5.0);
        DrawDebugLine(GetOwner()->GetWorld(),Lever_ref->StartLinePos1->GetComponentLocation(),Lever_ref->StartLinePos2->GetComponentLocation(),FColor::Cyan,false,0.0,0,5.0);
    }
    if (BeginRot >= Lever_ref->GetActorRotation().Yaw)
    {
        SlideBack = 0.0;
    }
    else
    {
        Lever_ref->AddActorLocalRotation(FRotator(0.0,SlideBack*MyDeltaTime,0.0));
    }
    if (bOpen)
    {
        if (TimerTillReset <= 0.0)
        {
            bOpen = false;
            TimerTillReset =0.0f;
            SlideBack = -10.0f;
        }
        else
        {
            TimerTillReset = TimerTillReset - (MyDeltaTime*TimeScale);
        }
    }
}

void ULeverComponent::ScareButton_Implementation()
{
    if (bOpen == false)
    {
        SlideBack = 0.0f;
        if (Lever_ref->GetActorRotation().Yaw >= BeginRot+180.0)
        {
            Lever_ref->SetActorRotation(FRotator(0.0,BeginRot+180.0,0.0));
            bOpen = true;
            TimerTillReset = 7.0f;
            TimeScale = 1.0f;
        }
        else
        {
           Lever_ref->AddActorLocalRotation(FRotator(0.0,MyDeltaTime*200.0,0.0));
           //GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,FString::Printf(TEXT("%f"),Lever_ref->GetActorRotation().Yaw));
           //GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,FString::Printf(TEXT("%f"),begin_rot+180.0));
        }
    }
}

void ULeverComponent::ScareButtonRelease_Implementation()
{
    if (bOpen == false)
    {
        SlideBack = -10.0f;
    }
}

void ULeverComponent::OnInteract_Implementation()
{
    bControlling = true;
}

void ULeverComponent::EndInteract_Implementation()
{
    bControlling = false;
}

bool ULeverComponent::GetOpen()
{
    return bOpen;
}

void ULeverComponent::SetTimerScale(float value)
{
    TimeScale = value;
}





