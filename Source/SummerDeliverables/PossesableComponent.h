// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "GameFramework/Pawn.h"
#include "PossesableComponent.generated.h"


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
		void MoveRightAxis(float Axis,float DeltaTime);
	
		virtual void MoveRightAxis_Implementation(float Axis,float DeltaTime){GetOwner()->AddActorLocalRotation(FQuat(0.0,0.0,DeltaTime*Axis,0.0));};//this function is a default implementation and should never be called. MoveRightAction should call it
	
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveForwardAxis(float Axis,float DeltaTime);
	
		virtual void MoveForwardAxis_Implementation(float Axis,float DeltaTime){GetOwner()->AddActorLocalOffset(FVector(DeltaTime*Axis,0.0,0.0));};

		virtual void TickComponent(float deltaTime);

		//Other Virtual inputs as necessary
	private:
		AController* CurrentPlayer = nullptr;
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

};
