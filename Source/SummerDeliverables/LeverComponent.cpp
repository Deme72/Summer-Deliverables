// Fill out your copyright notice in the Description page of Project Settings.

#include "LeverComponent.h"
#include "SummerDeliverables/InteractSystem/PossessablePawn.h"

#include <corecrt_io.h>



#include "DrawDebugHelpers.h"
#include "LeverPawn.h"

ULeverComponent::ULeverComponent()
{
    open = false;
    Slide_Back = 0.0f;
    begin_rot =0.0f;
    timer = 0.0f;
    controlling = false;
    Lever_ref = Cast<ALeverPawn>(GetOwner());
    maxYaw = 0.0f;
}

void ULeverComponent::BeginPlay()
{
    Super::BeginPlay();
    if (Lever_ref)
    {
        Endline_pos1 = Lever_ref->EndLinepos1->GetComponentLocation();
        Endline_pos2 = Lever_ref->EndLinepos2->GetComponentLocation();
        begin_rot = Lever_ref->GetActorRotation().Yaw;
    }
}

void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Deltatime = DeltaTime;
    if (controlling)
    {
        DrawDebugLine(GetOwner()->GetWorld(),Endline_pos1,Endline_pos2,FColor::Red,false,0.0, 0, 5.0);
        DrawDebugLine(GetOwner()->GetWorld(),Lever_ref->StartLinepos1->GetComponentLocation(),Lever_ref->StartLinepos2->GetComponentLocation(),FColor::Cyan,false,0.0,0,5.0);
    }
    if (begin_rot >= Lever_ref->GetActorRotation().Yaw)
    {
        Slide_Back = 0.0;
    }
    else
    {
        Lever_ref->AddActorLocalRotation(FRotator(0.0,Slide_Back*Deltatime,0.0));
    }
    if (open)
    {
        if (timer <= 0.0)
        {
            open = false;
            timer =0.0f;
            Slide_Back = -10.0f;
        }
        else
        {
            timer = timer - (Deltatime*timescale);
        }
    }
}

void ULeverComponent::ScareButton_Implementation()
{
    if (open == false)
    {
        Slide_Back = 0.0f;
        if (Lever_ref->GetActorRotation().Yaw >= begin_rot+180.0)
        {
            Lever_ref->SetActorRotation(FRotator(0.0,begin_rot+180.0,0.0));
            open = true;
            timer = 7.0f;
            timescale = 1.0f;
        }
        else
        {
           Lever_ref->AddActorLocalRotation(FRotator(0.0,Deltatime*400.0,0.0));
           //GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,FString::Printf(TEXT("%f"),Lever_ref->GetActorRotation().Yaw));
           //GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,FString::Printf(TEXT("%f"),begin_rot+180.0));
        }
    }
}

void ULeverComponent::ScareButtonRelease_Implementation()
{
    if (open == false)
    {
        Slide_Back = -10.0f;
    }
}

void ULeverComponent::OnInteract_Implementation()
{
    controlling = true;
}

void ULeverComponent::EndInteract_Implementation()
{
    controlling = false;
}

bool ULeverComponent::get_open()
{
    return open;
}

void ULeverComponent::set_timerscale(float value)
{
    timescale = value;
}





