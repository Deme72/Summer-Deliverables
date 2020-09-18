// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemyCharacter.generated.h"


UCLASS()
class SUMMERDELIVERABLES_API ABaseEnemyCharacter : public ACharacter
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
	/// The enum class for Enemy States
	enum class EState { Searching, Cautious, Running, Stealing, Dying };
	
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
	/// The Enemy's current Bravery (i.e. health)
	float Bravery;

	/// The Enemy's current Paranoia
	float Paranoia;

	/// How many consecutive scares this enemy has experienced (resets after ScareComboInterval)
	int ComboCounter;

	/// The time remaining until the ComboCounter resets
	float ComboTimer;

	/// The current ESate of this Enemy
	EState CurrentEState;

	/// How much time the enemy has spent in this state
	float CurrentEStateTime;

	/// Time until Paranoia will begin to decay
	float ParanoiaDecayTime;

	/// Time until the enemy extis the running state
	/// ; random > RunningDurationMin < RunningDurationMax
	float CurrentRunningDuration;
	
	public:
	/// The Enemy's max Bravery (i.e. health)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Bravery")
	int BraveryMax;

	/// The Enemy's max Paranoia
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Paranoia")
	int ParanoiaMax;

	/// The rate at which paranoia decays (paranoia per second)(*not per tick)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Paranoia")
	float ParanoiaDecay;

	/// The time after an enemy takes paranoia damage (in seconds) before it will decay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Paranoia")
	float ParanoiaDecayDelay;

	/// Additive bravery damage based on how far above ParanoiaMax the current Paranoia is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Paranoia|Bravery")
	float ParanoiaOverflowDamage;
	
	/// The rate at which paranoia decays (paranoia per second)(*not per tick) while the enemy
	/// is in the running state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Paranoia")
	float ParanoiaRunningDecay;

	/// The percent of max paranoia in which the enemy will change to the Cautious State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Paranoia")
	float ParanoiaCautiousThreshold;

	/// The percent of max paranoia in which the enemy will change to the Running State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Paranoia")
	float ParanoiaRunningThreshold;

	/// A percent multiplying her Paranoia that creates an additive bonus to scare damage
	/// (ScareBonus * (Paranoia / ParanoiaMax) + 1) * ScareDamage = TotalDamage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Bravery")
	float ScareBonus;

	/// The time in which it takes for the scare combo counter to reset once it's > 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Bravery")
	float ScareComboInteraval;

	/// The percent of max Bravery (i.e. health) in which the enemy will change to the Running State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Bravery|States")
	float ScareRunningThreshold;

	/// The speed at which an enemy will move at while in the searching state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float SearchingMovementSpeed;

	/// The speed at which an enemy will move at while in the cautious state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float CautiousMovementSpeed;

	/// The speed at which an enemy will move at while in the Running state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float RunningMovementSpeed;

	/// The speed at which an enemy will move at while in the Escaping state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float EscapingMovementSpeed;

	/// The speed at which an enemy will move at while in the Stealing state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float StealingMovementSpeed;

	/// The minimum amount of time an enemy will spend in the Running state before changing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float RunningDurationMin;

	/// The maximum amount of time an enemy will spend in the Running state before changing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement")
	float RunningDurationMax;

	/// The AI Controller for this Enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="States|Components")
	AAIController* AIController;

	/// The Skeletalmesh for this enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMesh;

	/// The socket in BaseEnemyCharacter::Mesh that holds the Flashlight object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Sockets|States")
	USkeletalMeshComponent* FlashLightSocket;

	/// The socket in BaseEnemyCharacter::Mesh that holds the Treasure object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Sockets|States")
	USkeletalMeshComponent* TreasureSocket;

	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	/// Handles changing the enemy state to the new state
	void SetEState(EState new_State);
	
	public:
	/// Returns the current Bravery (i.e health) of the Enemy
	UFUNCTION(Category="Getters|Bravery")
	float GetBravery() const { return Bravery; }

	/// Returns the current Paranoia of the Enemy
	UFUNCTION(Category="Getters|Paranoia")
    float GetParanoia() const { return Paranoia; }

	/// Takes a base bravery damage and applies the damage formula
	/// (ScareBonus * (Paranoia / ParanoiaMax) + 1) * ScareDamage = TotalDamage
	/// before modifying enemy bravery (i.e health)
	UFUNCTION(Category="Setters|Bravery")
    void TakeBraveryDamage(float BraveryBaseDamage);

	/// Applies Paranoia Damage to the enemy's Paranoia
	UFUNCTION(Category="Setters|Paranoia")
    void TakeParanoiaDamage(float ParanoiaDamage);

	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	public:
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ====== enemy class specific methods =====
	/// Applies the treasure to the enemy skeletalmesh and modifies behavior
	UFUNCTION(Category="States")
    void PickUpTreasure(AActor* treasure);

	/// Removes a treasure from the enemy skeletalmesh and modifies behavior
	/// ; else throws a debug message
	UFUNCTION(Category="States")
    void DropTreasure();
	
};
