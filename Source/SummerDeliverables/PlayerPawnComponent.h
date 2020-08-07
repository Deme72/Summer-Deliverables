// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Components/ActorComponent.h"
#include "InputBindingComponent.h"
#include "PossessableComponent.h"
#include "PlayerPawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERDELIVERABLES_API UPlayerPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerPawnComponent();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool TakeAction();
	TArray<UInteractableComponent *> GetOverlappingInteractables();
	UFUNCTION(BlueprintCallable, Category="Getter")
	bool IsPossessing();
protected:
	
public:
	UPROPERTY()
	UInputBindingComponent * InputBindingComponent;
	UShapeComponent * InteractBounds;
private:
	float Stamina;
	UPossesableComponent * CurrentBindings = nullptr;
	TArray<UInteractableComponent *> OverlappingInteractables;
};
