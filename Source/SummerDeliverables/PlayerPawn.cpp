// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/ShapeComponent.h"
#include "InteractSystem/FlashlightComponent.h"
#include "InteractSystem/PossessablePawn.h"
#include "InteractSystem/ParanoiaComponent.h"
#include "InteractSystem/PlayerGhostController.h"
#include "Kismet/GameplayStatics.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "PlayerStamina.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for properties...
	BaseTurnRate = 1.f;
	MovementSpeed = 1.f;

	// ... and components
	CurrentBindings = nullptr;
	OverlappingInteractables = {};

	entering = exiting = false;
}

/// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	InteractBounds->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnBeginOverlap);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(entering||exiting)
	{
		if(animTimer <= 0.0)
		{
			if(entering)
				Possess(animPossess);
			entering = exiting = false;
		}
		else
		{
			float lerp = 1 - animTimer/(entering?enterTime:exitTime);
			SetActorLocation(FMath::Lerp(animStartPos.GetLocation(), animExitPos.GetLocation(), lerp));
			SetActorRotation(FMath::Lerp(animStartPos.GetRotation(), animExitPos.GetRotation(), lerp));
			animTimer-=DeltaTime;
		}
	}
	else
	{
		OverlappingInteractables.Empty();
		TArray<AActor *> collisions = {};
		InteractBounds->GetOverlappingActors(collisions);
		for(auto i = collisions.begin(); i != collisions.end(); ++i)
		{
			UActorComponent * comp  = (*i)->FindComponentByClass(UInteractableComponent::StaticClass());
			if(comp)
			{
				UInteractableComponent * add = Cast<UInteractableComponent>(comp);
				check(add);
				OverlappingInteractables.Add(add);
			}
			UActorComponent * comp2  = (*i)->FindComponentByClass(UFlashlightComponent::StaticClass());
			if(comp2)
			{
				APlayerGhostController* Con = Cast<APlayerGhostController>(GetController());
				Con->SetStamina(-20.0f * DeltaTime);
			}
		
			// PARANOIA
			if(lookingForParaProps)
			{
				UParanoiaComponent* paraProp = Cast<UParanoiaComponent>((*i)->FindComponentByClass(UParanoiaComponent::StaticClass()));
				if(paraProp && !paraProp->IsInUse())
				{
					UParanoiaComponent* add = Cast<UParanoiaComponent>(paraProp);
					check(add);
					if(!SelectedProps.Contains(add))
					{
						if(SelectedProps.Num() == 0)
						{
							SelectedProps.Add(add);
							add->OnInteractInternal();
						}
						else if (CanAffordStaminaCost(ParanoiaCost))
						{
							SetStamina(-ParanoiaCost);
							SelectedProps.Add(add);
							add->OnInteractInternal();
						}
					}
				}
			}
		}
		AddActorWorldOffset(ConsumeMovementInputVector(), true);
		SetActorRotation(GetControlRotation());
	}
}


// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerPawn::LookRight); 
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerPawn::LookUp); 
	PlayerInputComponent->BindAction("InteractButton", IE_Released, this, &APlayerPawn::Interact);
	PlayerInputComponent->BindAction("ScareButton", IE_Pressed, this, &APlayerPawn::ScareButtonStart);
	PlayerInputComponent->BindAction("ScareButton", IE_Released, this, &APlayerPawn::ScareButtonEnd);
}


void APlayerPawn::Interact()
{
	if(!entering && !exiting){
		UInteractableComponent * target = nullptr;
		for(auto interactable: OverlappingInteractables)
		{
			if(!target ||
                FVector::Dist(interactable->GetOwner()->GetActorLocation(), GetActorLocation())
                < FVector::Dist(interactable->GetOwner()->GetActorLocation(),target->GetOwner()->GetActorLocation()))
                	if(!Cast<UParanoiaComponent>(interactable))
                		target = interactable;
		}
		if(target && !target->IsInUse())
		{
		
			UPossesableComponent * comp = Cast<UPossesableComponent>(target);
			if(comp)
			{
				auto ghost_controller = Cast<APlayerGhostController>(GetController());
				auto possessable = Cast<UPossesableComponent>(comp);
				if (ghost_controller->CanAffordStaminaCost(possessable->GetFrontStaminaCost()))
				{
					ghost_controller->SetStamina(-possessable->GetFrontStaminaCost());
					PlayPossessAnimation(true, comp->GetOwner()->GetTransform(), comp);
				}
			}
			else
			{
				target->OnInteractInternal();
				target->EndInteractInternal();
			}
		}
	}
}


