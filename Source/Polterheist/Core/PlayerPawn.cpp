// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "Components/ShapeComponent.h"

#include "Polterheist/Interactable/Core/InteractableComponent.h"
#include "Polterheist/Interactable/Core/ParanoiaComponent.h"
#include "Polterheist/Interactable/Core/PossessableComponent.h"
#include "Polterheist/Interactable/Core/PossessablePawn.h"

#include "Polterheist/Enemy/FlashlightComponent.h"
#include "Polterheist/Movement/PlayerGhostController.h"
#include "Polterheist/External/External.h"
#include "Polterheist/Core/PlayerStamina.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

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

	LastMovementNormal = {0,0,0};
	
	bEntering = bExiting = false;

	Cast<APlayerGhostController>(GetController())->PlayerCameraManager;
	//Cast<APlayerGhostController>(GetController())->PlayerCameraManager->ViewPitchMin = -70.0f;
}

/// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	InteractBounds->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnBeginOverlap);
	InteractBounds->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::OnOverlapEnd);
	Cam = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
	PlayerMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}

void APlayerPawn::WhiskersRayCast(float DeltaTime)
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = GetActorLocation();
	FVector DirVector;
	FVector EndTrace;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	FRotator rot(0, 15, 0);
	FRotator startRot(0, 45, 0);
	//Right Side
	for(int i=0; i < 4; i++)
	{
		FRotator tempRot = rot * i + startRot;
		DirVector = tempRot.RotateVector(GetActorRightVector());
		EndTrace = (DirVector * Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength) + StartTrace;
		if(GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{

			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, 0.25);
			///Checks if the player is currently moving or has recently moved the camera
			if(GetInputAxisValue("MoveRight") > 0 &&GetWorldTimerManager().GetTimerRemaining(LookTimer) < 0.0f)
			{	
				AddControllerYawInput(10 * CurrentSpeed.Size() * BaseTurnRate * DeltaTime);
			}
		}
	}
	//Left Side
	for(int i=0; i < 4; i++)
	{
		FRotator tempRot = rot * -i - startRot;
		DirVector = tempRot.RotateVector(-GetActorRightVector());
		EndTrace = (DirVector * Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength) + StartTrace;
		//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, 0.25);
		if(GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, 0.25);
			///Checks if the player is currently moving or has recently moved the camera
			if(GetInputAxisValue("MoveRight") < 0 && GetWorldTimerManager().GetTimerRemaining(LookTimer) < 0.0f)
			{
				AddControllerYawInput(10 * CurrentSpeed.Size() * -BaseTurnRate * DeltaTime);
			}
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bEntering||bExiting)
	{
		if(AnimTimer <= 0.0)
		{
			if(bEntering)
				Possess(AnimPossess);
			bEntering = bExiting = false;
		}
		else
		{
			float lerp;
			if(bEntering)
			{
				lerp = 1 - AnimTimer/EnterTime;
			}
			else
			{
				lerp = 1 - AnimTimer/ExitTime;
			}
			
			SetActorLocation(FMath::Lerp(AnimStartPos.GetLocation(), AnimExitPos.GetLocation(), lerp));
			SetActorRotation(FMath::Lerp(AnimStartPos.GetRotation(), AnimExitPos.GetRotation(), lerp));
			
			AnimTimer-=DeltaTime;
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
			if(bLookingForParaProps)
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
#pragma region CameraTick
		WhiskersRayCast(DeltaTime);
		FVector CameraForward = Cam->GetForwardVector();
		CameraForward = {CameraForward.X, CameraForward.Y, 0};
		CameraForward.Normalize();
		FVector pforward = PlayerMesh->GetForwardVector();
		pforward = {pforward.X, pforward.Y, 0};
		pforward.Normalize();
		FVector pright = PlayerMesh->GetRightVector();
		pright = {pright.X, pright.Y, 0};
		pright.Normalize();
		float rightdp = FVector::DotProduct(pright,CameraForward);
		float forwarddp = FVector::DotProduct(pforward,CameraForward);
		if(rightdp < 0.99 && GetWorldTimerManager().GetTimerRemaining(LookTimer) < 0.0f)
		{
			float yawAmount = 0.5 - pow(FMath::RadiansToDegrees(acos(rightdp)) - 180, 2)/64800;
			if(forwarddp > 0)
			{	
				AddControllerYawInput( FMath::RadiansToDegrees(acos(rightdp) * yawAmount * DeltaTime));
			}
			else
			{
				AddControllerYawInput(-FMath::RadiansToDegrees(acos(rightdp) * yawAmount * DeltaTime));
			}
		}
#pragma endregion
		AddMovementVector(ConsumeMovementInputVector(), DeltaTime);
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
	if(!bEntering && !bExiting){
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
	{
		GetController()->Possess(possess);
		comp->OnInteractInternal();
		Destroy();
	}
}

void APlayerPawn::PlayPossessAnimation(bool enter, FTransform lerpLoc, class UPossesableComponent * comp)
{
	bEntering = enter;
	bExiting = !enter;
	AnimTimer = enter ? EnterTime : ExitTime;
	AnimStartPos = GetTransform();
	AnimExitPos = lerpLoc;
	AnimPossess = comp;
	
}


void APlayerPawn::ScareButtonStart()
{
	if(!bEntering || !bExiting)
		bLookingForParaProps = true;
}


void APlayerPawn::ScareButtonEnd()
{
	
#pragma region ParanoiaProps
	if(bLookingForParaProps)
	{
		bLookingForParaProps = false;
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

#pragma region Collisions

void APlayerPawn::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Most of these have classes associated with them. The tags are unnecessary and this can be cleaned up
	if(OtherActor->ActorHasTag("Stamina") && GetController() != nullptr)
	{
		APlayerGhostController* Con = Cast<APlayerGhostController>(GetController());
		Con->SetStamina(Cast<APlayerStamina>(OtherActor)->StaminaVal);
		OtherActor->Destroy();
	}
	if(OtherActor->ActorHasTag("TeamStamina") && GetController() != nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
		for(int i=0; i< FoundActors.Num(); i++)
		{
			Cast<APlayerGhostController>(((APlayerPawn*)FoundActors[i])->GetController())->SetStamina(Cast<APlayerStamina>(OtherActor)->TeamStaminaVal);
		}
		APlayerGhostController* Con = Cast<APlayerGhostController>(GetController());
		Con->SetStamina(Cast<APlayerStamina>(OtherActor)->StaminaVal);
		OtherActor->Destroy();
	}
	if(OtherActor->ActorHasTag("DynamicProp"))
	{
		APossessablePawn* dprop = Cast<APossessablePawn>(OtherActor);
		dprop->SetOutline(true,0);
	}
	if(OtherActor->ActorHasTag("StaticProp"))
	{
		APossessablePawn* sprop = Cast<APossessablePawn>(OtherActor);
		sprop->SetOutline(true,3);
	}
	if (OtherActor->ActorHasTag("ParanoiaProp"))
	{
		UParanoiaComponent* pprop = Cast<UParanoiaComponent>(OtherActor);
		pprop->SetOutline(true,2);
	}
}

void APlayerPawn::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("DynamicProp") || OtherActor->ActorHasTag("StaticProp"))
	{
		APossessablePawn* prop = Cast<APossessablePawn>(OtherActor);
		prop->SetOutline(false,0);
	}
	if (OtherActor->ActorHasTag("ParanoiaProp"))
	{
		UParanoiaComponent* pprop = Cast<UParanoiaComponent>(OtherActor);
		pprop->SetOutline(false,0);
	}
}
#pragma endregion

#pragma region stamina

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

void APlayerPawn::AddMovementVector(FVector in, float DeltaTime){
	static FRotator lastRotation = FRotator::ZeroRotator;
	if(in.Size() < DeadZone)
		in = {0,0,0};
	else if(in.Size() > 1)
		in.Normalize();
	FVector inNorm = in.GetSafeNormal();
	if(LastMovementNormal.Equals({0,0,0}) || FMath::RadiansToDegrees(acos(FVector::DotProduct(LastMovementNormal, inNorm))) > MovementLockAngle)
	{
		LastMovementNormal = inNorm;
		LockedCameraForward = Cam->GetForwardVector();
		LockedCameraForward = {LockedCameraForward.X, LockedCameraForward.Y, 0};
		LockedCameraForward.Normalize();
		
		LockedCameraRight = Cam->GetRightVector();
		LockedCameraRight = {LockedCameraRight.X, LockedCameraRight.Y, 0};
		LockedCameraRight.Normalize();
	}

	float mag = in.Size() * MovementSpeed;
	FVector wIn = mag * inNorm.X * LockedCameraForward + mag * inNorm.Y * LockedCameraRight;
	FVector dist = wIn - CurrentSpeed;
	float timeDiff = DeltaTime/MovementRamp;

	CurrentSpeed += dist * timeDiff;
	
	if(CurrentSpeed.Size() > MovementSpeed)
	{
		CurrentSpeed.Normalize();
		CurrentSpeed *= MovementSpeed;
	}
	UCapsuleComponent * cap = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	FHitResult groundHit;
	GetWorld()->SweepSingleByChannel(groundHit,
                                    cap->GetComponentLocation() + FVector{0,0,0},
                                    cap->GetComponentLocation() - FVector{0,0, 100000000},
                                    FQuat::Identity,
                                    ECollisionChannel::ECC_GameTraceChannel4,
                                    FCollisionShape::MakeBox({cap->GetUnscaledCapsuleRadius()/3 ,
                                        cap->GetUnscaledCapsuleRadius()/3,1}));
	FRotator rot;
	if(CurrentSpeed.Size() < 0.1)
	{
		rot = lastRotation;
	}
	else
	{
		rot = UKismetMathLibrary::FindLookAtRotation({0,0,0}, {CurrentSpeed.Y, -CurrentSpeed.X, 0});
		lastRotation = rot;
	}
	FHitResult moveHit;
	
	AddActorWorldOffset(CurrentSpeed,true, &moveHit);
	AddActorWorldOffset(FVector{0,0, (FloatHeight - groundHit.Distance) * FloatSpeed * DeltaTime},true);
	
	if(moveHit.bBlockingHit)
	{
		FVector backmove = CurrentSpeed.GetSafeNormal();
		AddActorWorldOffset(backmove *.1,true);
		FVector Normal = FVector::VectorPlaneProject(moveHit.Normal, {0,0,1}).GetSafeNormal();
		FVector newMove = FVector::VectorPlaneProject(CurrentSpeed, Normal);
		AddActorWorldOffset(newMove, true);
	}
	PlayerMesh->SetWorldRotation(rot);
}

void APlayerPawn::MoveForward(float Value)
{
	if(!bExiting && !bEntering)
		if ((Controller != NULL) && (Value != 0.0f))
			AddMovementInput({Value, 0,0});
}

void APlayerPawn::MoveRight(float Value)
{
	if(!bExiting && !bEntering)
		if ( (Controller != NULL) && (Value != 0.0f) )
			AddMovementInput({0, Value,0});
}


void APlayerPawn::LookRight(float Value)
{
	if(!bExiting && !bEntering)
		if ((Controller != NULL) && (Value != 0.0f)){
			AddControllerYawInput(Value);
		}
	if(Value != 0)
		GetWorldTimerManager().SetTimer(LookTimer,0.5f, false);
}

void APlayerPawn::LookUp(float Value)
{
	if(!bExiting && !bEntering)
		if ((Controller != NULL) && (Value != 0.0f)){
			//AddControllerPitchInput(Value * BaseTurnRate * 0.5 * GetWorld()->GetDeltaSeconds());
		}
	if(Value != 0)
		GetWorldTimerManager().SetTimer(LookTimer,0.5f, false);
}

//void APlayerPawn::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
#pragma endregion
