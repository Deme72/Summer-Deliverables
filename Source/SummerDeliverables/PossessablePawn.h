#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "PossessablePawn.generated.h"

UCLASS()
class SUMMERDELIVERABLES_API APossessablePawn : public APawn
{
    GENERATED_BODY()
    
public:
    // Constructor and destructor
	APossessablePawn();
	
	~APossessablePawn();
	
private:
    // Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
    // Reference to currently possessing player, used for un-possessing.
    class APlayerPawn* CurrentPlayer;
    
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    virtual void EndPossession();
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USceneComponent* ExitPoint;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UPossesableComponent* PossessableComponent;
	
};
