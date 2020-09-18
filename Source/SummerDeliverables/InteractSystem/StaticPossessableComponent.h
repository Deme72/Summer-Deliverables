// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossessableComponent.h"
#include "StaticPossessableComponent.generated.h"

/**
 * A Object oriented way of telling which type of prop we are possessing, this is for static props (props that don't move)
 * Functions in this class are a template for what a Static Possessable should override at minimum
 * TODO: add any functionality that is specific only to Static Possessables
**/
UCLASS()
class SUMMERDELIVERABLES_API UStaticPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()
	public:
	///Doesn't change anything, only here as a template
	virtual void ScareButton_Implementation() override;
};
