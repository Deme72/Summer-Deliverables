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

	PlayerInputComponent->BindAction("Interact", IE_Pressed, PossessableComponent, &UPossesableComponent::TakeAction);
	PlayerInputComponent->BindAxis("MoveRight", PossessableComponent, &UPossesableComponent::MoveRightAxis);
	PlayerInputComponent->BindAxis("MoveForward", PossessableComponent, &UPossesableComponent::MoveRightAxis);
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
		PossessableComponent->EndInteract();
	}
}
