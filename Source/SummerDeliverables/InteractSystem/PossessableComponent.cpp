// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableComponent.h"
#include "PlayerGhostController.h"

#include "AIController.h"
#include "PlayerGhostController.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "SummerDeliverables/PlayerPawn.h"
#include "SummerDeliverables/BaseEnemyCharacter.h"

UPossesableComponent::UPossesableComponent():UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    CurrentCooldown = 0.0f;
    bIsDrainingStamina = true;
}

float UPossesableComponent::GetStamina() const
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

bool UPossesableComponent::SetStamina(float delta_stamina, bool b_is_relative) const
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

void UPossesableComponent::Scare(float baseMultiplier) const
{
    TArray<AActor *> EnemyCollisions;
    DamageBounds->GetOverlappingActors(EnemyCollisions);
    for(auto collision:EnemyCollisions)
    {
        ABaseEnemyCharacter * enemy = Cast<ABaseEnemyCharacter>(collision);
        if(enemy)
            enemy->TakeBraveryDamage(DamageAmount*baseMultiplier);
    }
    ParanoiaBounds->GetOverlappingActors(EnemyCollisions);
    for(auto collision:EnemyCollisions)
    {
        ABaseEnemyCharacter * enemy = Cast<ABaseEnemyCharacter>(collision);
        if(enemy)
            enemy->TakeParanoiaDamage(ParanoiaAmount*baseMultiplier);
        
    }
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
