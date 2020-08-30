// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessablePawn.h"
#include "GameFramework/Actor.h"
#include "PlayerPawn.h"
#include "DefinedDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PossessableComponent.h"

APossessablePawn::APossessablePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass()));
	RootComponent = StaticMeshComponent;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(MakeUniqueObjectName(this, USkeletalMeshComponent::StaticClass()));
	SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	ExitPoint = CreateDefaultSubobject<USceneComponent>(MakeUniqueObjectName(this, USceneComponent::StaticClass()));
	ExitPoint->SetupAttachment(SkeletalMeshComponent);
}

void APossessablePawn::OnConstruction(const FTransform & Transform)
{
	/*RootComponent = StaticMeshComponent;
	SkeletalMeshComponent->AttachTo(StaticMeshComponent);
	ExitPoint->AttachTo(SkeletalMeshComponent);*/
	CurrentPlayer = nullptr;
	if(PossessableComponentType && !PossessableComponent)
	{
		PossessableComponent = NewObject<UPossesableComponent>(this , PossessableComponentType);
		PossessableComponent->RegisterComponent();
		TArray<UShapeComponent*> tmp = {};
		GetComponents<UShapeComponent>(tmp);
		PossessableComponent->InteractBounds = tmp.Pop();
		V_LOG(("NUM OF COMPONENTS" + (tmp.Num())));
	}
}

void APossessablePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APossessablePawn::EndPossession);
	PlayerInputComponent->BindAction("FaceButtonBottom", IE_Pressed, PossessableComponent, &UPossesableComponent::TakeAction);
	PlayerInputComponent->BindAction("FaceButtonRight", IE_Pressed, PossessableComponent, &UPossesableComponent::ButtonRight);
	PlayerInputComponent->BindAction("FaceButtonTop", IE_Pressed, PossessableComponent, &UPossesableComponent::ButtonTop);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, PossessableComponent, &UPossesableComponent::RightTrigger);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, PossessableComponent, &UPossesableComponent::LeftTrigger);
	PlayerInputComponent->BindAxis("MoveRight", PossessableComponent, &UPossesableComponent::MoveRightAxis);
	PlayerInputComponent->BindAxis("MoveForward", PossessableComponent, &UPossesableComponent::MoveForwardAxis);
}

void APossessablePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPlayer)
	{
		CurrentPlayer->Stamina -= PossessableComponent->StamDrainRate*DeltaTime;
		if(CurrentPlayer->Stamina <= 0)
		{
			EndPossession();
			CurrentPlayer->Stamina = 0;
		}
	}
}

void APossessablePawn::EndPossession()
{
	if (CurrentPlayer)
	{
		// move player pawn to the exit point and repossess
		CurrentPlayer->SetActorLocation(ExitPoint->GetComponentLocation());
		GetController()->Possess(CurrentPlayer);
		PossessableComponent->EndInteractInternal();
		CurrentPlayer = nullptr;
	}
}