// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticPossessableComponent.h"

UStaticPossessableComponent::UStaticPossessableComponent():UPossesableComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UStaticPossessableComponent::ScareButton_Implementation()
{
    
}
