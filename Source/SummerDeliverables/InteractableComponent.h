// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SUMMERDELIVERABLES_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// constructor and destructor:
	// Sets default values for this component's properties
	UInteractableComponent();

private:
	// Private methods
	
protected:
	// Protected methods

public:
	//Public methods
	virtual void BluePrintInit(){}; //need to ask johnathan about how to do this

	UFUNCTION(BlueprintNativeEvent, Category="Event")
	void OnInteract();
	
	virtual void OnInteract_Implementation(){}; //this function is a default implementation and should never be called EndInteract should call it

	
	UFUNCTION(BlueprintNativeEvent, Category="Event")
    void EndInteract();
	
	virtual void EndInteract_Implementation(){}; //this function is a default implementation and should never be called EndInteract should call it

	UFUNCTION(BlueprintCallable, Category="Getter")
	virtual bool IsInUse(){return bInUse;}; //getter for bInUse
protected:
	// Private properties
	bool bInUse;
	
public:
	// Public properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UShapeComponent * InteractBounds;

		
};
