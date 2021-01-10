// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Polterheist/HUD/ControllerButton.h"
#include "ControllerButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UControllerButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/// The root of this widget. Preserves the aspect ratio.
	UPROPERTY(meta=(OptionalWidget))
	class UScaleBox* ScaleBox;

	/// Optionally, wrap the button texture with a border (background) image.
	UPROPERTY(meta=(OptionalWidget))
	class UBorder* Border;

	/// The image (or texture, if you will) that will be displayed.
	UPROPERTY(meta=(BindWidget))
	class UImage* ButtonImage;

public:
	UControllerButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void CreateControllerButton(EControllerButton Button);
};
