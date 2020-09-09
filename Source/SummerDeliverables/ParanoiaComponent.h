// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "ParanoiaComponent.generated.h"

// TODO: set up uses cooldown

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERDELIVERABLES_API UParanoiaComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParanoiaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UShapeComponent * ParanoiaBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float paranoiaAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float timer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float useCooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool active;


	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	int uses=0; 			 // effectiveness modified by uses
	float currentTime = 0;
	float usesCooldownTime = 0;
	// TODO: TArray<AEnemies>
	// TODO: Effected enemies

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnInteract_Implementation() override; //this function is a default implementation and should never be called EndInteract should call it
	
	virtual void EndInteract_Implementation() override;  //this function is a default implementation and should never be called EndInteract should call it
};
