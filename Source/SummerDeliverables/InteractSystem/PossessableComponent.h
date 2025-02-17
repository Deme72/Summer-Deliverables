// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "GameFramework/Pawn.h"
#include "PossessableComponent.generated.h"


/**
 * 
 */
UCLASS()
class SUMMERDELIVERABLES_API UPossesableComponent : public UInteractableComponent
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
		/// The time, in seconds, since this component has been used
		float CurrentCooldown;

		/// Is per second stamina drain active
		bool bIsDrainingStamina;
	
	public:
		/// The rate, in stamina/sec, at which this prop drains the player's stamina
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamDrainRate;

		/// The initial stamina cost required by a player to possess this prop
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float StamFrontCost;

		/// The length of time, in seconds, a Player must wait in between using this prop's abilities
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
		float MaxCooldown;

		/// The amount of Paranoia damage this prop causes to enemies
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float ParanoiaAmount;

		/// The bounding shape in which this prop must collide with enemies to cause paranoia and/or health damage
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		UShapeComponent * ParanoiaBounds;

		/// The amount of health damage this prop causes to enemies 
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float DamageAmount;

		/// The bounding shape in which this prop must collide with enemies to cause paranoia and/or health damage
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		UShapeComponent * DamageBounds;
	
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	/// Default constructor.
    UPossesableComponent();
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	protected:
	public:
	UFUNCTION(BlueprintCallable, Category="Stamina")
	/// Sets whether the possession is actively draining stamina w/ StaminaDrainRate
	void SetActiveStaminaDrain(bool is_active) { bIsDrainingStamina = is_active; }

	UFUNCTION(BlueprintCallable, Category="Stamina")
    /// Sets whether the possession is actively draining stamina w/ StaminaDrainRate
    bool GetIsActiveStaminaDrain() const { return bIsDrainingStamina; }
	
	UFUNCTION(BlueprintCallable, Category="Getters")
    /// Gets the player's current stamina
    float GetStamina();
	
	UFUNCTION(BlueprintCallable, Category="Getters")
    /// Adds the passed stamina value to the possessing player's current stamina |
    /// b_is_relative dictates with the action is relative (i.e. CurrentStamina += stamina_drain) or is absolute (i.e. CurrentStamina = stamina_drain) |
    /// returns true if the player has no stamina left
    bool SetStamina(float delta_stamina, bool b_is_relative = true);

	/// Returns true if the current possessing player has enough stamina to afford the stamina_cost
	UFUNCTION(BlueprintCallable, Category="Possession")
    bool CanAffordStaminaCost(const float stamina_cost) const;
	
	UFUNCTION(BlueprintCallable, Category="Getters")
	/// Returns the up-front stamina cost of this possessable
	float GetFrontStaminaCost() const { return StamFrontCost; }

	UFUNCTION(BlueprintCallable, Category="Getters")
	class APlayerGhostController* GetCurrentPlayer() const;

	
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
		/// The UE4 Tick Component for handling realtime actions
		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

		/// A component-side wrapper for endPossession
		UFUNCTION(BlueprintCallable, Category="Possessing")
		void Eject() const;

		/// Function to deal damage
		UFUNCTION(BlueprintCallable, Category="Damage")
	    void Scare(float baseMultiplier = 1.0) const;

		/// Function to spawn a sub Pawn
		UFUNCTION(BlueprintCallable, Category="Possessing")		
		void SpawnSubPawn(TSubclassOf<class APossessablePawn> pawn, FTransform pos) const;

		/// Function to spawn a sub Pawn
		UFUNCTION(BlueprintCallable, Category="Possessing")		
		void SetNextExit(class APossessablePawn* pawn) const;

		/// Called at the beginning of the EndInteract event within PossessablePawn
		UFUNCTION(BlueprintCallable, Category="Unpossessing")
		virtual void BeginningEndInteract() {};

	// === Input Event Functions ===
		/// An Event for when the Possessable Pawn is hit by a flashlight
		UFUNCTION(BlueprintNativeEvent, Category="GameplayEvents")
		void HitByFlashLight(); // new

		/// UE4 facing function. Only override, never use
		virtual void HitByFlashLight_Implementation(){}; // new
	
		/// An Event for the left face button (keyboard: left shift button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void ScareButton();

		/// UE4 facing function. Only override, never use
		virtual void ScareButton_Implementation(){};

		/// An Event for the release of the left face button (keyboard: left shift button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void ScareButtonRelease();

		/// UE4 facing function. Only override, never use
		virtual void ScareButtonRelease_Implementation(){};

		/// An Event for the bottom face button (keyboard: space-bar button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void MoveButton();

		/// UE4 facing function. Only override, never use
		virtual void MoveButton_Implementation(){};

		/// An Event for the release of the left face button (keyboard: space-bar button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void MoveButtonRelease();

		/// UE4 facing function. Only override, never use
		virtual void MoveButtonRelease_Implementation(){};

		/// An Event for the top face button (keyboard: 'Q' button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void ButtonTop();

		/// UE4 facing function. Only override, never use
		virtual void ButtonTop_Implementation(){};

		/// An Event for the release of the top face button (keyboard: 'Q' button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void ButtonTopRelease();

		/// UE4 facing function. Only override, never use
		virtual void ButtonTopRelease_Implementation(){};

		/// An Event for the right trigger button (Right Mouse button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void RightTrigger();

		/// UE4 facing function. Only override, never use
		virtual void RightTrigger_Implementation(){};

		/// An Event for the release of the Right Trigger button (Right Mouse button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void RightTriggerRelease();

		/// UE4 facing function. Only override, never use
		virtual void RightTriggerRelease_Implementation(){};

		/// An Event for the right bumper button 
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void RightBumper();

		/// UE4 facing function. Only override, never use
		virtual void RightBumper_Implementation(){};

		/// An Event for the release of the Right Bumper button
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void RightBumperRelease();

		/// UE4 facing function. Only override, never use
		virtual void RightBumperRelease_Implementation(){};
	
		/// An Event for the Left Trigger button (Left Mouse button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void LeftTrigger();

		/// UE4 facing function. Only override, never use
		virtual void LeftTrigger_Implementation(){};

		/// An Event for the release of the Left Trigger button (Left Mouse button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void LeftTriggerRelease();

		/// UE4 facing function. Only override, never use
		virtual void LeftTriggerRelease_Implementation(){};

		/// An Event for the left bumper button 
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void LeftBumper();

		/// UE4 facing function. Only override, never use
		virtual void LeftBumper_Implementation(){};

		/// An Event for the release of the Left Bumper button
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void LeftBumperRelease();

		/// UE4 facing function. Only override, never use
		virtual void LeftBumperRelease_Implementation(){};
	
		/// An axis event for the left thumbstick x-axis (keyboard: A, D buttons)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveRightAxis(float Axis);

		/// UE4 facing function. Only override, never use
		virtual void MoveRightAxis_Implementation(float Axis){}

		/// An axis event for the left thumbstick y-axis (keyboard: W, S buttons)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
		void MoveForwardAxis(float Axis);

		/// UE4 facing function. Only override, never use
		virtual void MoveForwardAxis_Implementation(float Axis){}

		/// An axis event for the right thumbstick y-axis (mouse y-axis)
		UFUNCTION(BlueprintNativeEvent, Category = "InputEvent")
	    void LookUpAxis(float Axis);

		/// UE4 facing function. Only override, never use
		virtual void LookUpAxis_Implementation(float Axis){};

		/// An axis event for the right thumbstick x-axis (mouse x-axis)
		UFUNCTION(BlueprintNativeEvent, Category = "InputEvent")
	    void LookRightAxis(float Axis);

		/// UE4 facing function. Only override, never use
		virtual void LookRightAxis_Implementation(float Axis){};
		
		/// An Event for the right special button (keyboard: escape button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void StartButton();

		/// UE4 facing function. Only override, never use
		virtual void StartButton_Implementation(){};

		/// An Event for the release of the right special button (keyboard: escape button)
		UFUNCTION(BlueprintNativeEvent, Category="InputEvent")
	    void StartButtonRelease();

		/// UE4 facing function. Only override, never use
		virtual void StartButtonRelease_Implementation(){};
	
		// TODO: Add new virtual inputs as necessary
	// === End >> Input Event Functions ===
	

};
