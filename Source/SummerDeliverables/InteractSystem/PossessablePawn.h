#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "SummerDeliverables/PlayerPawn.h"

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

		///Pawn to possess on exit
		APossessablePawn * ExitPawn;

		///Is the prop on the ground
		bool bOnGround;

		///Is the prop above the capsule
		bool bAboveCapsule;

		///The distance the prop is off of the ground
		float GroundHeight;
		
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
		
		/// A capsule to block out any AI movement into this pawn.
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCapsuleComponent* NavBlocker;
    	
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
		///Sets outline properties.
		UFUNCTION(BlueprintCallable, Category="Outline")
	    void Set_Outline(bool OutLine_ON,int depthInt);
		//depthInt A number which will tell the outline material when we set the depth on the mesh which color to use.
    	
    	/// Setter for CurrentPlayer pointer
    	// void SetPlayerController(APlayerGhostController * pc){CurrentPlayerController = pc;}


		/// Sets a pawn to exit to
		UFUNCTION(BlueprintCallable, Category="Getters")
	    void SetNextExit(APossessablePawn * pawn){ExitPawn  = pawn;};
	
		/// Returns true if the pawn is currently possessed by a player
		UFUNCTION(BlueprintCallable, Category="Possession")
		bool IsPossessing() const { return IsPlayerControlled();}

		/// Returns true if the pawn is on the ground
		UFUNCTION(BlueprintCallable, Category="Movement")
	    bool IsOnGround() const {return bOnGround;}

		/// Returns true if the pawn is above the capsule bounds
		UFUNCTION(BlueprintCallable, Category="Movement")
	    bool IsAboveCapsule() const {return bAboveCapsule;}

		/// Returns the distance above the ground
		UFUNCTION(BlueprintCallable, Category="Movement")
	    float GetDistFromGround() const {return GroundHeight;}

		/// Sets the mesh to be a certain distance from the ground
		void SetDistFromGround(float f)
		{StaticMeshComponent->AddLocalOffset({0, 0, f-GroundHeight}, true);}

		///
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

		/// Creates a sub pawn
		UFUNCTION(BlueprintCallable, Category="Possession")
		APossessablePawn* SpawnSubPawn(TSubclassOf<APossessablePawn> subclass, FTransform const pos);
		
};