// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"

#include <string>

#include "PlayerGhostController.h"
#include "AIController.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "SummerDeliverables/PlayerPawn.h"
#include "PossessablePawn.h"
#include "SummerDeliverables/BaseEnemyCharacter.h"

UPossesableComponent::UPossesableComponent():UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    CurrentCooldown = 0.0f;
    bIsDrainingStamina = true;
}

float UPossesableComponent::GetStamina()
{
	TArray<AActor *> EnemyCollisions;
    APossessablePawn* owner = Cast<APossessablePawn>(GetOwner());
    if (owner->IsPossessing())
    {
        AController* controller = owner->GetController();
        if (controller != nullptr)
        {
            APlayerGhostController* ghost_controller = Cast<APlayerGhostController>(controller);
            if (ghost_controller)
                return ghost_controller->GetStamina();
            else 
                SCREENMSG("Controller isn't of APlayerGhostController");
        }
    }
    return 0.0f;
}

bool UPossesableComponent::SetStamina(float delta_stamina, bool b_is_relative)
{
    if (Cast<APossessablePawn>(GetOwner())->IsPossessing())
    {
        auto controller = Cast<APlayerGhostController>(Cast<APawn>(GetOwner())->GetController());
        if (controller)
            return controller->SetStamina(delta_stamina, b_is_relative);
    }
    return false;
}

bool UPossesableComponent::CanAffordStaminaCost(const float stamina_cost) const
{
    if (Cast<APossessablePawn>(GetOwner())->IsPossessing())
        return Cast<APlayerGhostController>(Cast<APawn>(GetOwner())->GetController())->CanAffordStaminaCost(stamina_cost);
    return false;
}


void UPossesableComponent::Eject() const
{
    APossessablePawn * tmp = Cast<APossessablePawn>(GetOwner());
    tmp->EndPossession();
}


void UPossesableComponent::SpawnSubPawn(TSubclassOf<APossessablePawn> pawn, FTransform pos) const
{
    APossessablePawn * tmp = Cast<APossessablePawn>(GetOwner());
    tmp->SpawnSubPawn(pawn, pos);
}

void UPossesableComponent::SetNextExit(APossessablePawn * pawn) const
{
    APossessablePawn * tmp = Cast<APossessablePawn>(GetOwner());
    tmp->SetNextExit(pawn);
}
 
void UPossesableComponent::Scare(float baseMultiplier) const
{
    TArray<AActor *> EnemyCollisions;
    DamageBounds->GetOverlappingActors(EnemyCollisions);
    
    FVector location = GetOwner()->GetTransform().GetLocation();
    
    //float tot_bravery_damage = 0.0f, tot_paranioa_damage = 0.0f; // For tracking player stats
    for(auto collision:EnemyCollisions)
    {
        ABaseEnemyCharacter * enemy = Cast<ABaseEnemyCharacter>(collision);
        if(enemy)
        {
            //tot_bravery_damage += enemy->TakeBraveryDamage(DamageAmount*baseMultiplier, location);
            enemy->TakeBraveryDamage(DamageAmount*baseMultiplier, location);
        }
    }
    ParanoiaBounds->GetOverlappingActors(EnemyCollisions);
    for(auto collision:EnemyCollisions)
    {
        ABaseEnemyCharacter * enemy = Cast<ABaseEnemyCharacter>(collision);
        if(enemy)
        {
            //tot_paranioa_damage += enemy->TakeParanoiaDamage(ParanoiaAmount*baseMultiplier, location);
            enemy->TakeParanoiaDamage(ParanoiaAmount*baseMultiplier, location);
        }
    }

    /*// Update Player Stats
    FPlayerGhostStatistics &stats = GetCurrentPlayer()->GetStatisticsRef();
    stats.BraveryDamageDealt += tot_bravery_damage;
    stats.ParanoiaDamageDealt += tot_paranioa_damage;

    // Debuging player stats
    //std::string msg = "";
    //msg += "Player braveryDmg = " + std::to_string(stats.BraveryDamageDealt) + " | paranoiaDmg = " + std::to_string(stats.ParanoiaDamageDealt);
    //SCREENMSGT(msg.c_str(), 10.0f);
    */
}

void UPossesableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(CurrentCooldown>0)
        CurrentCooldown-=DeltaTime;
    else
        CurrentCooldown = 0;
}

APlayerGhostController* UPossesableComponent::GetCurrentPlayer() const
{
    const auto owner = Cast<APossessablePawn>(GetOwner());
    if (owner)
    {
        return Cast<APlayerGhostController>(owner->GetController());
    }
    return nullptr;
}
