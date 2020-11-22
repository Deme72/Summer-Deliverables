// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerStamina.generated.h"

UCLASS()
class SUMMERDELIVERABLES_API APlayerStamina : public AActor
{
	GENERATED_BODY()

public:
	///Stamina Value
	UPROPERTY(EditAnywhere)
	float StaminaVal;

	UPROPERTY(EditAnywhere)
	float TeamStaminaVal;
	
	// Sets default values for this actor's properties
	APlayerStamina();
};
