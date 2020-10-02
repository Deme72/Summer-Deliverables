// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "ParanoiaComponent.generated.h"

// TODO: set up uses cooldown

/**
 * Component to deal with the interactable objects that cause the enemies to be paranoid
**/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMERDELIVERABLES_API UParanoiaComponent : public UInteractableComponent
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
		/// How many times this paranoia component has been activated.
		/// Affects the amount of paranoia damage caused by this prop's use (negative paranoia damage to uses correlation)
		int uses;

		/// The current time in between uses of this prop
		/// if greater than useCooldown then the uses variable will decrement
		float usesCooldownTime;
	
	public:
		/// A bounding object that defines the area in which this object must collide with enemies to cause paranoia damage to them 
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
		UShapeComponent * ParanoiaBounds;

		/// The amount of Paranoia damage inflicted on enemies within the ParanoiaBounds
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float paranoiaAmount;

		/// The length of time a player must wait in between using this prop
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float useCooldown;

		/// Triggers the damage dealing call in Tick
		UPROPERTY(VisibleAnywhere, Category="Debug")
		bool active;
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	/// Called after to initialize this component
	virtual void InitializeComponent() override;
	public:
		/// Sets default values for this component's properties
		UParanoiaComponent();
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
		/// Called every frame, put collision functionality in here
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		/// A wrapper for the OnInteract, used to set relevant parameters before the event call
		virtual void OnInteractInternal() override; 

		/// A wrapper for the EndInteract, used to set relevant parameters before the event call
		virtual void EndInteractInternal() override; 
	
	
};
