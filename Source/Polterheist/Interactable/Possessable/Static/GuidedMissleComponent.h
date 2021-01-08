// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Interactable/Core/StaticPossessableComponent.h"
#include "GuidedMissleComponent.generated.h"


/**
 * 
 */
UCLASS()
class POLTERHEIST_API UGuidedMissleComponent : public UStaticPossessableComponent
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
	// determine which part is being controlled when
	bool isControling = false;
	float power = 0.0;
	float accuracy =0.0;
	protected:
	public:
	//varriables to determine which mode the ball launcher is currently in powermode to gauge power same for accuracy
	bool Powermode =true;
	bool Accuracymode = false;
	//variables to determine where on the bar you are
	float bar = 0.0;
	float barDir=1.0;
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
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	public:
	///Doesn't change anything, only here as a template
	virtual void ScareButton_Implementation() override;
	virtual void OnInteract_Implementation() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	
};
