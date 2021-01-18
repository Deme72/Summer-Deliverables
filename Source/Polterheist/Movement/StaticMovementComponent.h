// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Movement/NavBlockingMovementComponent.h"
#include "StaticMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UStaticMovementComponent : public UNavBlockingMovementComponent
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
	public:
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	UStaticMovementComponent();

	/// updates the raycast
	virtual void BeginPlay() override {UpdateRaycastData();};
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
	/// This shouldn't be used for this component so it errors
	virtual FHitResult MoveOwner(FVector DeltaLoc) override;

	/// Wrapper for TeleportTo unreal function, teleports to the location and sets appropriate flags
	/// Specification from this subclass adds raycastUpdates
	virtual bool TeleportOwner(FVector loc, FRotator rot) override;
};
