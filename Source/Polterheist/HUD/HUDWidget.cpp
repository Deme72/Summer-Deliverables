// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Polterheist/HUD/StaminaBar.h"
#include "Polterheist/Movement/PlayerGhostController.h"
#include "Polterheist/External/DefinedDebugHelpers.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	V_LOG("In HUDWidget NativeConstruct");
}

void UHUDWidget::UpdateStaminaBar(float CurrentStaminaPercent)
{
	if(StaminaBar)
	{
		StaminaBar->UpdateStaminaBar(CurrentStaminaPercent);
	}
}

