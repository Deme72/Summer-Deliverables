<<<<<<< Updated upstream
// Fill out your copyright notice in the Description page of Project Settings.


#include "PossesableComponent.h"

void UPossesableComponent::OnInteract_Implementation()
{
   bInUse=true;
    
    
}

void UPossesableComponent::EndInteract_Implementation()
{
    bInUse=false;
}
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "PossesableComponent.h"

void UPossesableComponent::OnInteract_Implementation()
{
   APawn* Owner=  Cast<APawn>(GetOwner());
    if (Owner==nullptr)
    {
        //send error message here
        return;
    }
   bInUse=true;
    CurrentPlayer=Owner->GetController();
}

void UPossesableComponent::EndInteract_Implementation()
{
    bInUse=false;
    CurrentPlayer=nullptr;
}

void UPossesableComponent::TickComponent(float deltaTime)
{
    if(CurrentCooldown>0)
    {
        CurrentCooldown-=deltaTime;
    }  
}


>>>>>>> Stashed changes
