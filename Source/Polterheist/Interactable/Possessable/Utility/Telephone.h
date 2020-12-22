// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractSystem/UtilityPossessableComponent.h"
#include "Telephone.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UTelephone : public UUtilityPossessableComponent
{
	GENERATED_BODY()

public:
	UTelephone();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Network")
	APossessablePawn* ConnectedSwitchboard;

	virtual void MoveButton_Implementation() override;
};
