// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

/**
 * Base class for anything the player uses the interact button to interact with 
 **/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SUMMERDELIVERABLES_API UInteractableComponent : public UActorComponent
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
	protected:
		///True if the component is being used
		UPROPERTY(VisibleAnywhere)
		bool bInUse;
	public:
		///Bounds for interactions
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
		UShapeComponent * InteractBounds;
	
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
		// constructor and destructor:
		// Sets default values for this component's properties
		UInteractableComponent();
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	///getter for bInUse
		UFUNCTION(BlueprintCallable, Category="Getter")
	    virtual bool IsInUse() const {return bInUse;};
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
		//Public methods
		//virtual void BluePrintInit(){}; //not needed for now

		///The playerPawn should call this c++ function which then calls the overwritten Blueprint OnInteract function
		virtual void OnInteractInternal();

		///This function should not be called from other c++ files
		UFUNCTION(BlueprintNativeEvent, Category="Event")
	    void OnInteract();
		
		virtual void OnInteract_Implementation(){}; //this function is a default implementation and should never be called OnInteract should call it

		///The playerPawn should call this c++ function which then calls EndInteract, other c++ files can call this
		virtual void EndInteractInternal();

		///This function should not be called from other c++ files
		UFUNCTION(BlueprintNativeEvent, Category="Event")
	    void EndInteract();
		
		///this function is a default implementation and should never be called EndInteract should call it
		virtual void EndInteract_Implementation(){}; 

		
};
