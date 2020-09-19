// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "PossessablePawn.h"
#include "GameFramework/Pawn.h"
#include "PossessableComponent.generated.h"


/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UPossesableComponent : public UInteractableComponent
{
	GENERATED_BODY()
	public:
		// Default constructor.
		UPossesableComponent();

		/// An Event that should define using a prop's ability
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void TakeAction();
		
		virtual void TakeAction_Implementation(){};

		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void ButtonRight();

		virtual void ButtonRight_Implementation(){};

		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void ButtonTop();
		
		virtual void ButtonTop_Implementation(){};
	
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void RightTrigger();
			
		virtual void RightTrigger_Implementation(){};

		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void LeftTrigger();
			
		virtual void LeftTrigger_Implementation(){};
	
		UFUNCTION(BlueprintNativeEvent,Category="InputEvent")
		void MoveRightAxis(float Axis);
	
		virtual void MoveRightAxis_Implementation(float Axis){}
		
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveForwardAxis(float Axis);
		
		virtual void MoveForwardAxis_Implementation(float Axis){}

		UFUNCTION(BlueprintNativeEvent, Category = "InputEvent")
	    void LookUpAxis(float Axis);

		virtual void LookUpAxis_Implementation(float Axis){};

		UFUNCTION(BlueprintNativeEvent, Category = "InputEvent")
	    void LookRightAxis(float Axis);

		virtual void LookRightAxis_Implementation(float Axis){};

		// TODO: Add new virtual inputs as necessary
	
		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	private:
		/// The time, in seconds, since this component has been used
		float CurrentCooldown;
	
	public:
		/// The rate, in stamina/sec, at which this prop drains the player's stamina
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamDrainRate;

		/// The initial stamina cost required by a player to possess this prop
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamFrontCost;

		/// The length of time, in seconds, a Player must wait in between using this prop's abilities
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float MaxCooldown;

		/// The amount of Paranoia damage this prop causes to enemies
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float ParanoiaAmount;

		/// The amount of health damage this prop causes to enemies 
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float DamageAmount;

		/// The bounding shape in which this prop must collide with enemies to cause paranoia and/or health damage
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		UShapeComponent * DamageBounds;

};
