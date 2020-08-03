// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PossesableComponent.h"
#include "Components/InputComponent.h"
#include "InputBindingComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UInputBindingComponent : public UInputComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Bindings")
	void BindInput(UPossesableComponent * comp);
	UFUNCTION(BlueprintCallable, Category="Bindings")
    void UnBindInput(UPossesableComponent * comp);
};
