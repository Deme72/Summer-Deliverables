// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

UInteractableDoor::UInteractableDoor()
{
    PrimaryComponentTick.bCanEverTick = true;
    Player = nullptr;
    bOpen = false;
    bCanUse = true;
}

void UInteractableDoor::BeginPlay()
{
    Super::BeginPlay();
    RotateValue = 1.0f;
    TInlineComponentArray<UActorComponent*> tempa;
    Door = Cast<USceneComponent>(GetOwner()->GetRootComponent());
    if (DoorCurve)
    {
        FOnTimelineFloat TimeLineBeginFunc;
        FOnTimelineEventStatic Timelinefinished;
        TimeLineBeginFunc.BindUFunction(this,FName("TimeLineControl"));
        Timelinefinished.BindUFunction(this,FName("CanBeActivated"));
        MyTimeLine.AddInterpFloat(DoorCurve,TimeLineBeginFunc);
        MyTimeLine.SetTimelineFinishedFunc(Timelinefinished);
    }
}

void UInteractableDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    MyTimeLine.TickTimeline(DeltaTime);
}

void UInteractableDoor::TimeLineControl()
{
    TimeLineValue = MyTimeLine.GetPlaybackPosition();
    CurveFloatValue = RotateValue*DoorCurve->GetFloatValue(TimeLineValue);
    FQuat Newrotation = FQuat(FRotator(0.0f,CurveFloatValue,0.0f));
    Door->SetRelativeRotation(Newrotation);
}

void UInteractableDoor::EndInteract_Implementation()
{
    
}

void UInteractableDoor::CanBeActivated()
{
    bCanUse = true;
}

void UInteractableDoor::OnInteract_Implementation()
{
    if (bCanUse)
    {
        bOpen=!bOpen;
        DoorRotation = Door->GetRelativeRotation();
        if (bOpen)
        {
            RotateValue = -1.0f;
            MyTimeLine.PlayFromStart();
        }
        else
        {
            MyTimeLine.Reverse();
        }
        bCanUse = false;
    }
}







