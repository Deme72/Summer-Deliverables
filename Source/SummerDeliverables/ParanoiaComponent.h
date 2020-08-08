// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParanoiaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERDELIVERABLES_API UParanoiaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParanoiaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UShapeComponent * paranoiaBounds = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float paranoiaAmount;

//>---- ? -----
	float effectiveness; // a detractor of the paranoiaAmount
	int uses; 			 // effectiveness modified by uses
	float timer; 		 // timer to reset uses after short while
//<---- ? -----

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, Category="Event")
	void OnInteract();
	
	virtual void OnInteract_Implementation(){}; //this function is a default implementation and should never be called EndInteract should call it

	
	UFUNCTION(BlueprintNativeEvent, Category="Event")
    void EndInteract();
	
	virtual void EndInteract_Implementation(){}; //this function is a default implementation and should never be called EndInteract should call it

		
};
