// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingPossessableComponent.h"

UPaintingPossessableComponent::UPaintingPossessableComponent()
{
    StamFrontCost=0.0f;
}

void UPaintingPossessableComponent::SetCurrentExitPoint(USceneComponent* NewExitPoint)
{
    currentExitPoint=NewExitPoint;
}
