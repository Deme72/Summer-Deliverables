// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Interactable/Core/StaticPossessableComponent.h"
#include "CodeReaderPossessableComponent.generated.h"

/**
* 
*/
UCLASS()
class POLTERHEIST_API UCodeReaderPossessableComponent : public UStaticPossessableComponent
{
	public:
	UCodeReaderPossessableComponent();
	void GenerateSequence();
	void CheckSequence(FString input);
	void SetDefaults();
	void AddCooldown();
	TArray<FString> MCodeString = {};
	TArray<FString> MString_Gen{"Button_Top","Button_Down","Left_Trigger", "Right_Trigger","Left_Move","Right_Move","Back_Move","Forward_Move"};
	bool MInprop;
	int MIndex = 0;
	float MScareMult=.75;
	bool MCanScare = false;
	float MTimeForSequence;
	float MInputCooldown=0;
	float MFailStamina=-30;
	float MaxScare = 2.5;
	GENERATED_BODY()
	protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void ScareButton_Implementation() override;
	virtual void OnInteract_Implementation() override;

	virtual void EndInteract_Implementation() override;

	virtual void ButtonTop_Implementation() override;

	virtual void MoveButton_Implementation() override;

	virtual void LeftTrigger_Implementation() override;

	virtual void RightTrigger_Implementation() override;

	virtual void MoveRightAxis_Implementation(float Axis) override;

	virtual void MoveForwardAxis_Implementation(float Axis) override;

	virtual void LookRightAxis_Implementation(float Axis) override;

	
};
