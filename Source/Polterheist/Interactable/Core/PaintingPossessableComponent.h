// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityPossessableComponent.h"
#include <set>
#include <map>
#include <list>
#include "PaintingPossessableComponent.generated.h"

#define INTERNAL_UPDATE 0.5f

/// A package of data that passes from one UPaintingPossessableComponent to another while a player
/// traverses the network
struct FPaintingTransitionPackage
{
	/// Is this Struct Initialized
	bool bInit;
	
	/// The Painting this player entered the network through
	class UPaintingPossessableComponent* RootPainting;
	
	/// The current path through the networks the player instantiated
	std::set<UPaintingPossessableComponent*> Path;
	
	/// The remaining time this painting needs to be blinded
	/// Acts as bIsBlinded if > 0.0f
	float TimeBeingBlind;

	/// The transform of the RootPainting's Pawn's ExitPoint
	FTransform RootExitPoint;

	/// Am I leaving the Network
	bool bIsExitingNetwork;
};

/// A possessable that is used for reconnaissance rather than to scare enemies
UCLASS()
class SUMMERDELIVERABLES_API UPaintingPossessableComponent : public UUtilityPossessableComponent
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
	/// The current state of a given painting
	/// (state is relative to IsInUse() and whether or not it's a painting used to enter the network)
	enum state
	{
		Inactive, 	// Non-Root and unpossessed
        Root,		// Is the painting that a player entered the network through
        NonRoot 	// Non-Root but is possessed
    };
	
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
	/// The manager for all Paintings in the Scene
	class APaintingManager* Manager;

	/// The transform of the owning APossessablePawn's Exitpoint at this time
	FTransform TempExitPoint;

	/// The transform of the owning APossessablePawn's Exitpoint at BeginPlay()
	FTransform DefaultExitPoint;
	
	/// Identifies whether this painting is: 
	/// | Inactive 
	/// | NonRoot (Possessed but not Root) 
	/// | Root (A Root Painting, whether or not it's possessed)
	state State;

	/// A collection of data that is passed from painting to painting as a player travels the network
	FPaintingTransitionPackage PaintingDataPackage;

	/// An internal (C++ friendly) copy of the editor APaintingPossessablePawn equivalent 
	std::list<FString> ConnectedNetworks;

	/// For Debugging purposes
	FString PaintingName;

	/// Debug timer for internal print-to-screens
	float TimeTillUpdate;

	/// Whether the painting should ejected the player during tick
	bool bLeaving;
	
	protected:
	public:

	
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	/// The default constructor
	UPaintingPossessableComponent();
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
	// UFUNCTION(BlueprintCallable)
    // void SetCurrentExitPoint(USceneComponent* NewExitPoint);

	/// Generates a Painting Data Package
	void NewPaintingDataPackage();

	/// Purely for debugging Purposes; gives a user-created, not-necessarily-unique Painting Name 
	UFUNCTION(BlueprintCallable, Category="Getters")
	FString GetPaintName() const { return PaintingName; }
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	/// Finds a valid PaintingPossessableComponent in the network and stores it in the passed reference
	/// return true if there is a valid Painting accessable
	UPaintingPossessableComponent* FindValidPainting(bool& b_valid, bool b_forward);
	
	protected:
	public:
	/// When the Player is caught in the flashlight of an enemy inside a painting that isn't root
	void ApplyBlindEffect(float time_remaining);

	/// Initializes the Painting and PaintingManager systems
	UFUNCTION(BlueprintCallable)
	void BlueprintConstructorInit(TArray<FString> connected_networks, FString painting_name);

	/// Handles Possessions between Paintings
	void InternalPaintingPossession(UPaintingPossessableComponent* target_painting);

	/// Handles being hit by flashlight
	virtual void HitByFlashLight_Implementation() override final;
	
	/// Forward iteration through the Painting's path
	virtual void RightTriggerRelease_Implementation() override final;

	/// Backwards iteration through the Painting's path
	virtual void LeftTriggerRelease_Implementation() override final;

	/// On Interact Event
	virtual void OnInteract_Implementation() override final;

	/// On End Interact Event
	virtual void EndInteract_Implementation() override final;

	/// Update
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// FOR DEBUGGING
	virtual void ButtonTopRelease_Implementation() override final;

	/// FOR DEBUGGING
	virtual void ScareButton_Implementation() override;

	virtual void BeginningEndInteract() override final;

};

/// A user-hidden class that is handled by PaintingPossessableComponents existing in a Unreal Map
/// Holds the list of networks and how all paintings are connected
/// Generates Paths through the network whenever a painting calls for one
/// (allows for changes to painting networks mid-game; with a bit more editing)
UCLASS(NotBlueprintable)
class APaintingManager : public AActor
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
	/// The "Manager" i.e. the network names (fstring)
	/// and all paintings connected to that network
	std::multimap<FString, UPaintingPossessableComponent*> Networks;
	
protected:
public:
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
protected:
public:
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
private:
protected:
public:
	/// Returns a path relative to a list of connected networks and a root painting
	std::set<UPaintingPossessableComponent*> GetPaintingPath(std::list<FString> connected_networks, UPaintingPossessableComponent* painting);
	// ===================
	// ===== METHODS =====
	// ===================
private:
protected:
public:
	/// To be called by the PaintingPossessiblePawn BeginPlay()
	/// registers a painting with the painting manager
    void AppendToNetworks(std::list<FString> connected_networks, UPaintingPossessableComponent* painting);
	
	/// FOR DEBUGGING NETWORKS DATA
	UFUNCTION(BlueprintCallable)
    void PrintNetworks() const;
};