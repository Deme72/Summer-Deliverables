// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ButtonMash.h"

#include <string>
#include "Polterheist/External/DefinedDebugHelpers.h"


UCPP_ButtonMash_Component::UCPP_ButtonMash_Component():UStaticPossessableComponent()
{
    ButtonList.Add(TEXT("LeftTrigger"));
    ButtonList.Add(TEXT("RightTrigger"));
    ButtonList.Add(TEXT("TopButton"));
    ButtonList.Add(TEXT("MoveButton"));
    ButtonList.Add(TEXT("ScareButton"));
    
    MashDrainRatePerSecond = 10.0;
    MashStrength = 5.0;
    
    CurrentLevel = 0;
    MaxLevel = 100;
    
    StaminaPoints.Add(-10);
    StaminaPoints.Add(10);
    StaminaPoints.Add(30);
    StaminaPoints.Add(65);
    StaminaPoints.Add(80);
    StaminaPoints.Add(90);
    StaminaPoints.Add(1000);
    
    PointIndex = 0;
    StaminaChunk = 10000;
    TempPoint = 0;

    AnimTime = 1.2;
    CurrentTime = -1;
}

void UCPP_ButtonMash_Component::PickButton()
{
    const int32 index = FMath::RandRange(0, ButtonList.Num() - 1);
    
    CurrentButton = ButtonList[index];
    SCREENMSGT(CurrentButton, 1.8675309);
}

void UCPP_ButtonMash_Component::OnInteract_Implementation()
{
    PickButton();
}

void UCPP_ButtonMash_Component::DoMash(FString button)
{
    if(CurrentButton == button)
    {
        CurrentLevel += MashStrength;
        
    }
    else
    {
        SetStamina(-1.0, true);
    }
}

void UCPP_ButtonMash_Component::ScareButton_Implementation()
{
    DoMash("ScareButton");
}

void UCPP_ButtonMash_Component::ButtonTop_Implementation()
{
    DoMash("TopButton");
}

void UCPP_ButtonMash_Component::MoveButton_Implementation()
{
    DoMash("MoveButton");
}

void UCPP_ButtonMash_Component::RightTrigger_Implementation()
{
    DoMash("RightTrigger");
}

void UCPP_ButtonMash_Component::LeftTrigger_Implementation()
{
    DoMash("LeftTrigger");
}

void UCPP_ButtonMash_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if(CurrentTime <= 0.0)
    {
        if(CurrentLevel > 0.0)
        {
            CurrentLevel = CurrentLevel - (MashDrainRatePerSecond * DeltaTime);
            SCREENMSGT(std::to_string(CurrentLevel).c_str(), 0.2);
        }
        StaminaPointLogic();
        if(CurrentLevel > MaxLevel)
        {
            Scare();
            //if( GetOwner()->IsA(null))
            //{
            //GetOwner()->DidScare();
            //}
            CurrentTime = AnimTime;
            CurrentLevel = 0.0;
        }
    }
    else
    {
        CurrentTime -= DeltaTime;
        if(CurrentTime <= 0)
        {
            Eject();
        }
    }
}

void UCPP_ButtonMash_Component::StaminaPointLogic()
{
    // Did we go down a level?
    if(CurrentLevel < StaminaPoints[PointIndex])
    {
        PointIndex = FMath::Max(PointIndex-1, 0);
        SetStamina(StaminaChunk);
    }
    else
    {
        // Did we go up a level?
        if(CurrentLevel > StaminaPoints[PointIndex+1])
        {
            PointIndex += 1;
        }
    }
}
