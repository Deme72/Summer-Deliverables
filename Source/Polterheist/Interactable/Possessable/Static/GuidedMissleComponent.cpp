// Fill out your copyright notice in the Description page of Project Settings.


#include "GuidedMissleComponent.h"


void UGuidedMissleComponent::ScareButton_Implementation()
{
	if (isControling==true)
	{
		if(Powermode==true)
			if(bar>.98)
				power=1.0;
			else
				power=bar;
			barDir=-1.0;
			Accuracymode=true;
			Powermode=false;
		if(Accuracymode==true)
		{
			if(bar<.01)
				accuracy=0.0;
			else
				accuracy=bar;
		}
	}
}

void UGuidedMissleComponent::OnInteract_Implementation()
{
	isControling=true;
	
}

void UGuidedMissleComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	//makes variable amount for what bar is at for power and accuracy 
	if (isControling==true)
	{
		bar=bar+(DeltaTime*barDir);
		if (bar>=1.0)
			barDir=-1.0;
		if (bar<=0.0)
			barDir =1.0;
	}
	//sets values back to base numbers
	if (isControling==false)
	{
		bar=0.0;
		barDir=1.0;
	}
}
