// Fill out your copyright notice in the Description page of Project Settings.

//#include <activation.h>
#include "BaseEnemyCharacter.h"

#include "DefinedDebugHelpers.h"
#include "GameFramework/Actor.h"


// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemyCharacter::SetLastScareDirection()
{
	//if we're given the zero vector, set the scare direction to nondirectional
	if (LastScareLocation.X == 0 && LastScareLocation.Y == 0 && LastScareLocation.Z == 0)
	{    
		LastScareDirection = Nondirectional;
		return;
	}
	//calculate the angle
	FVector forwardVector = GetCapsuleComponent()->GetForwardVector(); //forward vector of the unit
	FVector scareVector = LastScareLocation-GetActorLocation();
	forwardVector = forwardVector.GetSafeNormal2D();
	scareVector = scareVector.GetSafeNormal2D();
	float dotProduct = FVector::DotProduct(forwardVector,scareVector);
	float angle = acos(dotProduct);
	if (angle < PI/4) //coming from the front
		LastScareDirection = Front;
	else if (angle > 3*PI/4) //coming from behind
		LastScareDirection = Back;
	else //coming from either left or right
	{
		FVector crossProduct = FVector::CrossProduct(forwardVector,scareVector);
		if (crossProduct.Z > 0)
			LastScareDirection = Right;
		else
			LastScareDirection = Left;
	}
}


void ABaseEnemyCharacter::SetEState(EState NewEState)
{
	if (NewEState != CurrentEState)
	{
		//adjust the character movement speed to account for the different state
		const float MovementSpeedMultiplier = GetStateSpeed(NewEState)/GetStateSpeed(CurrentEState);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Old Movement SPeed: %f"),FindComponentByClass<UCharacterMovementComponent>()[0].MaxWalkSpeed));    
		FindComponentByClass<UCharacterMovementComponent>()[0].MaxWalkSpeed *= MovementSpeedMultiplier;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("New Movement Speed: %f"),FindComponentByClass<UCharacterMovementComponent>()[0].MaxWalkSpeed));    
		//change the speed
		CurrentEState = NewEState;
		CurrentEStateTime = 0;

		if (NewEState == EState::Running)
		{
			CurrentRunningDuration = FMath::FRandRange(RunningDurationMin, RunningDurationMax);
		}
	}
}

float ABaseEnemyCharacter::GetStateSpeed(EState State)
{
	switch(State)
	{
		case EState::Searching:
			return SearchingMovementSpeed;
		break;
		case EState::Cautious:
			return CautiousMovementSpeed;
		break;
		case EState::Running:
			return RunningMovementSpeed;
		break;
		case EState::Stealing:
			return StealingMovementSpeed;
		break;
		case EState::Dying:
			return EscapingMovementSpeed;
		break;
		default:
			return 1;
	}
}

float ABaseEnemyCharacter::TakeBraveryDamage(float base_bravery_damage, FVector prop_position)
{
	float damage = (ScareBonus * (Paranoia / ParanoiaMax) + 1) * base_bravery_damage;
	damage += FMath::Max(Paranoia - ParanoiaMax, 0.0f) * ParanoiaOverflowDamage;
	damage *= (ComboCounter+1);
	Bravery = FMath::Max(Bravery - damage, 0.0f);

	ComboCounter += 1;
	ComboTimer = ScareComboInteraval;
	
	if (Bravery <= ScareRunningThreshold)
	{
		SetEState(EState::Running);
	}

	if (Bravery <= 0)
	{
		SetEState(EState::Dying);
	}

	LastScareLocation = prop_position;  
	SetLastScareDirection();
	return damage;
}

float ABaseEnemyCharacter::TakeParanoiaDamage(float ParanoiaDamage, FVector prop_position)
{
	Paranoia = Paranoia + ParanoiaDamage; /// Paranoia max isn't a clamp - based on the fact that paranoia overflow damage exists
	if (Paranoia >= ParanoiaRunningThreshold)
	{
		SetEState(EState::Running);
	}
	else if (Paranoia >= ParanoiaCautiousThreshold)
	{
		SetEState(EState::Cautious);
	}

	ParanoiaDecayTime = ParanoiaDecayDelay;

	LastScareLocation = prop_position;
	SetLastScareDirection();
	return ParanoiaDamage;
}

