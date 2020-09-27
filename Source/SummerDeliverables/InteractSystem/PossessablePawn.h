#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "PossessablePawn.generated.h"

/**
 * Contains all of the functionality specific to pawns for possession
 * Blueprint extensions change the visuals of the prop to match its functionality
**/
UCLASS()
class SUMMERDELIVERABLES_API APossessablePawn : public APawn
{
    GENERATED_BODY()
    	// ========================
    	// ===== ENUM_CLASSES =====
    	// ========================
    private:
    protected:
    public:
    	// ==========================
    	// ===== NESTED_CLASSES =====
    	// ==========================
    private:
    protected:
    public:
    	// ======================
    	// ===== ATTRIBUTES =====
    	// ======================
    private:
    protected:
    	/// Reference to currently possessing player controller, used for un-possessing.
    	//class APlayerGhostController* CurrentPlayerController;
	
    public:
    	/// A scenecomponent for providing an exit location to spawn an unpossessing player
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    	class USceneComponent* ExitPoint;

    	/// The attached component that allows this pawn to be a possessable
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    	class UPossesableComponent * PossessableComponent;

    	/// An in-editor drop down to choose which subclass of PossessableComponent to attach
    	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    	class TSubclassOf<UPossesableComponent> PossessableComponentType;

    	/// A pointer to attach a potential skeletal mesh
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    	class USkeletalMeshComponent * SkeletalMeshComponent;

    	/// A pointer to attach a potential static mesh
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    	class UStaticMeshComponent * StaticMeshComponent;
    	
    	// ======================================
    	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
    	// ======================================
    private:
    protected:
    	/// Called when an instance of this class is placed (in editor) or spawned.
    	virtual void OnConstruction(const FTransform & Transform) override;

		/// Called after components are initialized
		virtual void PostInitializeComponents() override;
	
    public:
    	/// Constructor and destructor
    	APossessablePawn();
    	
    	// =============================
    	// ===== GETTERS_/_SETTERS =====
    	// =============================
    private:
    protected:
    public:
    	/// Setter for CurrentPlayer pointer
    	// void SetPlayerController(APlayerGhostController * pc){CurrentPlayerController = pc;}

		/// Returns true if the pawn is currently possessed by a player
		UFUNCTION(BlueprintCallable, Category="Possession")
		bool IsPossessing() const { if(GetOwner()==nullptr){	return false;	}	return true;}
	
    	// ===================
    	// ===== METHODS =====
    	// ===================
    private:
    protected:
    public:
    	/// Called every frame
    	virtual void Tick(float DeltaTime) override;

    	/// Called to bind functionality to input
    	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
    	/// Relocates the current player pawn to exit point and possesses it.
    	virtual void EndPossession();
		
};