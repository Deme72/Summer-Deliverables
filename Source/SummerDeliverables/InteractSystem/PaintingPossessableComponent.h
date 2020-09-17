// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityPossessableComponent.h"
#include "PaintingPossessableComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UPaintingPossessableComponent : public UUtilityPossessableComponent
{
	GENERATED_BODY()
	public:
	UPaintingPossessableComponent();
	UFUNCTION(BlueprintCallable)
	void SetCurrentExitPoint(USceneComponent* NewExitPoint);
	private:
	USceneComponent* currentExitPoint;
	
};
