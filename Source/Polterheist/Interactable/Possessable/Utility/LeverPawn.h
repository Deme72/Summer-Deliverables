// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractSystem/PossessablePawn.h"
#include "LeverPawn.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API ALeverPawn : public APossessablePawn
{
	GENERATED_BODY()
public:
    ALeverPawn();

public:

    ///Positions of where the debug lines start and end (pos1 = start and pos2 = end)
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USceneComponent* StartLinePos1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USceneComponent* StartLinePos2;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USceneComponent* EndLinePos1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USceneComponent* EndLinePos2;
};
