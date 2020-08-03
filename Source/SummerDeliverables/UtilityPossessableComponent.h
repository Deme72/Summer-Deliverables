// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossesableComponent.h"
#include "UtilityPossessableComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UUtilityPossessableComponent : public UPossesableComponent
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Network")
	TArray<UUtilityPossessableComponent*>Network;
};
