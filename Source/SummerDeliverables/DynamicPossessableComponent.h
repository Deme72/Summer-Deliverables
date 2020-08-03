// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossesableComponent.h"
#include "DynamicPossessableComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UDynamicPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()
	virtual void MoveRightAxis_Implementation(float Axis) override;
	virtual void MoveForwardAxis_Implementation(float Axis) override;
	virtual void TakeAction_Implementation()override;
};
