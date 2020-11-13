// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaRoomComponent.h"

#include "LeverComponent.h"
#include "LeverPawn.h"
#include "Components/TimelineComponent.h"
#include "InteractSystem/PlayerGhostController.h"

UStaminaRoomComponent::UStaminaRoomComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    //set the room to closed at start
    open = false;
    TInlineComponentArray<UActorComponent*> tempa;
    //get the room actor this component is attached to
    APossessablePawn* room = Cast<APossessablePawn>(GetOwner());
    //GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,room->GetDebugName(room));
    if (room)
    {
        //get the door mesh from the actor and its position
        room->GetComponents(tempa);
        for (auto com : tempa)
        {
            if (com->ComponentHasTag(TEXT("Door")))
            {
                Door = Cast<UStaticMeshComponent>(com);
                Startpos = Door->GetRelativeLocation();
                Endpos = Door->GetRelativeLocation()+ FVector(-975.0,0.0,0.0);  
            }
            if (com->IsA(UBoxComponent::StaticClass()))
            {
                Collider = Cast<UBoxComponent>(com);
            }
        }
    }
    Using = false;
    Utimer = 3.0f;
}

void UStaminaRoomComponent::BeginPlay()
{
    Super::BeginPlay();
    if (curve)
    {
        //start and end functions for binding to timeline
        FOnTimelineFloat Timelinecallback;
        FOnTimelineEventStatic Timelinefinishedcallback;

        //Bind our functions to our ftimelineevents above
        Timelinecallback.BindUFunction(this,FName("OpenDoor"));
        Timelinefinishedcallback.BindUFunction(this,FName("CloseDoor"));
        //add the bindings to the timelinecomponent
        TimeLineComponent.AddInterpFloat(curve,Timelinecallback);
        TimeLineComponent.SetTimelineFinishedFunc(Timelinefinishedcallback);

        //set looping to false so it doesnt repeativily play start
        TimeLineComponent.SetLooping(false);
    }
    if (Collider)
    {
        Collider->OnComponentBeginOverlap.AddDynamic(this,&UStaminaRoomComponent::OnOverlapBegin);
        Collider->OnComponentEndOverlap.AddDynamic(this,&UStaminaRoomComponent::OnOverlapEnd);
    }
}

void UStaminaRoomComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
    TimeLineComponent.TickTimeline(DeltaTime);
    for (auto& lever: Network)
    {
        ALeverPawn* temp_lever = Cast<ALeverPawn>(lever);
        if (temp_lever)
        {
            ULeverComponent* Levercom = Cast<ULeverComponent>(temp_lever->PossessableComponent);
            if (Levercom)
            {
                if (Levercom->get_open())
                {
                    open = true;
                }
                else
                {
                    open = false;
                    break;
                }
            }
        }
    }
    if (open)
    {
        TimeLineComponent.Play();
        for (auto& lever: Network)
        {
            ALeverPawn* temp_lever = Cast<ALeverPawn>(lever);
            if (temp_lever)
            {
                ULeverComponent* Levercom = Cast<ULeverComponent>(temp_lever->PossessableComponent);
                if (Levercom)
                {
                    Levercom->set_timerscale(0.0f);
                }
            }
        }
    }
    if (Using)
    {
        if (Utimer <=0.0f)
        {
            Utimer = 0.0f;
            for (FConstPlayerControllerIterator player = GetWorld()->GetPlayerControllerIterator(); player;++player)
            {
                APlayerGhostController* p = Cast<APlayerGhostController>(player->Get());
                if (p)
                {
                    p->SetStamina(101.0f);
                    p->SetInvisibility(true);
                }
            }
            Using= false;
        }
        else
        {
            Utimer-=DeltaTime;
        }
    }
}

void UStaminaRoomComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //check to see if players are inside so when the room closes we know which players get kicked out
    APlayerPawn* temp_p;
    temp_p = Cast<APlayerPawn>(OtherActor);
    if (temp_p && Using == false && Utimer > 0.0f)
    {
        Using = true;
    }
}

void UStaminaRoomComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    //removes player from the list of being inside so we dont kick them later
    APlayerPawn* temp_p;
    temp_p = Cast<APlayerPawn>(OtherActor);
    if (temp_p && Utimer > 0.0f)
    {
        Using = false;
        Utimer = 3.0f;
    }
}


void UStaminaRoomComponent::OpenDoor(float Curveamount)
{
    if (Door)
    {
        Door->SetRelativeLocation(FMath::Lerp(Startpos,Endpos,Curveamount));
    }
}

void UStaminaRoomComponent::CloseDoor()
{
    TimeLineComponent.Reverse();
}

