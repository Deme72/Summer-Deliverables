// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "InteractSystem/PossessablePawn.h"
#include "SwitchboardPawn.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API ASwitchboardPawn : public APossessablePawn
{
	GENERATED_BODY()

	ASwitchboardPawn();

public:	
	/// A pointer to the text representing room 1
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<UTextRenderComponent*> RoomText;

	virtual void EndPossession() override;
};
