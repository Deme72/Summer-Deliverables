// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "ParanoiaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERDELIVERABLES_API UParanoiaComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParanoiaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UShapeComponent * paranoiaBounds = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float paranoiaAmount;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//>---- ? -----
	int uses; 			 // effectiveness modified by uses
	float timer; 		 // timer to reset uses after short while
	// (1/uses)+1;		 // formula for degradation
	//TArray<AEnemies>   // Effected enemies
	//<---- ? -----
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnInteract_Implementation() override; //this function is a default implementation and should never be called EndInteract should call it
	
	virtual void EndInteract_Implementation() override;  //this function is a default implementation and should never be called EndInteract should call it

	//UFUNCTION()		// need to implement some form of overlaps.
	//void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
