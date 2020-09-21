// Fill out your copyright notice in the Description page of Project Settings.

//#include <activation.h>
#include "BaseEnemyCharacter.h"


// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEnemyCharacter::SetEState(EState NewEState)
{
	if (NewEState != CurrentEState)
	{
		CurrentEState = NewEState;
		CurrentEStateTime = 0;

		if (NewEState == EState::Running)
		{
			CurrentRunningDuration = FMath::FRandRange(RunningDurationMin, RunningDurationMax);
		}
	}
}

void ABaseEnemyCharacter::TakeBraveryDamage(float BraveryBaseDamage)
{
	float damage = (ScareBonus * (Paranoia / ParanoiaMax) + 1) * BraveryBaseDamage;
	damage += FMath::Max(Paranoia - ParanoiaMax, 0.0f) * ParanoiaOverflowDamage;
	damage *= ComboCounter;
	Bravery = FMath::Max(Bravery - damage, 0.0f);

	ComboCounter += 1;
	ComboTimer = ScareComboInteraval;
	
	if (Bravery <= ScareRunningThreshold)
	{
		SetEState(EState::Running);
	}
}

void ABaseEnemyCharacter::TakeParanoiaDamage(float ParanoiaDamage)
{
	Paranoia = FMath::Max(Paranoia - ParanoiaDamage, 0.0f);
	if (Paranoia <= ParanoiaCautiousThreshold)
	{
		SetEState(EState::Cautious);
	}
	else if (Paranoia <= ParanoiaRunningThreshold)
	{
		SetEState(EState::Running);
	}

	ParanoiaDecayTime = ParanoiaDecayDelay;
}

void ABaseEnemyCharacter::PickUpTreasure(AActor* treasure)
{
	// Pick up treasure functionality
}

void ABaseEnemyCharacter::DropTreasure()
{
	
	// Drop treasure functionality
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

	ParanoiaDecayTime -= DeltaTime;

	CurrentEStateTime += DeltaTime;
	if (CurrentEState == EState::Running && CurrentEStateTime > CurrentRunningDuration)
	{
		if (Paranoia < ParanoiaCautiousThreshold)
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

void ABaseEnemyCharacter::ParanoiaTick(float DeltaTime)
{
	while (DeltaTime > 0.0f)
	{
		if (ParanoiaDecayTime < 0.0f)
		{
			if (CurrentEState == EState::Running)
			{
				Paranoia = FMath::Max(Paranoia - (ParanoiaRunningDecay * DeltaTime), 0.0f);
			}
			else
			{
				Paranoia = FMath::Max(Paranoia - (ParanoiaDecay * DeltaTime), 0.0f);
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
}

