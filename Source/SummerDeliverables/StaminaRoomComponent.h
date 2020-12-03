// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "InteractSystem/UtilityPossessableComponent.h"
#include "StaminaRoomComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UStaminaRoomComponent : public UUtilityPossessableComponent
{
	GENERATED_BODY()

public:
    UStaminaRoomComponent();

private:
    ///Static mesh component of the room which is the door
    UStaticMeshComponent* Door;
    ///Gets the box collider on the room
    UBoxComponent* Collider;
    ///Doors starting position for closed
    FVector StartPos;
    ///Doors end position for open
    FVector EndPos;
    ///opens the door if set to true else closes it
    bool bOpen;
    ///Creates a timeline for creating a smooth transitions of the door closing and opening
    ///Door: https://unrealcpp.com/open-door-with-timeline/
    ///TimeLines: https://www.youtube.com/watch?v=-QxrTZrceqw
    FTimeline TimeLineComponent;
    ///Checks to see if stamania room collide is colliding with player
    bool bUsing;
    ///sees how long the player stands in the collider, no more than 3 secs, and then activates the stamania boost and whatever else
    float TimerTillActivate;
public:
    ///Called at start of game
    void BeginPlay();
    ///Called every Frame
    virtual void TickComponent(float DeltaTime,enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    ///Checks to see if the player is trying to use the ability 
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    ///checks if player has stopped trying to use the ability
    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
    ///timeline calls this when open is true to open the door
    UFUNCTION(BlueprintCallable,Category="DoorMovement")
    void OpenDoor(float Curveamount);
    ///timeline calls this when open is false to close the door
    UFUNCTION(BlueprintCallable,Category="DoorMovement")
    void CloseDoor();
    ///gets the curve object we created for the timeline to use for a smooth time scale
    ///EX: if max time on the curve is 1 and the current time is 0.5 then the value would be 0.5 so the door would be half opened.
    UPROPERTY(EditAnywhere,Category="DoorMovement")
    class UCurveFloat* Curve;
    
};
