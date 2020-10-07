// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessablePawn.h"
#include "GameFramework/Actor.h"
#include "SummerDeliverables/PlayerPawn.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "PossessableComponent.h"
#include "PlayerGhostController.h"
#include "../PlayerPawn.h"

APossessablePawn::APossessablePawn():APawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PossessableBaseMesh"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalBaseMesh"));
	ExitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Exit Point"));
	
	RootComponent = StaticMeshComponent;
	SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	ExitPoint->SetupAttachment(SkeletalMeshComponent);
    ExitPawn = nullptr;
}

void APossessablePawn::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	//CurrentPlayerController = nullptr;
	if(PossessableComponentType && !IsValid(PossessableComponent))
	{
		PossessableComponent = NewObject<UPossesableComponent>(this , PossessableComponentType);
		PossessableComponent->RegisterComponent();
	}
}

void APossessablePawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(PossessableComponent)
	{
		TArray<UShapeComponent*> tmp = {};
		GetComponents<UShapeComponent>(tmp);
		for(auto shape:tmp){
			if(PossessableComponent->InteractBounds == nullptr &&
                shape->ComponentHasTag("Interact"))
                	PossessableComponent->InteractBounds = shape;
			if(PossessableComponent->DamageBounds == nullptr &&
                shape->ComponentHasTag("Damage"))
                	PossessableComponent->DamageBounds = shape;
			if(PossessableComponent->ParanoiaBounds == nullptr &&
                shape->ComponentHasTag("Paranoia"))
                	PossessableComponent->ParanoiaBounds = shape;
		}
	}
}

void APossessablePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("InteractButton", IE_Released, this, &APossessablePawn::EndPossession);
	PlayerInputComponent->BindAction("MoveButton", IE_Pressed, PossessableComponent, &UPossesableComponent::MoveButton);
	PlayerInputComponent->BindAction("MoveButton", IE_Released, PossessableComponent, &UPossesableComponent::MoveButtonRelease);
	PlayerInputComponent->BindAction("ScareButton", IE_Pressed, PossessableComponent, &UPossesableComponent::ScareButton);
	PlayerInputComponent->BindAction("ScareButton", IE_Released, PossessableComponent, &UPossesableComponent::ScareButtonRelease);
	PlayerInputComponent->BindAction("FaceButtonTop", IE_Pressed, PossessableComponent, &UPossesableComponent::ButtonTop);
	PlayerInputComponent->BindAction("FaceButtonTop", IE_Released, PossessableComponent, &UPossesableComponent::ButtonTopRelease);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, PossessableComponent, &UPossesableComponent::RightTrigger);
	PlayerInputComponent->BindAction("RightTrigger", IE_Released, PossessableComponent, &UPossesableComponent::RightTriggerRelease);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, PossessableComponent, &UPossesableComponent::LeftTrigger);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Released, PossessableComponent, &UPossesableComponent::LeftTriggerRelease);
	PlayerInputComponent->BindAction("StartButton", IE_Pressed, PossessableComponent, &UPossesableComponent::StartButton);
	PlayerInputComponent->BindAction("StartButton", IE_Released, PossessableComponent, &UPossesableComponent::StartButtonRelease);
	PlayerInputComponent->BindAxis("MoveRight", PossessableComponent, &UPossesableComponent::MoveRightAxis);
	PlayerInputComponent->BindAxis("MoveForward", PossessableComponent, &UPossesableComponent::MoveForwardAxis);
}

void APossessablePawn::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	auto current_controller = Cast<APlayerGhostController>(GetController());
	if (current_controller && PossessableComponent->GetIsActiveStaminaDrain() && PossessableComponent->IsInUse())
	{
		
		if(current_controller->SetStamina(-PossessableComponent->StamDrainRate*DeltaTime))
		{
			EndPossession();
		}
	}
}

void APossessablePawn::EndPossession()
{
	auto ghost_controller = Cast<APlayerGhostController>(GetController());
	if (ghost_controller)
	{
		if(ExitPawn)
		{
			PossessableComponent->EndInteractInternal();
			ghost_controller->Possess(ExitPawn);
			if(!ExitPawn->PossessableComponent->IsInUse())
				ExitPawn->PossessableComponent->OnInteractInternal();
			
			ExitPawn = nullptr;
		}
		else
		{
			// move player pawn to the exit point and repossess
			APlayerPawn* new_pawn = ghost_controller->CreatePlayerPawn(ExitPoint->GetComponentTransform().GetLocation());
			if (new_pawn)
			{
				ghost_controller->Possess(new_pawn);
				new_pawn->setPlayer(ghost_controller);
				PossessableComponent->EndInteractInternal();
			}
		}
		//CurrentPlayerController = nullptr;
	}
}

void APossessablePawn::Set_Outline(bool OutLine_ON,int depthInt)
{
	StaticMeshComponent->SetRenderCustomDepth(OutLine_ON);
	StaticMeshComponent->SetCustomDepthStencilValue(depthInt);
	SkeletalMeshComponent->SetRenderCustomDepth(OutLine_ON);
	SkeletalMeshComponent->SetCustomDepthStencilValue(depthInt);
}

APossessablePawn* APossessablePawn::SpawnSubPawn(TSubclassOf<APossessablePawn> subclass, FTransform const pos)
{
	APossessablePawn * SubPawn = Cast<APossessablePawn>(GetWorld()->SpawnActor(subclass.Get(), &pos));
	if(SubPawn)
	{
		auto controller = Cast<APlayerGhostController>(GetController());
		if(controller)
			if (controller->CanAffordStaminaCost(SubPawn->PossessableComponent->GetFrontStaminaCost()))
			{
				controller->SetStamina(-SubPawn->PossessableComponent->GetFrontStaminaCost());
				controller->Possess(SubPawn);
				SubPawn->PossessableComponent->OnInteractInternal();
				SubPawn->SetNextExit(this);
				return SubPawn;
			}
	}
	return nullptr;
}
