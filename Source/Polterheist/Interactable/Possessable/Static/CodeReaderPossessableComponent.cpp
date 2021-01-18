// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeReaderPossessableComponent.h"
#include "Polterheist/External/DefinedDebugHelpers.h"
#include "Polterheist/External/DebugToolsSTDStringModifiers.h"
UCodeReaderPossessableComponent::UCodeReaderPossessableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UCodeReaderPossessableComponent::GenerateSequence()
{
	MCodeString.Empty();
	MIndex = 0;
	int rand = FMath::RandRange( 1,2 );
	for(int i = 0; i<rand; i++)
	{
		FString code = MString_Gen[FMath::RandRange(0,7)];
		MCodeString.Push(code);
		
		
	}
	SCREENMSGT(std::to_string("Start Input is "+MCodeString[0]).c_str(), 2);
}
void UCodeReaderPossessableComponent::AddCooldown()
{
	MInputCooldown =1;
}

void UCodeReaderPossessableComponent::CheckSequence(FString input)
{
	
	if(MInputCooldown<=0)
	{
		
		if(MIndex<MCodeString.Num())
		{
			if(MCodeString[MIndex] == input)
			{
				if(MIndex+1 < MCodeString.Num())
				{
					SCREENMSGT(std::to_string("next input is "+MCodeString[MIndex+1]).c_str(), 2);
				}
				MIndex++;
				if(MIndex<MCodeString.Num())
				{
					AddCooldown();
				}
				else
				{
					MScareMult = MScareMult + ((7/MTimeForSequence)*.5);
					SCREENMSGT("next input is " + std::to_string(MScareMult).c_str(), 2);
					MCanScare = true;
					GenerateSequence();
					//UI Stuff
					MTimeForSequence = 0;
					AddCooldown();
				}
			}
			else
			{
				GenerateSequence();
				MScareMult = MScareMult-.5;
				SetStamina(MFailStamina);
				AddCooldown();
			}
		}
	
	}
}
void UCodeReaderPossessableComponent::SetDefaults()
{
	MCodeString.Empty();
	MScareMult = .75;
	MCanScare = false;
	MInprop = false;
	MTimeForSequence = 0;
	MInputCooldown = 0;
	
}


// Called when the game starts
void UCodeReaderPossessableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetDefaults();
	// ...
	
}


// Called every frame
void UCodeReaderPossessableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if(MInprop)
	{
		MInputCooldown = MInputCooldown - DeltaTime;
		if(MScareMult>MaxScare)
		{
			MScareMult = MaxScare;
		}
		MTimeForSequence = MTimeForSequence+DeltaTime;
		if(MTimeForSequence>15)
		{
			GenerateSequence();
			MTimeForSequence = 0;
			SetStamina(-15);
		}
	}
	// ...
}
void UCodeReaderPossessableComponent::OnInteract_Implementation()
{
	MInprop = true;
	GenerateSequence();
}
void UCodeReaderPossessableComponent::EndInteract_Implementation()
{
	SetDefaults();
	
}
void UCodeReaderPossessableComponent::ScareButton_Implementation()
{
	if(MCanScare)
	{
		Scare(MScareMult);
		SetStamina(MFailStamina);
		Eject();
	}
}	
void UCodeReaderPossessableComponent::ButtonTop_Implementation()
{
	
	CheckSequence("Button_Top");
}
void UCodeReaderPossessableComponent::MoveButton_Implementation()
{
	CheckSequence("Button_Down");
}
void UCodeReaderPossessableComponent::LeftTrigger_Implementation()
{
	CheckSequence("Left_Trigger");
}
void UCodeReaderPossessableComponent::RightTrigger_Implementation()
{
	CheckSequence("Right_Trigger");
}
void UCodeReaderPossessableComponent::MoveRightAxis_Implementation(float Axis)
{
	if(Axis<0)
	{
		CheckSequence("Left_Move");
	}
	else if(Axis>0)
	{
		CheckSequence("Right_Move");
	}
}
void UCodeReaderPossessableComponent::MoveForwardAxis_Implementation(float Axis)
{
	if(Axis<0)
	{
		CheckSequence("Forward_Move");
	}
	else if(Axis>0)
	{
		CheckSequence(("Back_Move"));
	}
}
void UCodeReaderPossessableComponent::LookRightAxis_Implementation(float Axis)
{
	// UWorld* World = GetWorld();
	// const float Time = World->GetDeltaSeconds();
	// float roll = Time *50;
	// auto rotate = FRotator(0,0,roll);
	// //PLEASE FIX TEMPORARY
	// GetOwner()->FindComponentByClass<USpringArmComponent>()->AddLocalRotation(rotate);
	//
	
}

