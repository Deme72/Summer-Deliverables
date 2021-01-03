// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * Every player controller has their own instance of this HUD class.
 * This class manages all the widgets drawn on each player's individual viewport.
 * The purpose of this class is to decouple any HUD/UI related code
 * from the rest of the codebase. So just treat this as a manager.
 */
UCLASS()
class POLTERHEIST_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
};