// Pick up treasure functionality
void ABaseEnemyCharacter::PickUpTreasure(AActor* treasure)
{
	treasure->FindComponentByClass<UStaticMeshComponent>()[0].SetSimulatePhysics(false);
	treasure->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("RightHandSocket"));
	treasure->SetActorEnableCollision(false);
	TreasureActor = treasure;
	SetEState(EState::Stealing);
}

// Drop treasure functionality
void ABaseEnemyCharacter::DropTreasure()
{
	if (TreasureActor != nullptr)
	{
		TreasureActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		TreasureActor->SetActorEnableCollision(true);	
		TreasureActor->FindComponentByClass<UStaticMeshComponent>()[0].SetSimulatePhysics(true);
		TreasureActor = nullptr;
	}
}

//Animation setting functionality
void ABaseEnemyCharacter::SetAnimation(AnimType anim,float animTime)
{
	CurrentAnim = anim;
	CurrentAnimTime = animTime;
	//halt movement if non-none animation, restore it if it is
	if (CurrentAnim != None && !isMovementHalted)
	{
		isMovementHalted = true;
		FindComponentByClass<UCharacterMovementComponent>()[0].MaxWalkSpeed *= .01;
	}
	if (CurrentAnim == None && isMovementHalted)
	{
		isMovementHalted = false;
		FindComponentByClass<UCharacterMovementComponent>()[0].MaxWalkSpeed *= 100;
	}
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Bravery = static_cast<float>(BraveryMax);
	Paranoia = 0.0f;

	ComboCounter = 0;
	ComboTimer = 0.0f;

	SetEState(EState::Searching);
}

// Called every frame
void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ComboTimer -= DeltaTime;
	if (ComboTimer < 0.0f)
	{
		ComboCounter = 0;
	}

	ParanoiaTick(DeltaTime);
	AnimationTick(DeltaTime);

	CurrentEStateTime += DeltaTime;
	if (CurrentEState == EState::Running && CurrentEStateTime > CurrentRunningDuration)
	{
		if (Paranoia >= ParanoiaCautiousThreshold)
		{
			SetEState(EState::Cautious);
		}
		else
		{
			SetEState(EState::Searching);
		}
	}
}

// Called to bind functionality to input
void ABaseEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

// Called before this enemy would be destroyed
void ABaseEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Super::EndPlay();
	if (IsValid(TreasureActor))
		TreasureActor->Destroy();
}

void ABaseEnemyCharacter::ParanoiaTick(float DeltaTime)
{
	if (ParanoiaDecayTime <= 0.0f)
	{
		if (CurrentEState == EState::Running)
		{
			Paranoia = FMath::Max(Paranoia - (ParanoiaRunningDecay * DeltaTime), 0.0f);
		}
		else
		{
			Paranoia = FMath::Max(Paranoia - (ParanoiaDecay * DeltaTime), 0.0f);
		}
		if (Paranoia < ParanoiaCautiousThreshold && CurrentEState == EState::Cautious)
		{
			SetEState(EState::Searching);
		}
	}
	else
	{
		float diff = DeltaTime - ParanoiaDecayTime;
		if (diff > 0.0)
		{
			if (CurrentEState == EState::Running)
			{
				Paranoia = FMath::Max(Paranoia - (ParanoiaRunningDecay * diff), 0.0f);
			}
			else
			{
				Paranoia = FMath::Max(Paranoia - (ParanoiaDecay * diff), 0.0f);
			}
		}
		ParanoiaDecayTime -= DeltaTime;
	}
}

void ABaseEnemyCharacter::AnimationTick(float DeltaTime)
{
	//if current anim time is greater than zero
	if (CurrentAnimTime > 0)
	{
		//reduce anim type
		CurrentAnimTime -= DeltaTime;
		FString result = FString::SanitizeFloat(CurrentAnimTime);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, result);
		//exit the anim if time hits zero
		if (CurrentAnimTime <= 0)
		{
			SetAnimation(None,0);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ending Animation!"));
		}
	}
}