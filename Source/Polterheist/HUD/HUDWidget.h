// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerButton.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	// ======================
	// ===== ATTRIBUTES =====
	// ======================
	public:
	/// The root of this HUD
	/// /// Note: UMG will throw warnings if this component is not present.
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* Canvas;

	/// Stamina bar widget that has to be added as a component in UMG Designer and named the same as this variable.
	/// Note: UMG will throw warnings if this component is not present.
	UPROPERTY(meta=(BindWidget))
	class UStaminaBar* StaminaBar;

	UPROPERTY(EditDefaultsOnly, Category="Customization")
	TMap<EControllerButton, class UTexture2D*> ButtonTextures;

	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	public:
	UHUDWidget(const FObjectInitializer& ObjectInitializer);

	// ===================
	// ===== METHODS =====
	// ===================
	public:
	/// Widgets don't have BeginPlay.
	/// NativeConstruct loads stuff after the constructor is done.
	virtual void NativeConstruct() override;

	void UpdateStaminaBar(float CurrentStaminaPercent);
};
