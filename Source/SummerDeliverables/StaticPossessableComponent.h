// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossesableComponent.h"
#include "StaticPossessableComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UStaticPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()
	public:
	virtual void TakeAction_Implementation()override;
};
