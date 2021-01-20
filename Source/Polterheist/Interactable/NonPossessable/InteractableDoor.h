// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Polterheist/Interactable/Core/InteractableComponent.h"
#include "Components/TimelineComponent.h"
#include "Polterheist/Core/PlayerPawn.h"
#include "InteractableDoor.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POLTERHEIST_API UInteractableDoor : public UInteractableComponent
{
	GENERATED_BODY()
public:
	///Constructor
	UInteractableDoor();

private:
	///Static Mesh Component for the door
	USceneComponent* Door;
	///Tellsus if the door is open or closed
	bool bOpen;
	///can we use or not use the door
	bool bCanUse;
	///Tells door which way to rotate (set to 1.0 or -1.0)
	float RotateValue;
	///value of for where the Doorcurve is at right now
	float CurveFloatValue;
	///gets the current timeline value
	float TimeLineValue;
	///Doors current rotation
	FRotator DoorRotation;
	///Timeline usedd for creating smooth transition from closed to open andd vic versa
	FTimeline MyTimeLine;
	///the interactbounds
	UBoxComponent* InteractBox;
	///Controls the rotation of the door
	UFUNCTION()
	void ControlDoor();
	///called after the door has finishedd rotating
	UFUNCTION()
	void DoorCanUse();
	///Gets the player that is trying to interact with the door
	APlayerPawn* Player;
public:
	///Called at start of game
    void BeginPlay();
	///Called every Frame
    virtual void TickComponent(float DeltaTime,enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION()
	virtual void OnInteract_Implementation() override;
	UFUNCTION()
	virtual void EndInteract_Implementation() override;

	///Time line curve for the door to use for smoothly opening andd closing it
	UPROPERTY(EditAnywhere,Category="DoorMovement")
	UCurveFloat* DoorCurve;
};
