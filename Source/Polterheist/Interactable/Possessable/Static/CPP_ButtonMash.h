// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Polterheist/Interactable/Interactable.h"
#include "Polterheist/Interactable/Core/StaticPossessableComponent.h"
#include "CPP_ButtonMash.generated.h"

/**
 * 
 */
UCLASS()
class POLTERHEIST_API UCPP_ButtonMash_Component : public UStaticPossessableComponent
{
	GENERATED_BODY()
	
	// ==============================
    // ===== DEFINES_/_TYPEDEFS =====
    // ==============================
    private:
    protected:
    public:
        TArray<FString> ButtonList;
        FString CurrentButton;
        
        float MashDrainRatePerSecond;
        float MashStrength;
        
        float CurrentLevel;
        float MaxLevel;
        
        TArray<float> StaminaPoints;
        int32 PointIndex;
        float StaminaChunk;
        int32 TempPoint;

		float AnimTime;
		float CurrentTime;
        
       
    
    // ======================================
    // ===== CONSTRUCTORS_/_DESTRUCTORS =====
    // ======================================
    private:
    protected:
        UCPP_ButtonMash_Component();
    public:
    
    // ===================
    // ===== METHODS =====
    // ===================
    private:
    protected:
    public:
        void PickButton();
        void DoMash(FString button);
        //void TickComponent(float dt);
        void StaminaPointLogic();
        //float GetRatiosForUIPerTick();
        //UFUNCTION( Category="Blah", BlueprintCallable )
        //void GetRatiosForUIOnPossess(float LevelRatios, TArray<bool> WhichButton);

		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
        virtual void ScareButton_Implementation() override;
        virtual void OnInteract_Implementation() override;
        virtual void ButtonTop_Implementation() override;
        virtual void MoveButton_Implementation() override;
        virtual void LeftTrigger_Implementation() override;
        virtual void RightTrigger_Implementation() override;
};
