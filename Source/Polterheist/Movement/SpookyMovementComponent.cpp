// Fill out your copyright notice in the Description page of Project Settings.


#include "SpookyMovementComponent.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Interactable/Core/PossessablePawn.h"

USpookyMovementComponent::USpookyMovementComponent()
{
	bDirtyRaycast = true;
}

bool USpookyMovementComponent::UpdateRaycastData()
{
	if(bDirtyRaycast)
	{
		FHitResult groundHit;
		GetWorld()->SweepSingleByChannel(groundHit,
                                        GetOwner()->GetRootComponent()->GetComponentLocation(),
                                        GetOwner()->GetRootComponent()->GetComponentLocation() - FVector{0,0, 100000000},
                                        FQuat::Identity,
                                        ECollisionChannel::ECC_GameTraceChannel4,
                                        FCollisionShape::MakeBox({RaycastWidth,RaycastWidth,1}));
		GroundHeight = groundHit.Distance;
		bOnGround = GroundHeight < 50;
		bDirtyRaycast = false;
		return true;
	}
	return false;
}

FHitResult USpookyMovementComponent::MoveOwner(FVector DeltaLoc)
{
	FVector nextForwardNorm = DeltaLoc;
	FHitResult Hit;
	bDirtyRaycast = true;
	APossessablePawn * owner = Cast<APossessablePawn>(GetOwner());
	check(owner);
	owner->GetRootComponent()->AddLocalOffset(DeltaLoc, true, &Hit);
	if(Hit.bBlockingHit)
	{
		FVector backmove = DeltaLoc.GetSafeNormal();
		owner->AddActorWorldOffset(backmove *.1,true);
		FVector Normal = FVector::VectorPlaneProject(Hit.Normal, {0,0,1}).GetSafeNormal();
		FVector newMove = FVector::VectorPlaneProject(DeltaLoc, Normal);
		owner->AddActorWorldOffset(newMove, true);
		nextForwardNorm += newMove;
	}
	MovementForwardNormal = nextForwardNorm.GetSafeNormal();
	return Hit;
}

bool USpookyMovementComponent::TeleportOwner(FVector loc, FRotator rot)
{
	bDirtyRaycast = true;
	return GetOwner()->TeleportTo(loc,rot);
}

FVector USpookyMovementComponent::AllignInputToCamera(FVector in, FVector camForward, FVector camRight, float speed,
                                                       float DeadZone)
{
	if(in.Size() < DeadZone)
		in = {0,0,0};
	else if(in.Size() > 1)
		in.Normalize();
	FVector inNorm = in.GetSafeNormal();
	
	camForward = {camForward.X, camForward.Y, 0};
	camForward.Normalize();
	
	camRight = {camRight.X, camRight.Y, 0};
	camRight.Normalize();

	float mag = in.Size() * speed;
	FVector result = mag * inNorm.X * camForward + mag * inNorm.Y * camRight;
	return result;
}
