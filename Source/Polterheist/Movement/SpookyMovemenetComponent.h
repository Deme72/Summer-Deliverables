// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SpookyMovemenetComponent.generated.h"

/**
 * The base class for all movement components in this project
 * Name is a bit off but everything else is generic so it has to stand out
 */
UCLASS()
class POLTERHEIST_API USpookyMovemenetComponent : public UPawnMovementComponent
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
	///Is the prop on the ground
	bool bOnGround;

	///The distance the prop is off of the ground
	float GroundHeight;

	///The width of the raycast for ground distance
	float RaycastWidth;
	
	///Whether or not the last raycast data is old
	bool bDirtyRaycast;
	public:
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	USpookyMovemenetComponent();
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	///Updates the data from the raycast
	UFUNCTION(BlueprintCallable, Category="Setter")
	virtual bool UpdateRaycastData();
	
	/// Returns true if the pawn is on the ground
	UFUNCTION(BlueprintCallable, Category="Getter")
    bool IsOnGround() const {return bOnGround;}

	/// Gets the owner's static mesh
	///UFUNCTION(BlueprintCallable, Category="Getter")
    UStaticMeshComponent * GetOwnerMesh() const {return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));};

	/// Returns the distance above the ground
	UFUNCTION(BlueprintCallable, Category="Getter")
	float GetDistFromGround() const {return GroundHeight;}

	/// Sets the mesh to be a certain distance from the ground
	UFUNCTION(BlueprintCallable, Category="Setter")
	void SetDistFromGround(float f){MoveOwner({0, 0, f-GroundHeight});};
	
	/// Sets the width of the raycast, use the bounding collision volume's width
	UFUNCTION(BlueprintCallable, Category="Setter")
	void setRayWidth(float w){RaycastWidth = w;};

	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
	/// Moves the owner
	void MoveOwner(FVector DeltaLoc, FHitResult * Hit = nullptr);
};
