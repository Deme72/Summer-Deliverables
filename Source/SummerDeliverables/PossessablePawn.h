#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "PossessablePawn.generated.h"

/// Anything that can be possessed is a Possessable Pawn.

UCLASS()
class SUMMERDELIVERABLES_API APossessablePawn : public APawn
{
    GENERATED_BODY()
public:
    // Constructor and destructor
	APossessablePawn();

protected:
	
	virtual void OnConstruction(const FTransform & Transform) override;
	
protected:
    // Reference to currently possessing player, used for un-possessing.
    class APlayerPawn* CurrentPlayer;
    
public:

	void setPlayer(APlayerPawn * p){CurrentPlayer = p;}
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Relocates the current player pawn to exit point and possesses it.
	virtual void EndPossession();
    
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ExitPoint;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPossesableComponent * PossessableComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class TSubclassOf<UPossesableComponent> PossessableComponentType;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent * SkeletalMeshComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent * StaticMeshComponent;
};
