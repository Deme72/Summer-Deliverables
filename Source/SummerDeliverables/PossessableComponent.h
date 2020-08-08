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
		
		virtual void OnInteract_Implementation() override;
		virtual void EndInteract_Implementation() override;
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void TakeAction();
	
		virtual void TakeAction_Implementation(){};
	
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveRightAxis(float Axis);
	
		virtual void MoveRightAxis_Implementation(float Axis){}
	
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveForwardAxis(float Axis);
	
		virtual void MoveForwardAxis_Implementation(float Axis){}

		virtual void TickComponent(float deltaTime);

		//Other Virtual inputs as necessary
	private:
		float CurrentCooldown =0.0f;
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
		USceneComponent * ExitPoint;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamDrainRate;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamFrontCost;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float MaxCooldown;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float ParanoiaAmount;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float DamageAmount;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		UShapeComponent * DamageBounds;

};
