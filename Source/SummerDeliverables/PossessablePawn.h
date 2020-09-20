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

	UFUNCTION(BlueprintCallable, Category="Outline")
	void Set_Outline(bool OutLine_ON,int depthInt); //depthInt Basically a number which will tell the outline material when we set the depth on the mesh which color to use.

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USceneComponent* ExitPoint;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPossesableComponent * PossessableComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class TSubclassOf<UPossesableComponent> PossessableComponentType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent * SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * StaticMeshComponent;
};
