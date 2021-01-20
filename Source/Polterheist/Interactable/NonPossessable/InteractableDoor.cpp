// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

UInteractableDoor::UInteractableDoor()
{
    PrimaryComponentTick.bCanEverTick = true;
    Player = nullptr;
    bOpen = false;
    bActivate = true;
}

void UInteractableDoor::BeginPlay()
{
    Super::BeginPlay();
    RotateValue = 1.0f;
    TInlineComponentArray<UActorComponent*> tempa;
    Door = Cast<USceneComponent>(GetOwner()->GetRootComponent());
    if (DoorCurve)
    {
        FOnTimelineFloat Timelinebegin;
        FOnTimelineEventStatic Timelinefinished;

        Timelinebegin.BindUFunction(this,FName("ControlDoor"));
        Timelinefinished.BindUFunction(this,FName("SetCanBeUsed"));
        MyTimeLine.AddInterpFloat(DoorCurve,Timelinebegin);
        MyTimeLine.SetTimelineFinishedFunc(Timelinefinished);
    }
}

void UInteractableDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    MyTimeLine.TickTimeline(DeltaTime);
}

void UInteractableDoor::ControlDoor()
{
    TimeLineValue = MyTimeLine.GetPlaybackPosition();
    CurveFloatValue = RotateValue*DoorCurve->GetFloatValue(TimeLineValue);
    FQuat Newrotation = FQuat(FRotator(0.0f,CurveFloatValue,0.0f));
    Door->SetRelativeRotation(Newrotation);
}

void UInteractableDoor::SetCanBeUsed()
{
    bActivate = true;
}


void UInteractableDoor::OnInteract_Implementation()
{
    if (bActivate == true)
    {
        bOpen=!bOpen;
        DoorRotation = Door->GetRelativeRotation();
        GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,TEXT("Interacted with"));
        if (bOpen)
        {
            RotateValue = -1.0f;
            MyTimeLine.PlayFromStart();
        }
        else
        {
            MyTimeLine.Reverse();
        }
        bActivate = false;
    }
}







