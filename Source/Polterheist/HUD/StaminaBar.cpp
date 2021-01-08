// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Components/ProgressBar.h"
#include "Polterheist/External/DefinedDebugHelpers.h"

UStaminaBar::UStaminaBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	V_LOG("Stamina Bar Regular Ctor");
}

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();
	V_LOG("Stamina Bar Native Ctor");
	if (StaminaBar)
	{
		StaminaBar->SetVisibility(ESlateVisibility::Visible);
		V_LOG("Visible Stam Bar");
	}
}

void UStaminaBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UStaminaBar::UpdateStaminaBar(float CurrentStaminaPercent)
{
	if (StaminaBar && StaminaBar->Visibility == ESlateVisibility::Visible)
	{
		StaminaBar->SetPercent(CurrentStaminaPercent);
	}
}
