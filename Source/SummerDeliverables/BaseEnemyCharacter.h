// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

//#include "EdGraphSchema_K2.h"
#include "GameFramework/Character.h"
#include "BaseEnemyCharacter.generated.h"

/***************************************************************************//**
/// The enum class for Enemy States
/// @param BlueprintType
******************************************************************************/
UENUM(BlueprintType)
enum EState
{
    Searching UMETA(DisplayName = "Searching"),
    Cautious UMETA(DisplayName = "Cautious"),
    Running UMETA(DisplayName = "Running"),
    Stealing UMETA(DisplayName = "Stealing"),
    Dying UMETA(DisplayName = "Dying")
};

/// @param BlueprintType
/// The enum class for Enemy Targets
UENUM(BlueprintType)
enum TType 
{
	Treasure UMETA(DisplayName = "Treasure"),
    Prop UMETA(DisplayName = "Prop"),
    Ghost UMETA(DisplayName = "Ghost")
/// @param BlueprintType
};
/// The enum class for Enemy animations
UENUM(BlueprintType)
enum AnimType
{
    None UMETA(DisplayName = "None"),
	PickupTreasure UMETA(DisplayName = "Pick Up Treasure"),
	DropTreasure UMETA(DisplayName = "Drop Treasure"),
	InvestigatePOI UMETA(DisplayName = "Investigate POI"),
	Scare UMETA(DisplayName = "Scare"),
	InstantEscape UMETA(DisplayName = "InstantEscape")
};

