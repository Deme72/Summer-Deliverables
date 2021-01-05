// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Movement/SpookyMovemenetComponent.h"
#include "NavBlockingMovemenetComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UNavBlockingMovemenetComponent : public USpookyMovemenetComponent
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
	///Is the prop above the capsule
	bool bAboveCapsule;
	public:
	/// A capsule to block out any AI movement into this pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent * NavBlocker;
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	///Constructor, create the nav blocker and set up its params
	UNavBlockingMovemenetComponent();

	///Initialize the component
	virtual void InitializeComponent() override;
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	/// Updates the raycast data and the nav blocker location
	virtual bool UpdateRaycastData() override;

	/// Returns true if the pawn is above the capsule bounds
	UFUNCTION(BlueprintCallable, Category="Getter")
    bool IsAboveCapsule() const {return bAboveCapsule;}

	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
};
