// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Movement/NavBlockingMovementComponent.h"
#include "PhysicsMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UPhysicsMovementComponent : public UNavBlockingMovementComponent
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
	float UpdateCounter;
	public:
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	UPhysicsMovementComponent();
	
	/// updates the raycast
	virtual void BeginPlay() override;
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
	/// Moves the owner
	/// Specification from this subclass adds raycastUpdates
	virtual FHitResult MoveOwner(FVector DeltaLoc) override;

	/// Wrapper for TeleportTo unreal function, teleports to the location and sets appropriate flags
	/// Specification from this subclass adds raycastUpdates
	virtual bool TeleportOwner(FVector loc, FRotator rot) override;

	/// Tick for the component
	/// If possessed updates the ray casts once per tick
	/// Otherwise updates once per second
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
