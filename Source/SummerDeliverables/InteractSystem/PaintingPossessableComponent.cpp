// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintingPossessableComponent.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "PossessablePawn.h"
#include "../DebugToolsSTDStringModifiers.h"

UPaintingPossessableComponent::UPaintingPossessableComponent()
{
    StamFrontCost = 0.0f;
    StamDrainRate = 10.0f;
    SetActiveStaminaDrain(false);
    ConnectedNetworks = std::list<FString>{};
    Manager = nullptr;
    State = state::Inactive;
    TimeTillUpdate = 0.0f;
    DefaultExitPoint = FTransform{};
    bLeaving = false;
}

UPaintingPossessableComponent* UPaintingPossessableComponent::FindValidPainting(bool& b_valid, bool b_forward)
{
    auto it = PaintingDataPackage.Path.find(this); // Start @ us in list
    b_valid = false;
    for (int sanity = PaintingDataPackage.Path.size(); sanity > 0; --sanity)
    {
        if (b_forward)
        {
            ++it;
            if (it == PaintingDataPackage.Path.end()) // Loop if @ end
                it = PaintingDataPackage.Path.begin();
        }
        else
        {
            if (it == PaintingDataPackage.Path.begin()) // Loop if @ end
                it = PaintingDataPackage.Path.end();
            --it;
        }
        
        UPaintingPossessableComponent* p = *it;
        if (!p->IsInUse() && !(p != PaintingDataPackage.RootPainting && p->State == state::Root))
        {
            b_valid = true;
            return p;
        }
    }
    return nullptr;
}

void UPaintingPossessableComponent::ApplyBlindEffect(float time_remaining)
{
    // apply affect to camera
    // blend based on time remaining (it wains as time passes)
}

void UPaintingPossessableComponent::RightTriggerRelease_Implementation()
{
    bool valid;
    UPaintingPossessableComponent* p = FindValidPainting(valid, true);
    if (valid)
        InternalPaintingPossession(p); // Move to next painting
    //else
        //SCREENMSGT("The Network is full, you cannot possess any aviable paintings", 5.0f);
}

void UPaintingPossessableComponent::LeftTriggerRelease_Implementation()
{
    bool valid;
    UPaintingPossessableComponent* p = FindValidPainting(valid, false);
    if (valid)
        InternalPaintingPossession(p); // Move to next painting
    //else
        //SCREENMSGT("The Network is full, you cannot possess any aviable paintings", 5.0f);
}

void UPaintingPossessableComponent::BlueprintConstructorInit(TArray<FString> connected_networks, FString painting_name)
{
    PaintingName = painting_name;
    auto owner = Cast<APossessablePawn>(GetOwner());
    DefaultExitPoint = FTransform{owner->ExitPoint->GetComponentTransform()};
    
    // Either grab or create a reference to PaintingManager in world then save ptr
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
    // -----
    
    // For the sake of my sanity: convert TArray into std::list for c++ use
    for (auto string : connected_networks)
    {
        ConnectedNetworks.push_back(string);
    }
    // -----
    
    Manager->AppendToNetworks(ConnectedNetworks, this);
}

void UPaintingPossessableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsInUse())
    {
        TimeTillUpdate -= DeltaTime;
        if (TimeTillUpdate < 0)
        {
            std::string msg = "";
            msg += std::to_string(PaintingName) + "'s Update:";
            msg += "\nState = " + std::to_string(State);
            msg += "\nIsLeaving = " + std::to_string(bLeaving);
            msg += "\nbIsExitingNetwork = " + std::to_string(PaintingDataPackage.bIsExitingNetwork);
            //SCREENMSGT(msg.c_str(), INTERNAL_UPDATE);
            TimeTillUpdate = INTERNAL_UPDATE;
        }
    
        PaintingDataPackage.TimeBeingBlind = std::max(PaintingDataPackage.TimeBeingBlind - DeltaTime, 0.0f);
        ApplyBlindEffect(0.0f);
    
        if (bLeaving)
        {
            std::string msg{};
            msg += "Player in painting \"" + std::to_string(PaintingName) + "\" is leaving";
            //SCREENMSGT(msg.c_str(), 3.0f);
            Eject();
            bLeaving = false;
        }
    
        SetActiveStaminaDrain(false);
    }
}

