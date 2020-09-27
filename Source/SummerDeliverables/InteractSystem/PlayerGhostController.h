// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerGhostController.generated.h"

/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API APlayerGhostController : public APlayerController
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
	/// The Player's current Stamina
	float CurrentStamina;

	protected:
	public:
	/// The Player's max Stamina
	UPROPERTY(EditAnywhere, Category="Stamina", meta=(ClampMin=0.0))
	float MaxStamina;

	/// The type of APlayerPawn that Player Controller spawns when unpossessing a possessable
	UPROPERTY(EditAnywhere, Category="Possession")
	TSubclassOf<class APlayerPawn> PlayerPawn; 
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	/// The default Constructor
	APlayerGhostController();
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	/// Gets the player's current stamina
	UFUNCTION(BlueprintCallable, Category="Getters")
	float GetStamina() const { return CurrentStamina; }

	/// Subtracts the passed stamina value from the player's current stamina |
	/// b_is_relative dictates with the action is relative (i.e. CurrentStamina -= stamina_drain)
	/// or is absolute (i.e. CurrentStamina = stamina_drain) |
	/// returns true if the player has no stamina left
	UFUNCTION(BlueprintCallable, Category="Setters")
    bool SetStamina(float stamina_drain, bool b_is_relative = true);
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
	/// Creates an APlayerPawn at the given location and returns the pointer
	UFUNCTION(BlueprintCallable, Category="Possession")
	class APlayerPawn* CreatePlayerPawn(const FVector spawn_location) const;

	/// Returns true if the player currently has enough stamina to afford the stamina cost
	UFUNCTION(BlueprintCallable, Category="Possession")
	bool CanAffordStaminaCost(const float stamina_cost) const {return CurrentStamina - stamina_cost > 0.0;}
	
	virtual void BeginPlay() override;
};

