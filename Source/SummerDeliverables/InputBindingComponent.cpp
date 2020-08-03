// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBindingComponent.h"

void UInputBindingComponent::BindInput(UPossesableComponent* comp)
{
    ClearActionBindings();
    AxisBindings.Empty();
    BindAxis("RightAxis", comp, &UPossesableComponent::MoveRightAxis);
    BindAxis("ForwardAxis", comp, &UPossesableComponent::MoveForwardAxis);
    BindAction("Action", IE_Pressed, comp, &UPossesableComponent::TakeAction);
    //BindAction("Interact", IE_Pressed, comp, &UPossesableComponent::EndInteract);
}

void UInputBindingComponent::UnBindInput(UPossesableComponent* comp)
{
    ClearActionBindings();
    AxisBindings.Empty();
    //Default Bindings
}
