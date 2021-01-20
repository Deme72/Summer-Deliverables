// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SpookyMovementComponent.generated.h"

/**
 * The base class for all movement components in this project
 * Name is a bit off but everything else is generic so it has to stand out
 */
UCLASS()
class POLTERHEIST_API USpookyMovementComponent : public UPawnMovementComponent
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
	///The forward normal of the last movement operation
	FVector MovementForwardNormal;
	
	///The distance the prop is off of the ground
	float GroundHeight;

	///The width of the raycast for ground distance
	float RaycastWidth;
	
	///Whether or not the last raycast data is old
	bool bDirtyRaycast;
	
	///Is the prop on the ground
	bool bOnGround;
	public:
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	USpookyMovementComponent();
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	/// Updates the data from the raycast
	UFUNCTION(BlueprintCallable, Category="Setter")
	virtual bool UpdateRaycastData();
	
	/// Returns true if the pawn is on the ground
	UFUNCTION(BlueprintCallable, Category="Getter")
    bool IsOnGround() const {return bOnGround;}

	/// Gets the owner's static mesh
	UFUNCTION(BlueprintCallable, Category="Getter")
    UStaticMeshComponent * GetOwnerMesh() const {return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));};

	/// Returns the distance above the ground
	UFUNCTION(BlueprintCallable, Category="Getter")
	float GetDistFromGround() const {return GroundHeight;}

	/// Returns the forward vector of the last movement
	UFUNCTION(BlueprintCallable, Category="Getter")
	FVector GetMovementForward(){return MovementForwardNormal;}
	
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
	virtual FHitResult MoveOwner(FVector DeltaLoc);

	/// Wrapper for TeleportTo unreal function, teleports to the location and sets appropriate flags
	virtual bool TeleportOwner(FVector loc, FRotator rot);

	/// Takes move input and applies calculations and deadzones to make it camera relative
	FVector AllignInputToCamera(FVector in, FVector camForward, FVector camRight, float speed, float DeadZone = 0.2);
};
