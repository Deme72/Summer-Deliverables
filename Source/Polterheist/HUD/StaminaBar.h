// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()

	// ======================
	// ===== ATTRIBUTES =====
	// ======================	
	public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* StaminaBar;

	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	public:
	UStaminaBar(const FObjectInitializer& ObjectInitializer);

	// ===================
	// ===== METHODS =====
	// ===================
	public:
	/// BeginPlay for Widgets. Runs after the constructor.
	virtual void NativeConstruct() override;

	/// Widget's way of ticking.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void UpdateStaminaBar(float CurrentStaminaPercent);
};
