// Fill out your copyright notice in the Description page of Project Settings.


#include "ParanoiaComponent.h"
#include "Components/ShapeComponent.h"
#include "SummerDeliverables/BaseEnemyCharacter.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"


/// Sets default values for this component's properties
UParanoiaComponent::UParanoiaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	uses=0; 			
	usesCooldownTime = 0;
	useCooldown = 0;
	active = false;
}

void UParanoiaComponent::InitializeComponent()
{
	Super::InitializeComponent();
	TArray<UShapeComponent*> tmp = {};
	GetOwner()->GetComponents<UShapeComponent>(tmp);
	for(auto shape:tmp){
		if(InteractBounds == nullptr &&
            shape->ComponentHasTag("Interact"))
            	InteractBounds = shape;
		if(ParanoiaBounds == nullptr &&
            shape->ComponentHasTag("Paranoia"))
            	ParanoiaBounds = shape;
	}
}

void UParanoiaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(uses > 0)
	{
		usesCooldownTime += DeltaTime;
		if(usesCooldownTime > useCooldown)
		{
			uses--;
			usesCooldownTime -= useCooldown;
		}
	}

	
	if(active)
	{
		if(ParanoiaBounds)
		{
			TArray<AActor *> collisions = {};
			ParanoiaBounds->GetOverlappingActors(collisions);
			for(auto collision:collisions)
			{
				ABaseEnemyCharacter * enemy = Cast<ABaseEnemyCharacter>(collision);
				if(enemy)
					enemy->TakeParanoiaDamage(paranoiaAmount);
			}
		}
		active = false;
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
	uses++;
	Super::EndInteractInternal();
}

void UParanoiaComponent::Set_Outline(bool OutLine_ON,int depthInt)
{
	UActorComponent* staticmesh = GetOwner()->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());
	UStaticMeshComponent* staticmeshc = Cast<UStaticMeshComponent>(staticmesh);
	if (staticmeshc)
	{
		staticmeshc->SetRenderCustomDepth(OutLine_ON);
		staticmeshc->SetCustomDepthStencilValue(depthInt);
	}
}