void UPaintingPossessableComponent::OnInteract_Implementation()
{
    if (!PaintingDataPackage.bInit)                                                          // Just entered the network
    {
        State = state::Root; // We're a root now
        NewPaintingDataPackage();
        std::string msg = std::to_string(PaintingName) + " has created a data package";
        //SCREENMSGT(msg.c_str(), 2.0f);
    }
    else if (State != state::Root)                                              // Entering Painting from inside Network
    {
        State = state::NonRoot;
    }
    else if (State == state::Root && PaintingDataPackage.bIsExitingNetwork)                       // leaving the network
    {
        bLeaving = true;
    }
    PaintingDataPackage.bIsExitingNetwork = true;
}

void UPaintingPossessableComponent::NewPaintingDataPackage()
{
    // Create transferable struct of data for pathing in the painting network
    PaintingDataPackage = FPaintingTransitionPackage{};
    PaintingDataPackage.Path = Manager->GetPaintingPath(ConnectedNetworks, this);
    PaintingDataPackage.RootPainting = this;
    PaintingDataPackage.RootExitPoint = FTransform{DefaultExitPoint};
    PaintingDataPackage.TimeBeingBlind = 0.0f;
    PaintingDataPackage.bInit = true;
    PaintingDataPackage.bIsExitingNetwork = false;
}

void UPaintingPossessableComponent::InternalPaintingPossession(UPaintingPossessableComponent* target_painting)
{
    PaintingDataPackage.bIsExitingNetwork = false;
    target_painting->PaintingDataPackage = PaintingDataPackage;
    
    SetNextExit(Cast<APossessablePawn>(target_painting->GetOwner()));
    Eject();
}

void UPaintingPossessableComponent::HitByFlashLight_Implementation()
{
    if (IsInUse())
    {
        std::string m = std::to_string(PaintingName) + " being hit";
        //SCREENMSGT(m.c_str(), 0.1f);
        
        if (State == state::Root)
        {
            SetActiveStaminaDrain(true);
        }
        else
        {
            PaintingDataPackage.TimeBeingBlind = 5.0f;
        }
    }
}

void UPaintingPossessableComponent::BeginningEndInteract()
{
    if (PaintingDataPackage.bIsExitingNetwork && State != state::Root)
    {
        // Returning to Root Painting
        SetNextExit(Cast<APossessablePawn>(PaintingDataPackage.RootPainting->GetOwner()));
    }
}

void UPaintingPossessableComponent::EndInteract_Implementation()
{
    if (State == state::NonRoot || PaintingDataPackage.bIsExitingNetwork)
        State = state::Inactive;
    PaintingDataPackage.bInit = false;
}

void UPaintingPossessableComponent::ButtonTopRelease_Implementation()
{
    Manager->PrintNetworks();
}

void UPaintingPossessableComponent::ScareButton_Implementation()
{
    std::string msg = std::string{};
    msg += std::to_string(PaintingName) + "'s exit location is " +
        std::to_string(PaintingDataPackage.RootExitPoint.GetLocation());
    //SCREENMSGT(msg.c_str(), 5.0f);
}

/// Painting manager functions                                                                      /// PAINTING MANAGER
/// A "hidden" class that is handled entirely through the PaintingPossessibleComponent class
std::set<UPaintingPossessableComponent*> APaintingManager::GetPaintingPath(
    std::list<FString> connected_networks,
    UPaintingPossessableComponent* painting)
{
    std::set<UPaintingPossessableComponent*> path{};
   
    for (FString network : connected_networks)
    {
        for (auto it = Networks.find(network); it != Networks.end(); ++it)
        {
            if (it->first.Compare(network) != 0)
            {
                break;
            }
            else if (it->second != painting)
            {
                path.insert(it->second);
            }
        }
    }
    path.insert(path.end(), painting);
    
    return path;
}

void APaintingManager::PrintNetworks() const
{
    std::string msg = "{\n";
    for (auto pair : Networks)
    {
        if (pair.first != "")
            msg += "{ " + std::to_string(pair.first) + ", " +
                std::to_string(pair.second->GetPaintName()) + " }\n";
    }
    msg += " }";
    //SCREENMSGT(msg.c_str(), 10.0f);
}

void APaintingManager::AppendToNetworks(std::list<FString> connected_networks,
    UPaintingPossessableComponent* painting)
{
    for (FString network : connected_networks)
    {
        Networks.insert(std::pair<FString, UPaintingPossessableComponent*>{network, painting});
    }
}
