// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMovementComponent.h"

UDynamicMovementComponent::UDynamicMovementComponent() : UNavBlockingMovementComponent()
{
}

FHitResult UDynamicMovementComponent::MoveOwner(FVector DeltaLoc)
{
	FHitResult ret = Super().MoveOwner(DeltaLoc);
	UpdateRaycastData();
	return ret;
}

bool UDynamicMovementComponent::TeleportOwner(FVector loc, FRotator rot)
{
	bool ret = Super().TeleportOwner(loc, rot);
	UpdateRaycastData();
	return ret;
}
