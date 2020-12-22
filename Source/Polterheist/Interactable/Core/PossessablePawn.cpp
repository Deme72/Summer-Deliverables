// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessablePawn.h"
#include "GameFramework/Actor.h"
#include "Polterheist/Core/Core.h"
#include "Polterheist/Movement/Movement.h"
#include "Polterheist/External/External.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"

APossessablePawn::APossessablePawn():APawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PossessableBaseMesh"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalBaseMesh"));
	ExitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Exit Point"));
	NavBlocker = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NavBlocker"));
	NavBlocker->SetCapsuleSize(50, 200);
	
	RootComponent = StaticMeshComponent;
	SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	ExitPoint->SetupAttachment(SkeletalMeshComponent);
	NavBlocker->SetupAttachment(StaticMeshComponent);
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
	NavBlocker->SetRelativeLocation({0,0,NavBlocker->GetUnscaledCapsuleHalfHeight()});
	NavBlocker->SetUsingAbsoluteScale(true);
	NavBlocker->SetUsingAbsoluteRotation(true);
	NavBlocker->SetUsingAbsoluteLocation(true);
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
	PlayerInputComponent->BindAction("RightBumper", IE_Pressed, PossessableComponent, &UPossesableComponent::RightBumper);
	PlayerInputComponent->BindAction("RightBumper", IE_Released, PossessableComponent, &UPossesableComponent::RightBumperRelease);
	PlayerInputComponent->BindAction("LeftBumper", IE_Pressed, PossessableComponent, &UPossesableComponent::LeftBumper);
	PlayerInputComponent->BindAction("LeftBumper", IE_Released, PossessableComponent, &UPossesableComponent::LeftBumperRelease);
	PlayerInputComponent->BindAction("StartButton", IE_Pressed, PossessableComponent, &UPossesableComponent::StartButton);
	PlayerInputComponent->BindAction("StartButton", IE_Released, PossessableComponent, &UPossesableComponent::StartButtonRelease);
	PlayerInputComponent->BindAxis("MoveRight", PossessableComponent, &UPossesableComponent::MoveRightAxis);
	PlayerInputComponent->BindAxis("MoveForward", PossessableComponent, &UPossesableComponent::MoveForwardAxis);
	PlayerInputComponent->BindAxis("LookRight", PossessableComponent, &UPossesableComponent::LookRightAxis);
	PlayerInputComponent->BindAxis("LookUp", PossessableComponent, &UPossesableComponent::LookUpAxis);
}

void APossessablePawn::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	FHitResult groundHit;
	GetWorld()->SweepSingleByChannel(groundHit,
									StaticMeshComponent->GetComponentLocation() + FVector{0,0,0},
									StaticMeshComponent->GetComponentLocation() - FVector{0,0, 100000000},
									FQuat::Identity,
									ECollisionChannel::ECC_GameTraceChannel4,
									FCollisionShape::MakeBox({NavBlocker->GetUnscaledCapsuleRadius()/2,
										NavBlocker->GetUnscaledCapsuleRadius()/2,1}));
	GroundHeight = groundHit.Distance;
	bOnGround = GroundHeight < 50;
	bAboveCapsule = GroundHeight > 2*NavBlocker->GetUnscaledCapsuleHalfHeight();
	NavBlocker->SetWorldLocation(StaticMeshComponent->GetComponentLocation() +
								 FVector(0, 0, NavBlocker->GetUnscaledCapsuleHalfHeight() - GroundHeight - 1));
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
		PossessableComponent->BeginningEndInteract();
		if(ExitPawn)
		{
			ghost_controller->Possess(ExitPawn);
			PossessableComponent->EndInteractInternal();
			ExitPawn->PossessableComponent->OnInteractInternal();
			
			ExitPawn = nullptr;
		}
		else
		{
			// move player pawn to the exit point and repossess
			APlayerPawn* new_pawn = ghost_controller->CreatePlayerPawn(GetActorLocation());
			//APlayerPawn* new_pawn = ghost_controller->CreatePlayerPawn(ExitPoint->GetComponentLocation());
			if (new_pawn)
			{
				ghost_controller->Possess(new_pawn);
				new_pawn->SetPlayer(ghost_controller);
				new_pawn->SetActorRotation(GetActorRotation());
				new_pawn->PlayPossessAnimation(false, ExitPoint->GetComponentTransform());
				PossessableComponent->EndInteractInternal();
			}
		}
		//CurrentPlayerController = nullptr;
	}
}

void APossessablePawn::HitByFlashlight()
{
	PossessableComponent->HitByFlashLight(); // new
}

void APossessablePawn::SetOutline(bool OutLine_ON,int depthInt)
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
