// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "InteractSystem/InteractableComponent.h"
#include "InteractSystem/ParanoiaComponent.h"
#include "GameFramework/Pawn.h"
#include "InteractSystem/PossessableComponent.h"

#include "PlayerPawn.generated.h"

/**
 * Ghostly player pawn.
**/
UCLASS()
class SUMMERDELIVERABLES_API APlayerPawn : public APawn
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
		/// The current possessing Player controller; used for possession/ unpossession
		class APlayerGhostController * CurrentPlayerController;
	
		/// The current bindings (the active input axis/actions and their code) of this player component
		/// bindings change from possession to possession
		class UPossesableComponent * CurrentBindings;

		/// An array of all interactable props within the bounding area of InteractBounds 
		TArray<UInteractableComponent *> OverlappingInteractables;

		/// An array of all props currently being selected from
		TArray<UParanoiaComponent *> SelectedProps;

		/// A boolean value; true if the player is currently checking for collisions with paranoia props
		bool bLookingForParaProps = false;

		/// A TArray for holding the other found players to give them team stamina
		TArray<AActor*> FoundActors;

		/// The possessable component to possess after entering
		class UPossesableComponent * AnimPossess;

		/// Current animation progress
		float AnimTimer;

		/// Start position of the animation
		FTransform AnimStartPos;

		/// End position of the animation
		FTransform AnimExitPos;

		/// Reference to the camera attached to the player
		/// Todo: after merging player controls with camera use whatever pointer it uses instead
		UCameraComponent * Cam;

		/// The skeletalmesh for the player
		/// Todo: clean up the unreal object structure for player
		USkeletalMeshComponent * PlayerMesh;

		/// The current player speed
		FVector CurrentSpeed;
		
		/// The locked movement direction for the right axis
		/// for when the camera is moving while input is happening
		FVector LockedCameraForward;

		/// The locked movement direction for the right axis
		/// for when the camera is moving while input is happening
		FVector LockedCameraRight;

		/// The normal of the last movement input
		FVector LastMovementNormal;
		
		
		/// Timer for switching cam control back to code from player input
		FTimerHandle LookTimer;
	
	protected:
	public:
		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate.*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float BaseTurnRate;

		/// The max speed, in Unreal units, that the player pawn moves right and forward at
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float MovementSpeed;

		/// The time it takes, in seconds, to get to max speed
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float MovementRamp;

		/// The lowest input from 0-1 that counts as an actual stick input
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float DeadZone;

		/// The angle in degrees at which an move input is considered different. Somewhere around 15 seems about right
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float MovementLockAngle;
		
		/// The height at which the character floats
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float FloatHeight;

			
		/// The speed at which the character lerps to the float height
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
		float FloatSpeed;
	
		/// The speed, in Unreal units, that the player pawn moves right and forward at
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stamina)
		float ParanoiaCost;

		/// The bounding shape in which the Player can interact with other possessables whose bounding shape collides with this
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Collision)
		UShapeComponent * InteractBounds;

		//For Team Stamina
		UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassToFind; // Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there

		/// Time to enter a prop
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
		float EnterTime;

		/// Time to exit a prop
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
		float ExitTime;
		
		/// If this is in the exiting animation
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Animation)
		bool bExiting;

		/// If this is in the entering animation
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Animation)
		bool bEntering;
		
	// ======================================
	// ===== CONSTRUCTORS_/_DESTRUCTORS =====
	// ======================================
	private:
	protected:
	public:
		/// Sets default values for this pawn's properties
		APlayerPawn();
	// =============================
	// ===== GETTERS_/_SETTERS =====
	// =============================
	private:
	protected:
	public:
		UFUNCTION(BlueprintCallable, Category="Getters")
	    /// Gets the player's current stamina
	    float GetStamina() const;

		UFUNCTION(BlueprintCallable, Category="Setters")
	    /// Adds the passed stamina value to the possessing player's current stamina |
	    /// b_is_relative dictates with the action is relative (i.e. CurrentStamina += stamina_drain) or is absolute (i.e. CurrentStamina = stamina_drain) |
	    /// returns true if the player has no stamina left
	    bool SetStamina(float stamina_drain, bool b_is_relative = true) const;

		/// Returns true if the current possessing player has enough stamina to afford the stamina_cost
		UFUNCTION(BlueprintCallable, Category="Possession")
	    bool CanAffordStaminaCost(const float stamina_cost) const;

		/// Setter for CurrentPlayer pointer
		void SetPlayer(APlayerGhostController * p){CurrentPlayerController = p;}
		
		/// Resets the movement lock called by camera
		void ResetMoveLock(){LastMovementNormal = {0,0,0};};

	
		UFUNCTION(BlueprintCallable, Category="Getters")
		///Returns the player speed
		float GetSpeed(){return CurrentSpeed.Size();};
	
	// ===================
	// ===== METHODS =====
	// ===================
	private:
	protected:
		/// Called when the game starts or when spawned
		void BeginPlay();

		/// Performs the "whisker" raycasts for the camera
		void WhiskersRayCast(float DeltaTime);
	public:
		/// Called every frame, put collision related code in here
		virtual void Tick(float DeltaTime) override;

		/// Called to bind functionality to the InputComponents
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		/// Handles interacting with interactable component derived objects
		UFUNCTION()
		void Interact();
		
		/// Possesses the component and its owner
		void Possess(class UPossesableComponent * comp);

		/// Sets up all the animation variables
		void PlayPossessAnimation(bool enter, FTransform lerpLoc, UPossesableComponent * comp = nullptr);
	
		/// Handles calling the action mapping for a currently possessed object
		UFUNCTION()
	    void ScareButtonStart();

		/// Handles cleaning up an interaction with a prop
		UFUNCTION()
	    void ScareButtonEnd();
		
		/// Returns a TArray of all interactable components currently within the player's InteractBounds
		UFUNCTION()
	    TArray<UInteractableComponent*> GetOverlappingInteractables() const
		{return OverlappingInteractables;}
		
		/// Returns a boolean value; whether or not the Player is currently possessing a prop
		UFUNCTION()
	    bool IsPossessing() const
		{return CurrentBindings != nullptr;}

		/// The final step for movement
		/// takes the movement vector and does some tweaks to make the controls tighter
		void AddMovementVector(FVector in, float DeltaTime);
	
		/// Handles moving the player along their Z axis
		/// positive #s move forward, negative backwards
		void MoveForward(float Value);

		/// Handles moving the player along their X axis
		/// positive #s move right, negative left
		void MoveRight(float Value);

		/// Handles turning the player's view around their Y axis
		/// positive #s turning right, negative left
		void LookRight(float Value);

		/// Handles turning the player's view around their X axis
		/// positive #s turning up, negative down
		void LookUp(float Value);

		/// An overlap function used to pickup stamina and highlight on
		UFUNCTION()
	    void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                       bool bFromSweep, const FHitResult & SweepResult);
		//An overlap function used to turn highlight off 
		UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