/// enum class for the direction of the last scare
UENUM(BlueprintType)
enum ScareDirection
{
	Nondirectional UMETA(DisplayName = "Nondirectional"),
	Front UMETA(DisplayName = "Front"),
	Back UMETA(DisplayName = "Back"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

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
	/// Defines whether the current state of the enemy is newly changed
	bool bStateDirtyFlag;
	
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

	/// The current target type of this enemy
	TType CurrentTType;
	
	///The current animation
	AnimType CurrentAnim = AnimType::None;
	
	/// How much time the enemy has spent in this state
	float CurrentEStateTime;

	/// Time until Paranoia will begin to decay
	float ParanoiaDecayTime;

	/// Time until the enemy extis the running state
	/// ; random > RunningDurationMin < RunningDurationMax
	float CurrentRunningDuration;

	/// The location of the last prop that scared this enemy (when it was scared)
	FVector LastScareLocation;
	
	public:
	/// The Enemy's max Bravery (i.e. health) | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design", meta=(ClampMin=0.0))
	int BraveryMax;

	/// The Enemy's max Paranoia | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design", meta=(ClampMin=0.0))
	int ParanoiaMax;

	/// The rate at which paranoia decays (paranoia per second)(*not per tick) | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Damage|Paranoia", meta=(ClampMin=0.0))
	float ParanoiaDecay;

	/// The time after an enemy takes paranoia damage (in seconds) before it will decay | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Damage|Paranoia", meta=(ClampMin=0.0))
	float ParanoiaDecayDelay;

	/// Additive bravery damage based on how far above ParanoiaMax the current Paranoia is | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Damage|Bravery", meta=(ClampMin=0.0))
	float ParanoiaOverflowDamage;
	
	/// The rate at which paranoia decays (paranoia per second)(*not per tick) while the enemy
	/// is in the running state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Paranoia", meta=(ClampMin=0.0))
	float ParanoiaRunningDecay;

	/// The percent of max paranoia in which the enemy will change to the Cautious State | min=0.0, max=1.1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Paranoia", meta=(ClampMin=0.0, ClampMax=1.1))
	float ParanoiaCautiousThreshold;

	/// The percent of max paranoia in which the enemy will change to the Running State | min=0.0, max=1.1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Paranoia",meta=(ClampMin=0.0, ClampMax=1.1))
	float ParanoiaRunningThreshold;

	/// A percent multiplying her Paranoia that creates an additive bonus to scare damage | min=0.0
	/// (ScareBonus * (Paranoia / ParanoiaMax) + 1) * ScareDamage = TotalDamage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Damage|Bravery", meta=(ClampMin=0.0))
	float ScareBonus;

	/// The time in which it takes for the scare combo counter to reset once the combo counter > 1 | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Damage|Bravery", meta=(ClampMin=0.0))
	float ScareComboInteraval;

	/// The percent of max Bravery (i.e. health) in which the enemy will change to the Running State | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Bravery", meta=(ClampMin=0.0, ClampMax=1.0))
	float ScareRunningThreshold;

	/// The speed at which an enemy will move at while in the searching state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float SearchingMovementSpeed;

	/// The speed at which an enemy will move at while in the cautious state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float CautiousMovementSpeed;

	/// The speed at which an enemy will move at while in the Running state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float RunningMovementSpeed;

	/// The speed at which an enemy will move at while in the Escaping state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float EscapingMovementSpeed;

	/// The speed at which an enemy will move at while in the Stealing state | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float StealingMovementSpeed;

	/// The minimum amount of time an enemy will spend in the Running state before changing | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float RunningDurationMin;

	/// The maximum amount of time an enemy will spend in the Running state before changing | min=0.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|States|Movement", meta=(ClampMin=0.0))
	float RunningDurationMax;

	/// The AI Controller for this Enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AAIController* AIController;

	/// The Skeletalmesh for this enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMesh;

	// The socket in BaseEnemyCharacter::Mesh that holds the Flashlight object
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components|Sockets")
	//USkeletalMeshComponent* FlashLightSocket;

	// The socket in BaseEnemyCharacter::Mesh that holds the Treasure object
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components|Sockets")
	//USkeletalMeshComponent* TreasureSocket;

	/// Name of the treasure socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components|Sockets")
	FName TreasureSocketName;

	/// Treasure that the enemy is holding
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AActor* TreasureActor;

	/// Stolen props
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TArray<AActor*> OtherStolenObjects;
	
	/// Time remaining in the current animation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Animation")
	float CurrentAnimTime;

	/// A reference to the stamina pickup actor that only effects the player picking it up
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerStamina;

	///A reference to the stamina pickup actor that effects all players
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TeamStamina;

	/// The last scare direction
	ScareDirection LastScareDirection;

	///Whether the enemy's movement is halted for an animation
	bool bIsMovementHalted = false;
	
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
	/// Sets default values for this character's properties
	ABaseEnemyCharacter();
	
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	
	private:
	/// Internal function for setting the last scare direction after paranoia/bravery damage
	void SetLastScareDirection();
	protected:
	public:
	/// Handles changing the enemy state to the new state
	/// Currently a protected function
	/// Let Jefferey know if you can't work with this 
	UFUNCTION(BlueprintCallable, Category="Setters", meta=(BlueprintProtected))
	void SetEState(EState NewEState);

	/// Helper for the speed change functionality
	float GetStateSpeed(EState State);

	/// Handles changing the enemy state to the new state
	UFUNCTION(BlueprintCallable, Category="Getters")
	EState GetEState() const { return CurrentEState; }

	/// Handles changing the enemy state to the new state
	UFUNCTION(BlueprintCallable, Category="Getters")
    TType GetTType() const { return CurrentTType; }

	/// Handles getting the animation state
	UFUNCTION(BlueprintCallable, Category="Getters")
	AnimType GetAnimation() const { return CurrentAnim; }

	/// Sets the animation state
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetAnimation(AnimType newType, float animTime); //{CurrentAnim = newType; CurrentAnimTime = animTime; return CurrentAnim;}
	
	/// Returns the current Bravery (i.e health) of the Enemy
	UFUNCTION(BlueprintCallable, Category="Getters|Bravery")
	float GetBravery() const { return Bravery; }

	/// Returns the current Paranoia of the Enemy
	UFUNCTION(BlueprintCallable, Category="Getters|Paranoia")
    float GetParanoia() const { return Paranoia; }

	/// Set the enemy's AIController
	UFUNCTION(BlueprintCallable, Category="Setters|Components")
	void SetAIController( AAIController* new_aicontroller ) { AIController = new_aicontroller; };

	/// Get the enemy's AIController
	UFUNCTION(BlueprintCallable, Category="Setters|Components")
	AAIController* GetAIController() const { return AIController; };
	
	/// Set the enemy's SkeletalMesh
	UFUNCTION(BlueprintCallable, Category="Setters|Components")
	void SetSkeletalMesh( USkeletalMeshComponent* new_skeletalmesh ) { SkeletalMesh = new_skeletalmesh; };

	/// Get the enemy's SkeletalMesh
	UFUNCTION(BlueprintCallable, Category="Setters|Components")
	USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; };

