// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsMovementComponent.h"

UPhysicsMovementComponent::UPhysicsMovementComponent():UNavBlockingMovementComponent()
{
}

void UPhysicsMovementComponent::BeginPlay()
{
	UpdateRaycastData();
	UpdateCounter = FMath::RandRange(0.0f, 1.0f);
}

FHitResult UPhysicsMovementComponent::MoveOwner(FVector DeltaLoc)
{
	FHitResult ret = Super().MoveOwner(DeltaLoc);
	UpdateRaycastData();
	return ret;
}

bool UPhysicsMovementComponent::TeleportOwner(FVector loc, FRotator rot)
{
	bool ret = Super().TeleportOwner(loc, rot);
	StopMovementImmediately();
	UpdateRaycastData();
	return ret;
}

void UPhysicsMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if(PawnOwner->IsPlayerControlled())
	{
		UpdateRaycastData();
	}
	else
	{
		UpdateCounter+= DeltaTime;
		if(UpdateCounter > 1.0)
		{
			UpdateCounter = 0;
			UpdateRaycastData();
		}
	}
}
