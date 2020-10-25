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
		switch (type)
		{
		case HALLWAY:
			c = FColor::Blue;
			break;
		case ROOM:
			c = FColor::Magenta;
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
			c = FColor::White;
		}

		DrawDebugPoint(GetWorld(), GetActorLocation(), 10, c, true, 1, 0);

		// edges
		for (ANavigationNode* connection : neighbors)
		{
			// room to room
			if (type == NavNodeType::ROOM && connection->type == NavNodeType::ROOM)
			{
				c = FColor::Purple;
			}
			// treasure or connected to treasure
			else if (type == NavNodeType::TREASURE || connection->type == NavNodeType::TREASURE)
			{
				c = FColor::Orange;
			}
			// POI or connected to POI
			else if (type == NavNodeType::POI || connection->type == NavNodeType::POI)
			{
				c = FColor::Cyan;
			}
			// connected to exit
			else if (connection->type == NavNodeType::EXIT)
			{
				c = FColor::Red;
			}
			// entrance
			else if (type == NavNodeType::ENTRANCE)
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
