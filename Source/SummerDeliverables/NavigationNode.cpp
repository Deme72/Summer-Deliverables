// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNode.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANavigationNode::ANavigationNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavigationNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
	// draw debug lines to all connected nodes	
	if (bDebug)
	{
		FColor c;
		
		// node
		switch (Type)
		{
		case HALLWAY:
			c = FColor::Blue;
			break;
		case TRANSITION:
			c = FColor::Magenta;
			break;
		case ROOM:
			c = FColor::White;
			break;
		case POI:
			c = FColor::Turquoise;
			break;
		case TREASURE:
			c = FColor::Yellow;
			break;
		case ENTRANCE:
			c = FColor::Green;
			break;
		case EXIT:
			c = FColor::Red;
			break;
		default:
			c = FColor::Black;
		}

		DrawDebugPoint(GetWorld(), GetActorLocation(), 10, c, true, 1, 0);

		// edges
		for (ANavigationNode* connection : Neighbors)
		{
			// treasure or connected to treasure
			if (Type == NavNodeType::TREASURE || connection->Type == NavNodeType::TREASURE)
			{
				c = FColor::Orange;
			}
			// POI or connected to POI
			else if (Type == NavNodeType::POI || connection->Type == NavNodeType::POI)
			{
				c = FColor::Cyan;
			}
			// Room or connected to Room
			else if (Type == NavNodeType::ROOM || connection->Type == NavNodeType::ROOM)
			{
				c = FColor::Purple;
			}
			// connected to exit
			else if (connection->Type == NavNodeType::EXIT)
			{
				c = FColor::Red;
			}
			// entrance
			else if (Type == NavNodeType::ENTRANCE)
			{
				c = FColor::Emerald;
			}
			else
			{
				c = FColor::Blue;
			}

			DrawDebugLine(GetWorld(), GetActorLocation(), connection->GetActorLocation(), c, true, 1, 0, 10);
		}
	}
}
