// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "InteractSystem/UtilityPossessableComponent.h"
#include "Switchboard.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API USwitchboard : public UUtilityPossessableComponent
{
	GENERATED_BODY()

public:
	USwitchboard();

protected:
	int CurrRoomSelection;
	int MaxRooms;
	bool bIsNewInput;
	
public:
	void IncrementCurrRoomSelection(float val);

	virtual void MoveRightAxis_Implementation(float Axis) override;
	virtual void MoveButton_Implementation() override;
};
