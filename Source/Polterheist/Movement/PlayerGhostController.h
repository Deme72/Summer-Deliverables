// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PossessablePawn.h"
#include "GameFramework/PlayerController.h"
#include "PlayerGhostController.generated.h"


USTRUCT(BlueprintType)
struct FPlayerGhostStatistics
{
	GENERATED_BODY()
	
};


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

	/// For Science
	float LivingTime;

	/// The instance of the player HUD widget.
	/// The empty uproperty is there so we can keep the ptr around and to make sure it's garbage collected properly.
	UPROPERTY()
	UUserWidget* PlayerHUD;

	//struct FPlayerGhostStatistics PlayerStatistics;
	
	protected:
	public:
	UPROPERTY(EditAnywhere)
	class APossessablePawn* LastPossessedPawn;
	
	/// The class that will be used for the player's HUD
	UPROPERTY(EditAnywhere, Category="HUD and UI")
	TSubclassOf<class UUserWidget> HUDClass;
	
	/// The Player's max Stamina
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina", meta=(ClampMin=0.0))
	float MaxStamina;

	/// The player's stamina regen (in stamina/second)
	UPROPERTY(EditAnywhere, Category="Stamina", meta=(clampMin=0.0))
	float StaminaRegen;

	///Tells the controller that the player cant lose stamina
	UPROPERTY(EditAnywhere,Category="Stamina")
	bool Invisibility;
	///How long invisibility last for when active
	UPROPERTY(EditAnywhere,Category="Stamina")
	float InvisTimer;
	
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
	/// The BP pawn class to spawn
	UClass * PawnClass;
	public:
	/// Gets the player's current stamina
	UFUNCTION(BlueprintCallable, Category="Getters")
	float GetStamina() const { return CurrentStamina; }

	/// Adds the passed stamina value to the player's current stamina |
	/// b_is_relative dictates with the action is relative (i.e. CurrentStamina += delta_stamina)
	/// or is absolute (i.e. CurrentStamina = delta_stamina) |
	/// returns true if the player has no stamina left
	UFUNCTION(BlueprintCallable, Category="Setters")
    bool SetStamina(float delta_stamina, bool b_is_relative = true);

	/// Gets the percentage [0...1] of player stamina.
	UFUNCTION(BlueprintPure)
	float GetStaminaPercent() const;

	/*
	/// Returns a blueprint readable copy of the player statistics
	UFUNCTION(BlueprintCallable, Category="Getters")
	struct FPlayerGhostStatistics GetStatistics() const { return PlayerStatistics; }

	/// Returns a reference to the player's statistics. Should be only used in c++ (backend)
	struct FPlayerGhostStatistics& GetStatisticsRef() { return PlayerStatistics; }
	*/
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
	virtual void Tick(float DeltaSeconds) override;
	
	/// Creates an APlayerPawn at the given location and returns the pointer
	UFUNCTION(BlueprintCallable, Category="Possession")
	class APlayerPawn* CreatePlayerPawn(FVector spawn_location);

	/// Returns true if the player currently has enough stamina to afford the stamina cost
	UFUNCTION(BlueprintCallable, Category="Possession")
	bool CanAffordStaminaCost(const float stamina_cost) const {return CurrentStamina - stamina_cost > 0.0;}
	
	virtual void BeginPlay() override;

	virtual void OnUnPossess() override;
	///Changes Invisibility to true when player activates the Stamina room ability
	void SetInvisibility(bool value);
};

