// Copyright Epic Games, Inc. All Rights Reserved.

#include "SummerDeliverablesGameMode.h"
#include "SummerDeliverablesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "InteractSystem/PlayerGhostController.h"

ASummerDeliverablesGameMode::ASummerDeliverablesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerPawn_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerGhostController::StaticClass();
}
