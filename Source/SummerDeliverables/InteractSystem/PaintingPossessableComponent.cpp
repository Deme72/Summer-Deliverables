// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintingPossessableComponent.h"
#include "SummerDeliverables/DefinedDebugHelpers.h"
#include "PossessablePawn.h"

UPaintingPossessableComponent::UPaintingPossessableComponent()
{
    StamFrontCost=0.0f;
    StamDrainRate=0.0f;
    ConnectedNetworks = std::list<FString>{};
    Manager = nullptr;
    bIsRoot = false;
    bIsNonRoot = false;
}

void UPaintingPossessableComponent::SetCurrentExitPoint(USceneComponent* NewExitPoint)
{
    currentExitPoint=NewExitPoint;
}

void UPaintingPossessableComponent::NewPaintingDataPackage()
{
    bIsRoot = true; // We're a root now

    // Create transferable struct of data for pathing in the painting network
    PaintingDataPackage = FPaintingTransitionPackage{};
    //PaintingDataPackage.Path = std::list<UPaintingPossessableComponent*>{};
    PaintingDataPackage.Path = Manager->GetPaintingPath(ConnectedNetworks, this);
    PaintingDataPackage.RootPainting = this;
    PaintingDataPackage.TimeBeingBlind = 0.0f;
    PaintingDataPackage.Instantiated = true;
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
    
    Manager->AppendToConnectedNetworks(ConnectedNetworks, this);
    Manager->PrintNetworks();
}

void UPaintingPossessableComponent::InternalPaintingPossession(UPaintingPossessableComponent* target_painting)
{
    // 'leave no trace'
    target_painting->PaintingDataPackage = PaintingDataPackage;
    if (!target_painting->bIsRoot)
        target_painting->bIsNonRoot = true;
    bIsNonRoot = false;
    
    // -----
    
    SetNextExit(Cast<APossessablePawn>(target_painting->GetOwner()));
    Eject();
}

void UPaintingPossessableComponent::EndInternalPaintingPossession()
{
    // unnecessary function?
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

        
        p = *iter; // something about this isn't right THE SECOND time. The first jump to another prop is fine
        valid = true;
        
        if (p != PaintingDataPackage.RootPainting && p->bIsRoot) // Crashes here
        {
            valid = false;
            SCREENMSGT("Root Painting && non-owner Failure", 2.0f);
        }

        auto owner = Cast<APossessablePawn>(p->GetOwner()); // crashes here when you comment
        if (owner)                                                            // the previous crash out
            if (owner->IsPossessing())
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

void UPaintingPossessableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    auto owner = Cast<APossessablePawn>(GetOwner());
    if (owner)
        if (owner->IsPossessing())
        {
            if (!bIsNonRoot && !bIsRoot)
            {
                std::string msg = std::to_string(PaintingName) + " has created a data package";
                SCREENMSGT(msg.c_str(), 5.0f);
                NewPaintingDataPackage();
            }
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
        Networks.insert(std::pair<FString, UPaintingPossessableComponent*>{network, painting});
    }
}

void UPaintingPossessableComponent::EndInteract_Implementation()
{
    SCREENMSGT("Leave Network", 5.0f); // crash here when unpossessing
    bIsNonRoot = false;
    PaintingDataPackage.RootPainting->bIsRoot = false;
    PaintingDataPackage.Instantiated = false;
}

void UPaintingPossessableComponent::ButtonTopRelease_Implementation()
{
    Manager->PrintNetworks();
}

void UPaintingPossessableComponent::ScareButton_Implementation()
{
    std::string msg = "{\n";
    for (auto name : ConnectedNetworks)
    {
        msg += std::to_string(name) + ",\n";
    }
    msg += " }";
    SCREENMSGT(msg.c_str(), 10.0f);
}


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

void APaintingManager::BeginPlay()
{
    Super::BeginPlay();
    PrintNetworks();
}
