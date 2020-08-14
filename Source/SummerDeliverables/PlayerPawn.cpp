// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/ShapeComponent.h"
#include "PossessablePawn.h"
#include "PossessableComponent.h"
#include "ParanoiaComponent.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for properties...
	BaseTurnRate = 1.f;
	Stamina = 1.f;

	// ... and components
	CurrentBindings = nullptr;
	// OverlappingInteractables = ?
}


// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OverlappingInteractables.Empty();
	TArray<AActor *> collisions = {};
	InteractBounds->GetOverlappingActors(collisions);
	for(auto i = collisions.begin(); i != collisions.end(); ++i)
	{
		UActorComponent * comp  = (*i)->FindComponentByClass(UInteractableComponent::StaticClass());
		if(comp)
		{
		UInteractableComponent * add = Cast<UInteractableComponent>(comp);
			if(add)
			{
				OverlappingInteractables.Add(add);
			}
		}
	}
	AddActorWorldOffset(ConsumeMovementInputVector());
	SetActorRotation(GetControlRotation());
}


// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerPawn::TurnAtRate);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerPawn::Interact);
	PlayerInputComponent->BindAction("FaceButtonBottom", IE_Pressed, this, &APlayerPawn::TakeAction);
	PlayerInputComponent->BindAction("FaceButtonBottom", IE_Released, this, &APlayerPawn::EndAction);
}


void APlayerPawn::Interact()
{
	UInteractableComponent * target = nullptr;
	for(auto i = OverlappingInteractables.begin(); i!= OverlappingInteractables.end(); ++i)
	{
		if(!target ||
			FVector::Dist((*i)->GetOwner()->GetActorLocation(), GetActorLocation())
			< FVector::Dist((*i)->GetOwner()->GetActorLocation(),target->GetOwner()->GetActorLocation()))
				target = *i;
	}
	if(target)
	{
		target->OnInteract();
		UPossesableComponent * comp = Cast<UPossesableComponent>(target);
		if(comp)
		{
			APossessablePawn * possess = Cast<APossessablePawn>(comp->GetOwner());
			check(possess);
			if(!possess->IsControlled())
			{
				GetController()->Possess(possess);
				possess->setPlayer(this);
			}
		}
		else
		{
			target->EndInteract();
		}
	}
}


void APlayerPawn::TakeAction()
{
#pragma region ParanoiaProps
	for(auto i = OverlappingInteractables.begin(); i!= OverlappingInteractables.end(); ++i)
	{
		UParanoiaComponent* paraProp = Cast<UParanoiaComponent>(*i);
		if(paraProp)
		{
			SelectedProps.Add(paraProp);
			paraProp->OnInteract();
		}
	}
#pragma endregion
	
}


void APlayerPawn::EndAction()
{
#pragma region ParanoiaProps
	for(auto& prop: SelectedProps)
	{
		prop->EndInteract();
	}
#pragma endregion
	
}


#pragma region Movement
void APlayerPawn::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerPawn::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerPawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
#pragma endregion 
