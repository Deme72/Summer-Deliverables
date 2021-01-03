// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNetworkManager.h"
#include "DrawDebugHelpers.h"
#include "NavigationNode.h"

// Sets default values
ANavigationNetworkManager::ANavigationNetworkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavigationNetworkManager::BeginPlay()
{
	Super::BeginPlay();

	// draw debug lines to all connected nodes
	if (bDebugNodes || bDebugEdges)
	{
		FColor c;
		
		for (ANavigationNode* node : NavigationNetwork)
		{
			// nodes
			if (bDebugNodes)
			{
				switch (node->Type)
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
				
				DrawDebugPoint(GetWorld(), node->GetActorLocation(), 15, c, true, -1, 0);
			}

			// edges
			if (bDebugEdges)
			{
				for (ANavigationNode* connection : node->Neighbors)
				{
					// treasure or connected to treasure
					if (node->Type == NavNodeType::TREASURE || connection->Type == NavNodeType::TREASURE)
					{
						c = FColor::Orange;
					}
					// POI or connected to POI
					else if (node->Type == NavNodeType::POI || connection->Type == NavNodeType::POI)
					{
						c = FColor::Cyan;
					}
					// Room or connected to Room
					else if (node->Type == NavNodeType::ROOM || connection->Type == NavNodeType::ROOM)
					{
						c = FColor::Purple;
					}
					// connected to exit
					else if (connection->Type == NavNodeType::EXIT)
					{
						c = FColor::Red;
					}
					// entrance
					else if (node->Type == NavNodeType::ENTRANCE)
					{
						c = FColor::Emerald;
					}
					else
					{
						c = FColor::Blue;
					}

					DrawDebugLine(GetWorld(), node->GetActorLocation(), connection->GetActorLocation(), c, true, -1, 0, 10);
				}
			}
		}
	}
}

// Called every frame
void ANavigationNetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

TArray<ANavigationNode*> ANavigationNetworkManager::GetNodeNeighbors(ANavigationNode* targetNode, ANavigationNode* prevNode, bool& isNew) const
{
	TArray<ANavigationNode*> allNeighbors = targetNode->Neighbors;

	// check if node has only one neighbor
	if (allNeighbors.Num() == 1)
	{
		// check if only neighbor is new or old
		if (allNeighbors[0] == prevNode)
		{
			// only has prevNode
			isNew = false;
			return allNeighbors;
		}
	}

	// has new nodes
	TArray<ANavigationNode*> newNeighbors;
	for (ANavigationNode* node : allNeighbors)
	{
		if (node != prevNode)
		{
			if (node->Type == NavNodeType::HALLWAY ||
				node->Type == NavNodeType::TRANSITION ||
				node->Type == NavNodeType::ROOM)
			{
				newNeighbors.Add(node);
			}
		}
	}

	// "new" nodes were only poi's (newNeighbors is empty)
	if (newNeighbors.Num() == 0)
	{
		isNew = false;
	}
	else
	{
		isNew = true;
	}
	
	return newNeighbors;
}

TArray<ANavigationNode*> ANavigationNetworkManager::GetPoiNeighbors(ANavigationNode* targetNode, bool& hasPoi, ANavigationNode* prevPoi) const
{
	TArray<ANavigationNode*> allNeighbors = targetNode->Neighbors;
	hasPoi = true;

	// get list of all points of interest attached to the target node
	TArray<ANavigationNode*> poiNeighbors;
	for (ANavigationNode* node : allNeighbors)
	{
		if (node->Type == NavNodeType::POI && node != prevPoi)
		{
			poiNeighbors.Add(node);
		}
	}

	// check if the target node has no points of interest
	if (poiNeighbors.Num() == 0)
	{
		hasPoi = false;
	}

	return poiNeighbors;
}

ANavigationNode* ANavigationNetworkManager::GetNearestNode(FVector origin) const
{
	//UE_LOG(LogTemp, Warning, TEXT("Get Nearest Node test"));
	
	float closestDist = -1.0;
	ANavigationNode* closestNode = nullptr;
	
	for (ANavigationNode* testNode : NavigationNetwork)
	{
		if (testNode->Type != NavNodeType::POI)
		{
			float testDist = FVector::Dist(origin, testNode->GetActorLocation());

			if (closestDist < 0 || testDist < closestDist)
			{
				closestDist = testDist;
				closestNode = testNode;
			}
		}
	}
	
	return closestNode;
}


