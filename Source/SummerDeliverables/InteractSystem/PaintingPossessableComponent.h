// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityPossessableComponent.h"
#include <string>
#include <map>
#include <list>
#include "PaintingPossessableComponent.generated.h"

/**
 * TODO: As is the same as all the utilities, this is incomplete
 */
USTRUCT()
struct FPaintingTransitionPackage
{
	GENERATED_BODY()
	
	/// The Painting this player entered the network through
	class UPaintingPossessableComponent* RootPainting;
	
	/// The current path through the networks the player instantiated
	std::list<UPaintingPossessableComponent*> Iterator;
	
	/// The remaining time this painting needs to be blinded (follows Player through network
	float TimeBeingBlind;	
};



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
	USceneComponent* currentExitPoint;

	class APaintingManager* Manager;
	
	/// Whether this isn't a painting that a player currently in the network entered through
	bool bIsNonRoot;

	bool bIsRoot;

	FPaintingTransitionPackage PaintingDataPackage;

	std::list<FString> ConnectedNetworks;
	
	protected:
	public:
	/// For Debugging purposes
	FString PaintingName;
	
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
	UFUNCTION(BlueprintCallable)
    void SetCurrentExitPoint(USceneComponent* NewExitPoint);

	void NewPaintingDataPackage();
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
	/// When the Player is caught in the flashlight of an enemy inside a painting that isn't root
	void ApplyBlindEffect();

	/// When the Player is caught in the flashlight of an enemy inside a painting that is root
	void ApplyStaminaDrain(float delta_time);

	/// Initializes the Painting and PaintingManager systems
	UFUNCTION(BlueprintCallable)
	void BlueprintConstructorInit(TArray<FString> connected_networks, FString painting_name);

	void InternalPaintingPossession(UPaintingPossessableComponent* target_painting);

	void EndInternalPaintingPossession();
	
	virtual void RightTriggerRelease_Implementation() override final;

	virtual void LeftTriggerRelease_Implementation() override final;

	virtual void EndInteract_Implementation() override final;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};






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
	/// *** Public for debugging purposes ***
	/// The "Manager" i.e. the network names (fstring)
	/// and all paintings connected to that network
    //TMultiMap<FString, UPaintingPossessableComponent*> Networks;
	std::multimap<FString, UPaintingPossessableComponent*> Network;
	
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
	//TArray<UPaintingPossessableComponent*> GetPaintingIterator(TArray<FString> connected_networks, UPaintingPossessableComponent* painting);
	std::list<UPaintingPossessableComponent*> GetPaintingPath(std::list<FString> connected_networks, UPaintingPossessableComponent* painting);
	// ===================
	// ===== METHODS =====
	// ===================
private:
protected:
public:
	/// To be called by the PaintingPossessiblePawn Editor Constructor
    void AppendToConnectedNetworks(std::list<FString> connected_networks, UPaintingPossessableComponent* painting);
	

	/// FOR DEBUGGING NETWORKS DATA
	UFUNCTION(BlueprintCallable)
    void PrintNetworks() const;
	
	virtual void BeginPlay() override;

};