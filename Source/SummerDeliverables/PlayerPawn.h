// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class PossessableComponent;

UCLASS()
class SUMMERDELIVERABLES_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stamina)
	float Stamina = 1.0f;
	
private:
	PossessableComponent * CurrentBindings = nullptr;
	TArray<UInteractableComponent> OverlappingInteractables;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	bool TakeAction();

	UFUNCTION()
	TArray<UInteractableComponent> GetOverlappingInteractables() const
	{return OverlappingInteractables;}

	UFUNCTION()
	bool IsPossessing() const
	{return CurrentBindings != nullptr;}

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);
};
