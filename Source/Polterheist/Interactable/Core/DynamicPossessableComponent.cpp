// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicPossessableComponent.h"

UDynamicPossessableComponent::UDynamicPossessableComponent():UPossesableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UDynamicPossessableComponent::MoveRightAxis_Implementation(float Axis)
{
    GetOwner()->AddActorLocalRotation(FQuat(0.0,0.0,Axis,0.0));
}

void UDynamicPossessableComponent::MoveForwardAxis_Implementation(float Axis)
{
    GetOwner()->AddActorLocalOffset(FVector(Axis,0.0,0.0));
};


void UDynamicPossessableComponent::ScareButton_Implementation()
{
}
