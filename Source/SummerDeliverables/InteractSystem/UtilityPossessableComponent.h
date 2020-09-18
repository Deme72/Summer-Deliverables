// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossessableComponent.h"
#include "UtilityPossessableComponent.generated.h"

/**
 * TODO: As is the same as all the utilities, this is incomplete
**/

UCLASS()
class SUMMERDELIVERABLES_API UUtilityPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Network")
	TArray<UUtilityPossessableComponent*> Network;
};
