// Fill out your copyright notice in the Description page of Project Settings.


#include "ParanoiaComponent.h"
#include "Components/ShapeComponent.h"

// Sets default values for this component's properties
UParanoiaComponent::UParanoiaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UParanoiaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UParanoiaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(uses > 0) // use cooldown  
	{
		usesCooldownTime += DeltaTime;
		if(usesCooldownTime > useCooldown)
		{
			uses -= 1;
			usesCooldownTime = 0;
		}
	}

	
	if(active) // dealing damage  (UNFINISHED)
	{
		/*timer+=DeltaTime;
		if(currentTime >= timer) // if we are done doing damage
		{//reset
			active = false;
			currentTime = 0;
		}
		else
		{
			
		}*/
		TArray<AActor *> collisions = {};
		ParanoiaBounds->GetOverlappingActors(collisions);
		for(auto i = collisions.begin(); i != collisions.end(); ++i)
		{
			//TODO: get an enemy component to check against and make sure our target is an enemy
			paranoiaAmount = 1/uses+1;
			//target->TakeDamage(paranoiaAmount);
		}
		
	}

}

void UParanoiaComponent::OnInteract_Implementation() // When Selected
{
	//TODO: make pretty highlight/animations <3

}

void UParanoiaComponent::EndInteract_Implementation() // Activate ParaProp
{
	active = true;
	uses += 1;
}

