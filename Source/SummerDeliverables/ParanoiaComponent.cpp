// Fill out your copyright notice in the Description page of Project Settings.


#include "ParanoiaComponent.h"
#include "Components/ShapeComponent.h"
#include "DefinedDebugHelpers.h"

// Sets default values for this component's properties
UParanoiaComponent::UParanoiaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	uses=0; 			 // effectiveness modified by uses
	usesCooldownTime = 0;
	useCooldown = 0;
	active = false;
}


// Called when the game starts
void UParanoiaComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UParanoiaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(uses > 0) // use cooldown  
	{
		usesCooldownTime += DeltaTime;
		if(usesCooldownTime > useCooldown && uses > 0)
		{
			uses -= 1;
			usesCooldownTime = 0;
		}
	}

	
	if(active) // dealing damage  (UNFINISHED)
	{
		TArray<AActor *> collisions = {};
		ParanoiaBounds->GetOverlappingActors(collisions);
		for(auto i = collisions.begin(); i != collisions.end(); ++i)
		{
			// TODO: get an enemy component to check against and make sure our target is an enemy
			// TODO: Find the cause of ParanoiaComponent::uses falling below zero (parallel security issue(?))
			if (uses > 0)
				paranoiaAmount = 1/uses+1;
			//else
				//V_LOG(TEXT("USES IS FALLING BELOW ZERO"));
			//target->TakeDamage(paranoiaAmount);
		}
		
	}

}

void UParanoiaComponent::OnInteractInternal() // When Selected
{
	Super::OnInteractInternal();
	// TODO: make pretty highlight/animations <3

}

void UParanoiaComponent::EndInteractInternal() // Activate ParaProp
{
	active = true;
	uses += 1;
	Super::EndInteractInternal();
}