	// Set the enemy's Flashlight Socket
	//UFUNCTION(BlueprintCallable, Category="Setters|Components")
	//void SetFlashlightSocket( USkeletalMeshComponent* new_flashlightsocket ) { FlashLightSocket = new_flashlightsocket; };

	// Get the enemy's FlashlightSocket
	//UFUNCTION(BlueprintCallable, Category="Setters|Components")
	//USkeletalMeshComponent* GetFlashlightSocket() const { return FlashLightSocket; };

	// Set the enemy's TreasureSocket
	//UFUNCTION(BlueprintCallable, Category="Setters|Components")
	//void SetTreasureSocket( USkeletalMeshComponent* new_treasuresocket ) { TreasureSocket = new_treasuresocket; };

	// Get the enemy's TreasureSocket
	//UFUNCTION(BlueprintCallable, Category="Setters|Components")
	//USkeletalMeshComponent* GetTreasureSocket() const { return TreasureSocket; };
	
	/// Takes a base bravery damage and applies the damage formula
	/// (ScareBonus * (Paranoia / ParanoiaMax) + 1) * ScareDamage = TotalDamage
	/// before modifying enemy bravery (i.e health)
	UFUNCTION(BlueprintCallable, Category="Setters|Bravery")
    float TakeBraveryDamage(float base_bravery_damage, FVector prop_position);

	/// Applies Paranoia Damage to the enemy's Paranoia
	UFUNCTION(BlueprintCallable, Category="Setters|Paranoia")
    float TakeParanoiaDamage(float paranoia_damage, FVector prop_position);

	/// Returns an FVector of the location of the last prop that successfully scared this enemy when it was scared
	UFUNCTION(BlueprintCallable, Category="Getters")
	FVector GetLastScareLocation() const { return LastScareLocation; }

	/// Returns bStateDirtyFlag (i.e whether the current state of the enemy is new)
	UFUNCTION(BlueprintCallable, Category="Getters")
	bool GetStateIsDirty() const { return bStateDirtyFlag; }

	/// Sets the bStateDirtyFlag to false
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetStateFlagClean() { bStateDirtyFlag = false; }

	UFUNCTION(BlueprintCallable, Category="Getters")
	ScareDirection GetLastScareDirection() const { return LastScareDirection; }
	
	// ===================
	// ===== METHODS =====
	// ===================
	
	protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Called when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	public:
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Handles the OnTick functionality related to Paranoia (A tidying function)
	void ParanoiaTick(float DeltaTime);

	/// Handles the OnTick functionality related to Animation
	void AnimationTick(float DeltaTime);
	
	// ====== enemy class specific methods =====
	/// Applies the treasure to the enemy skeletalmesh and modifies behavior
	UFUNCTION(BlueprintCallable, Category="Function")
    virtual void PickUpTreasure(AActor* treasure);

	/// Removes a treasure from the enemy skeletalmesh and modifies behavior
	/// ; else throws a debug message
	UFUNCTION(BlueprintCallable, Category="Functions")
    virtual void DropTreasure();

	/// Adds a prop (or ghost?) to the enemy's bag
	UFUNCTION(BlueprintCallable, Category="Functions")
	virtual void PickUpOther(AActor* target);
	
	/// Removes all props from the enemy's bag
	UFUNCTION(BlueprintCallable, Category="Functions")
	virtual void DropObjects();
};
