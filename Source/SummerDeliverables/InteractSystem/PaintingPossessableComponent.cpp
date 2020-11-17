// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintingPossessableComponent.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "PossessablePawn.h"
#include "../DebugToolsSTDStringModifiers.h"

UPaintingPossessableComponent::UPaintingPossessableComponent()
{
    StamFrontCost = 0.0f;
    StamDrainRate = 0.0f;
    ConnectedNetworks = std::list<FString>{};
    Manager = nullptr;
    State = state::Inactive;
    TimeTillUpdate = 0.0f;
    DefaultExitPoint = FTransform{};
}

void UPaintingPossessableComponent::ApplyBlindEffect()
{
}

void UPaintingPossessableComponent::ApplyStaminaDrain(float delta_time)
{
}

void UPaintingPossessableComponent::RightTriggerRelease_Implementation()
{
    SCREENMSGT("Forward Iteration Through Network", 5.0f);
    
    auto iter = PaintingDataPackage.Path.find(this); // Start @ us in list
    UPaintingPossessableComponent* p;
    
    int loop_saftey_break = PaintingDataPackage.Path.size() * 2;
    bool valid;
    while (true)
    {
        loop_saftey_break--;
        ++iter;
        if (iter == PaintingDataPackage.Path.end()) // Loop if @ end
            iter = PaintingDataPackage.Path.begin();

        
        p = *iter; 
        valid = true;
        
        if (p != PaintingDataPackage.RootPainting && p->State == state::Root)
        {
            valid = false;
            SCREENMSGT("Root Painting && non-owner Failure", 2.0f);
        }

        auto owner = Cast<APossessablePawn>(p->GetOwner()); 
        if (owner)                                                            
            if (p->IsInUse())
            {
                valid = false;
                SCREENMSGT("IsPossessing Failure", 2.0f);
            }
        
        
        if (valid || loop_saftey_break <= 0) // If the painting is valid or we fail the sanity check
            break;
    }

    if (valid)
        InternalPaintingPossession(p); // Move to next painting
    else
        SCREENMSGT("The Network is full, you cannot possess any aviable paintings", 5.0f);
}

void UPaintingPossessableComponent::LeftTriggerRelease_Implementation()
{
    SCREENMSGT("Reverse Iteration Through Network", 5.0f);
    
    // DEBUG: Print out the current generated path
    auto path = Manager->GetPaintingPath(ConnectedNetworks, this);
    std::string msg = "{ ";
    for (auto s : path)
    {
        msg += "\n" + std::string(TCHAR_TO_UTF8(*s->PaintingName)) + ", ";
    }
    msg += "\n}";
    SCREENMSGT(msg.c_str(), 10.0f);
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
   
    TimeTillUpdate -= DeltaTime;
    if (TimeTillUpdate <= 0.0f && GetOwner())
    {
        std::string msg = "\n" + std::to_string(PaintingName) + "'s update:";
        msg += "\nState = " + std::to_string(State);

        SCREENMSGT(msg.c_str(), INTERNAL_UPDATE);
        TimeTillUpdate = INTERNAL_UPDATE;
    }
}

void UPaintingPossessableComponent::OnInteract_Implementation()
{
    if (!PaintingDataPackage.bInit)                                                       // Just entered the network
        {
            State = state::Root; // We're a root now
            NewPaintingDataPackage();
            std::string msg = std::to_string(PaintingName) + " has created a data package";
            SCREENMSGT(msg.c_str(), 2.0f);
        }
        else if (State != state::Root)                                                   // Entering Painting from inside Network
        {
            State = state::NonRoot;
            auto owner = Cast<APossessablePawn>(GetOwner());
            //owner->ExitPoint->SetWorldTransform(PaintingDataPackage.RootExitPoint);
        }
        else if (State == state::Root && PaintingDataPackage.bIsExitingNetwork)         // leaving the network
        {
            State = state::Inactive;
            PaintingDataPackage.bInit = false;
            Eject();
        }
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
    // 'leave no trace'
    target_painting->PaintingDataPackage = PaintingDataPackage;
    PaintingDataPackage.bInit = false; // ear marks that we haven't left the network
    
    // -----
    SetNextExit(Cast<APossessablePawn>(target_painting->GetOwner()));
    Eject();
}

void UPaintingPossessableComponent::BeginningEndInteract()
{
    if (PaintingDataPackage.bInit)
    {
        // Setup for leaving network
        PaintingDataPackage.bIsExitingNetwork = true;
        PaintingDataPackage.RootPainting->PaintingDataPackage = PaintingDataPackage;
        SetNextExit(Cast<APossessablePawn>(PaintingDataPackage.RootPainting->GetOwner()));
    }
    else
    {
        // Possessing a new painting in network
    }
}

void UPaintingPossessableComponent::EndInteract_Implementation()
{
    if (State == state::NonRoot)
        State = state::Inactive;
    auto owner = Cast<APossessablePawn>(GetOwner());
}

void UPaintingPossessableComponent::ButtonTopRelease_Implementation()
{
    Manager->PrintNetworks();
}

void UPaintingPossessableComponent::ScareButton_Implementation()
{
    std::string msg = std::string{};
    msg += std::to_string(PaintingName) + "'s exit location is " + std::to_string(PaintingDataPackage.RootExitPoint.GetLocation());
    SCREENMSGT(msg.c_str(), 5.0f);
}




/// Painting manager functions                                                                                                /// PAINTING MANAGER
/// A "hidden" class that is handled entirely through the PaintingPossessibleComponent class
std::set<UPaintingPossessableComponent*> APaintingManager::GetPaintingPath(std::list<FString> connected_networks,
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
            msg += "{ " + std::to_string(pair.first) + ", " + std::to_string(pair.second->GetPaintName()) + " }\n";
    }
    msg += " }";
    SCREENMSGT(msg.c_str(), 10.0f);
}

void APaintingManager::AppendToNetworks(std::list<FString> connected_networks,
    UPaintingPossessableComponent* painting)
{
    for (FString network : connected_networks)
    {
        Networks.insert(std::pair<FString, UPaintingPossessableComponent*>{network, painting});
    }
}
