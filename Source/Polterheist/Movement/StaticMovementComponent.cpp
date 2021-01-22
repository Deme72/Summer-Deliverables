// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMovementComponent.h"
#include "Polterheist/External/External.h"

UStaticMovementComponent::UStaticMovementComponent() :UNavBlockingMovementComponent()
{
	
}

FHitResult UStaticMovementComponent::MoveOwner(FVector DeltaLoc)
{
	FHitResult ret;
	V_LOGV(LogTemp, Error, "StaticComponents Can't Move");
	return ret;
}

bool UStaticMovementComponent::TeleportOwner(FVector loc, FRotator rot)
{
	bool ret = Super().TeleportOwner(loc, rot);
	UpdateRaycastData();
	return ret;
}
