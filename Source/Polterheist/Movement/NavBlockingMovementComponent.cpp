// Fill out your copyright notice in the Description page of Project Settings.


#include "NavBlockingMovementComponent.h"
#include "Components/CapsuleComponent.h"

UNavBlockingMovementComponent::UNavBlockingMovementComponent():USpookyMovementComponent()
{
	NavBlocker = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NavBlocker"));
	NavBlocker->SetCapsuleSize(50, 200);
	bWantsInitializeComponent = true;
}

void UNavBlockingMovementComponent::InitializeComponent()
{
	NavBlocker->SetupAttachment(GetOwner()->GetRootComponent());
	NavBlocker->SetRelativeLocation({0,0,NavBlocker->GetUnscaledCapsuleHalfHeight()});
	NavBlocker->SetUsingAbsoluteScale(true);
	NavBlocker->SetUsingAbsoluteRotation(true);
	NavBlocker->SetUsingAbsoluteLocation(true);
}

bool UNavBlockingMovementComponent::UpdateRaycastData()
{
	setRayWidth(NavBlocker->GetUnscaledCapsuleRadius()/2);
	if(Super::UpdateRaycastData())
	{
		bAboveCapsule = GroundHeight > 2*NavBlocker->GetUnscaledCapsuleHalfHeight();
		NavBlocker->SetWorldLocation(GetOwnerMesh()->GetComponentLocation() +
	                                     FVector(0, 0, NavBlocker->GetUnscaledCapsuleHalfHeight() - GroundHeight - 1));
		return true;
	}
	return false;
}
