// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractSystem/InteractableComponent.h"
#include "InteractSystem/ParanoiaComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class PossessableComponent;

/**
 * Ghostly player pawn.
**/
UCLASS()
class SUMMERDELIVERABLES_API APlayerPawn : public APawn
{
	GENERATED_BODY()
	// ==============================
	// ===== DEFINES_/_TYPEDEFS =====
	// ==============================

	// ======================================
	// ===== FRIEND_FUNCTIONS_/_CLASSES =====
	// ======================================

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
		/// The current bindings (the active input axis/actions and their code) of this player component
		/// bindings change from possession to possession
		PossessableComponent* CurrentBindings;

		/// An array of all interactable props within the bounding area of InteractBounds 
		TArray<UInteractableComponent*> OverlappingInteractables;

		/// An array of all props currently being selected from
		TArray<UParanoiaComponent*> SelectedProps;

		/// A boolean value; true if the player is currently checking for collisions with paranoia props
		bool lookingForParaProps = false;

		/// A TArray for holding the other found players to give them team stamina
		TArray<AActor*> FoundActors;
	
	protected:
	public:
		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate.*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float BaseTurnRate;

		/// The speed, in Unreal units, that the player pawn moves right and forward at
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float MovementSpeed;

		/// The current stamina that the Player has
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stamina)
		float Stamina;

		/// The bounding shape in which the Player can interact with other possessables whose bounding shape collides with this
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Collision)
		UShapeComponent * InteractBounds;

		//For Team Stamina
		UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassToFind; // Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there
	
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
		/// Sets default values for this pawn's properties
		APlayerPawn();
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
		/// Called every frame, put collision related code in here
		virtual void Tick(float DeltaTime) override;

		/// Called to bind functionality to the InputComponents
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		/// Handles interacting with interactable component derived objects
		UFUNCTION()
	    void Interact();

		/// Handles calling the action mapping for a currently possessed object
		UFUNCTION()
	    void ScareButtonStart();

		/// Handles cleaning up an interaction with a prop
		UFUNCTION()
	    void ScareButtonEnd();
		
		/// Returns a TArray of all interactable components currently within the player's InteractBounds
		UFUNCTION()
	    TArray<UInteractableComponent*> GetOverlappingInteractables() const
		{return OverlappingInteractables;}

		/// Returns a boolean value; whether or not the Player is currently possessing a prop
		UFUNCTION()
	    bool IsPossessing() const
		{return CurrentBindings != nullptr;}

		/// Handles moving the player along their Z axis
		/// positive #s move forward, negative backwards
		void MoveForward(float Value);

		/// Handles moving the player along their X axis
		/// positive #s move right, negative left
		void MoveRight(float Value);

		/// Handles turning the player's view around their Y axis
		/// positive #s turning right, negative left
		void LookRight(float Value);

		/// Handles turning the player's view around their X axis
		/// positive #s turning up, negative down
		void LookUp(float Value);

		/// An overlap function used to pickup stamina
		UFUNCTION()
	    void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                       bool bFromSweep, const FHitResult & SweepResult);
};
