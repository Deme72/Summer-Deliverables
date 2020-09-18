// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossessableComponent.h"
#include "DynamicPossessableComponent.generated.h"

/**
 * A Object oriented way of telling which type of prop we are possessing, this is for dynamic props (props that move)
 * Functions in this class are a template for what a Dynamic Possessable should override at minimum
 * TODO: add any functionality that is specific only to Dynamic Possessables
 */
UCLASS()
class SUMMERDELIVERABLES_API UDynamicPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()

	///Override of move to basic movement functionality
	virtual void MoveRightAxis_Implementation(float Axis) override;
	///Override of move to basic movement functionality
	virtual void MoveForwardAxis_Implementation(float Axis) override;
	///Doesn't change anything, only here as a template
	virtual void ScareButton_Implementation() override;
};