void APlayerPawn::Possess(class UPossesableComponent * comp)
{
	APossessablePawn * possess = Cast<APossessablePawn>(comp->GetOwner());
	check(possess);
	auto ghost_controller = Cast<APlayerGhostController>(GetController());
	auto possessable = Cast<UPossesableComponent>(comp);
	if(ghost_controller && possessable)
		if (ghost_controller->CanAffordStaminaCost(possessable->GetFrontStaminaCost()))
		{
			comp->OnInteractInternal();
			GetController()->Possess(possess);
			Destroy();
		}
}

void APlayerPawn::PlayPossessAnimation(bool enter, FTransform lerpLoc, class UPossesableComponent * comp)
{
	entering = enter;
	exiting = !enter;
	animTimer = enter ? enterTime : exitTime;
	animStartPos = GetTransform();
	animExitPos = lerpLoc;
	animPossess = comp;
}


void APlayerPawn::ScareButtonStart()
{
	if(!entering || !exiting)
		lookingForParaProps = true;
}


void APlayerPawn::ScareButtonEnd()
{
	
#pragma region ParanoiaProps
	if(lookingForParaProps)
	{
		lookingForParaProps = false;
		for(auto& prop: SelectedProps)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Prop Went Off"));
			}
		
			prop->EndInteractInternal();
		}
		SelectedProps.Empty();
	}
#pragma endregion
	
}

#pragma region stamana

void APlayerPawn::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(OtherActor->ActorHasTag("Stamina"))
	{
		APlayerGhostController* Con = Cast<APlayerGhostController>(GetController());
		Con->SetStamina(Cast<APlayerStamina>(OtherActor)->StaminaVal);
		OtherActor->Destroy();
	}
	if(OtherActor->ActorHasTag("TeamStamina"))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
		for(int i=0; i< FoundActors.Num(); i++)
		{
			Cast<APlayerGhostController>(((APlayerPawn*)FoundActors[i])->GetController())->SetStamina(Cast<APlayerStamina>(OtherActor)->StaminaVal);
		}
		OtherActor->Destroy();
	}
}

float APlayerPawn::GetStamina() const
{
	AController* controller = GetController();
	if (controller != nullptr)
	{
		APlayerGhostController* ghost_controller = Cast<APlayerGhostController>(controller);
		if (ghost_controller)
			return ghost_controller->GetStamina();
		else 
			SCREENMSG("Controller isn't of APlayerGhostController");
	}
	return 0.0f;
}

bool APlayerPawn::SetStamina(float stamina_drain, bool b_is_relative) const
{
	return Cast<APlayerGhostController>(GetController())->SetStamina(stamina_drain, b_is_relative);
}

bool APlayerPawn::CanAffordStaminaCost(const float stamina_cost) const
{
	return Cast<APlayerGhostController>(GetController())->CanAffordStaminaCost(stamina_cost);
}
#pragma endregion 

#pragma region Movement

void APlayerPawn::MoveForward(float Value)
{
	if(!exiting && !entering)
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value * MovementSpeed);
		}
}

void APlayerPawn::MoveRight(float Value)
{
	if(!exiting && !entering)
		if ( (Controller != NULL) && (Value != 0.0f) )
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
		
			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value * MovementSpeed);
		}
}


void APlayerPawn::LookRight(float Value)
{
	if(!exiting && !entering)
		if ((Controller != NULL) && (Value != 0.0f))
		{
			AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		}
}

void APlayerPawn::LookUp(float Value)
{
	if(!exiting && !entering)
		if ((Controller != NULL) && (Value != 0.0f))
		{
			AddControllerPitchInput(Value * BaseTurnRate * 0.5 * GetWorld()->GetDeltaSeconds());
		}
}

//void APlayerPawn::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
#pragma endregion
