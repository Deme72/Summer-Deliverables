// Fill out your copyright notice in the Description page of Project Settings.


#include "SpookyMovemenetComponent.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Interactable/Core/PossessablePawn.h"

USpookyMovemenetComponent::USpookyMovemenetComponent()
{
	bDirtyRaycast = true;
}

bool USpookyMovemenetComponent::UpdateRaycastData()
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

void USpookyMovemenetComponent::MoveOwner(FVector DeltaLoc, FHitResult * Hit)
{
	bDirtyRaycast = true;
	APossessablePawn * owner = Cast<APossessablePawn>(GetOwner());
	check(owner);
	owner->GetRootComponent()->AddLocalOffset(DeltaLoc, true, Hit);
}
