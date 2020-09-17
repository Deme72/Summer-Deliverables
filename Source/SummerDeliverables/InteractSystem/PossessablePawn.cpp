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
}

void APossessablePawn::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	CurrentPlayer = nullptr;
	if(PossessableComponentType && !IsValid(PossessableComponent))
	{
		PossessableComponent = NewObject<UPossesableComponent>(this , PossessableComponentType);
		PossessableComponent->RegisterComponent();
		//TODO: BAD WAY OF GETTING SHAPE COMPONENTS, DOESN'T WORK WITH MORE THAN ONE, FIX LATER
		TArray<UShapeComponent*> tmp = {};
		GetComponents<UShapeComponent>(tmp);
		if(tmp.Num())
			PossessableComponent->InteractBounds = tmp.Pop();
	}
}

void APossessablePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("InteractButton", IE_Released, this, &APossessablePawn::EndPossession);
	PlayerInputComponent->BindAction("MoveButton", IE_Pressed, PossessableComponent, &UPossesableComponent::MoveButton);
	PlayerInputComponent->BindAction("ScareButton", IE_Pressed, PossessableComponent, &UPossesableComponent::ScareButton);
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