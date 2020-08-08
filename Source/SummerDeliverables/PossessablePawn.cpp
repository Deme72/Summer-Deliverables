// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessablePawn.h"
#include "GameFramework/Actor.h"
#include "PlayerPawn.h"
#include "PossessableComponent.h"

APossessablePawn::APossessablePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayer = nullptr;
}

void APossessablePawn::BeginPlay()
{
	Super::BeginPlay();
}

void APossessablePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TakeAction", IE_Pressed, PossessableComponent, &UPossesableComponent::TakeAction);
	PlayerInputComponent->BindAxis("MoveRightAxis", PossessableComponent, &UPossesableComponent::MoveRightAxis);
	PlayerInputComponent->BindAxis("MoveForwardAxis", PossessableComponent, &UPossesableComponent::MoveRightAxis);
}

void APossessablePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPlayer)
	{
		// TODO: check and drain stamina here
		// if stamina <= 0: EndPossession()
	}
}

void APossessablePawn::EndPossession()
{
	// repossesses the og player
	// calls OnUnpossess

	if (CurrentPlayer)
	{
		// move player pawn to the exit point and repossess
		CurrentPlayer->SetActorLocation(ExitPoint->GetComponentLocation());
		GetController()->Possess(CurrentPlayer);
		PossessableComponent->EndInteract();
	}
}
