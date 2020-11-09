// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingPossessableComponent.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "PossessablePawn.h"

UPaintingPossessableComponent::UPaintingPossessableComponent()
{
    StamFrontCost=0.0f;
    StamDrainRate=0.0f;
}

void UPaintingPossessableComponent::SetCurrentExitPoint(USceneComponent* NewExitPoint)
{
    currentExitPoint=NewExitPoint;
}

void UPaintingPossessableComponent::NewPaintingDataPackage()
{
    bIsRoot = true;
    PaintingDataPackage = FPaintingTransitionPackage{};
    PaintingDataPackage.Iterator = Manager->GetPaintingPath(ConnectedNetworks, this);
    PaintingDataPackage.RootPainting = this;
    PaintingDataPackage.TimeBeingBlind = 0.0f;
}

void UPaintingPossessableComponent::ApplyBlindEffect()
{
}

void UPaintingPossessableComponent::ApplyStaminaDrain(float delta_time)
{
}

void UPaintingPossessableComponent::BlueprintConstructorInit(TArray<FString> connected_networks, FString painting_name)
{
    PaintingName = painting_name;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaintingManager::StaticClass(), FoundActors);
    if (FoundActors.Num() == 0)
    {
        Manager = Cast<APaintingManager>(GetWorld()->SpawnActor(APaintingManager::StaticClass()));
    }
    else
    {
        Manager = Cast<APaintingManager>(FoundActors[0]);
    }

    std::list<FString> connected_net = std::list<FString>{};
    for (auto string : connected_networks)
    {
        connected_net.push_back(string);
    }
    Manager->AppendToConnectedNetworks(connected_net, this);
    Manager->PrintNetworks();
}

void UPaintingPossessableComponent::InternalPaintingPossession(UPaintingPossessableComponent* target_painting)
{
    if (!target_painting->bIsRoot)
        target_painting->bIsNonRoot = true;
    bIsNonRoot = false;

    SetNextExit(Cast<APossessablePawn>(target_painting->GetOwner()));
    Eject();
}

void UPaintingPossessableComponent::EndInternalPaintingPossession()
{
}

void UPaintingPossessableComponent::RightTriggerRelease_Implementation()
{
    SCREENMSGT("Forward Iteration Through Network", 5.0f);
    auto& path = PaintingDataPackage.Iterator;
    std::string msg = "{ ";
    for (auto s : path)
    {
        msg += "\n" + std::string(TCHAR_TO_UTF8(*s->PaintingName)) + ", ";
    }
    msg += "\n}";
    SCREENMSGT(msg.c_str(), 10.0f);
}

void UPaintingPossessableComponent::LeftTriggerRelease_Implementation()
{
    SCREENMSGT("Reverse Iteration Through Network", 5.0f);
    /*auto iter = PaintingDataPackage.Iterator.CreateIterator();
    
    --iter;
    UPaintingPossessableComponent* p = iter.operator*();
    
    bool valid = true;
    while (true)
    {
        if (p != PaintingDataPackage.RootPainting && p->bIsRoot)
            valid = false;
        
        auto owner = Cast<APossessablePawn>(p->GetOwner());
        if (owner)
            if (owner->IsPossessing())
                valid = false;
        
        if (valid)
            break;
        
        --iter;
        p = iter.operator*();
    }
    InternalPaintingPossession(p);*/
}

void UPaintingPossessableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    auto owner = Cast<APossessablePawn>(GetOwner());
    if (owner)
        if (owner->IsPossessing())
        {
            if (!bIsNonRoot && !bIsRoot)
                NewPaintingDataPackage();
            //std::string msg = "";
            //msg += "CurrBIsRoot = " + std::to_string(bIsRoot);
            //msg += "\nCurrBIsNonRoot = " + std::to_string(bIsNonRoot);
            //SCREENMSGT(msg.c_str(), 1.0f);
        } 
}

void APaintingManager::AppendToConnectedNetworks(std::list<FString> connected_networks,
    UPaintingPossessableComponent* painting)
{
    for (FString network : connected_networks)
    {
        Network.emplace(network, painting);
    }
}

void UPaintingPossessableComponent::EndInteract_Implementation()
{
    SCREENMSGT("Leave Network", 5.0f);
    PaintingDataPackage.RootPainting->bIsRoot = false;
    bIsNonRoot = false;
}


std::list<UPaintingPossessableComponent*> APaintingManager::GetPaintingPath(std::list<FString> connected_networks,
                                                                             UPaintingPossessableComponent* painting)
{
    std::list<UPaintingPossessableComponent*> iter{};
   
    for (FString network : connected_networks)
    {
        for (std::multimap<FString, UPaintingPossessableComponent*>::iterator networks_iter = Network.begin(); networks_iter != Network.end(); ++networks_iter)
        {
            if(networks_iter->second != painting)
            {
                iter.push_back(networks_iter->second);
            }
        }
    }
    iter.push_back(painting);
    
    return iter;
}

void APaintingManager::PrintNetworks() const
{
    std::string msg = "{\n";
    for (auto pair : Network)
    {
        if (pair.first != "")
            msg += "{ " + std::string(TCHAR_TO_UTF8(*pair.first)) + ", " + std::string(TCHAR_TO_UTF8(*pair.second->PaintingName)) + " }\n";
    }
    msg += " }";
    SCREENMSGT(msg.c_str(), 10.0f);
}

void APaintingManager::BeginPlay()
{
    Super::BeginPlay();
    PrintNetworks();
}
