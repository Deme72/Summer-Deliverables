// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractSystem/UtilityPossessableComponent.h"

#include "LeverComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API ULeverComponent : public UUtilityPossessableComponent
{
	GENERATED_BODY()

	ULeverComponent();

protected:
	///sets the start pos of the end line
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	FVector Endline_pos1;
	///sets the end pos of the end line
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
    FVector Endline_pos2;
	///how much the lever moves back to its orginal rotation
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	float Slide_Back;
	///what the starting rotation is depending on which axis the rotation is happening on
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	float begin_rot;
	///how long until the lever can be used again
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
    float utimer;
	///Checkes to see if we are controlling the lever or not
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	bool controlling;
	///World time
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	float Deltatime;
	///Sets the max rotation for when the object is set to open
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
	float maxYaw;
	///Tells the secret room that the lever this component is attached has been switched on or off
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Lever")
    bool open;
	///Gets a ref to the thing we are attached to
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
    class ALeverPawn* Lever_ref;
	//Tells the lever that the room is open andd to not start reseting and closing the door until the players use the rooms ability
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
    float timescale;
	///how long until the lever resets this will be between 1 to 3 secs
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Lever")
    float timer;

public:

	void BeginPlay();
	///Called every Frame
	virtual void TickComponent(float DeltaTime,enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void ScareButton_Implementation() override;

	virtual void ScareButtonRelease_Implementation() override;

	virtual void OnInteract_Implementation() override;

	virtual void EndInteract_Implementation() override;

	bool get_open();

	void set_timerscale(float value);
};
