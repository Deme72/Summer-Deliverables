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
		
		for (ANavigationNode* node : navigationNetwork)
		{
			// nodes
			if (bDebugNodes)
			{
				switch (node->type)
				{
				case HALLWAY:
					c = FColor::Turquoise;
					break;
				case ROOM:
					c = FColor::Magenta;
					break;
				case POI:
					c = FColor::Cyan;
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
				
				DrawDebugPoint(GetWorld(), node->GetActorLocation(), 15, c, true, -1, 0);
			}

			// edges
			if (bDebugEdges)
			{
				for (ANavigationNode* connection : node->neighbors)
				{
					// room to room
					if (node->type == NavNodeType::ROOM && connection->type == NavNodeType::ROOM)
					{
						c = FColor::Purple;
					}
					// treasure or connected to treasure
					else if (node->type == NavNodeType::TREASURE || connection->type == NavNodeType::TREASURE)
					{
						c = FColor::Orange;
					}
					// connected to exit
					else if (connection->type == NavNodeType::EXIT)
					{
						c = FColor::Red;
					}
					// entrance
					else if (node->type == NavNodeType::ENTRANCE)
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
	TArray<ANavigationNode*> allNeighbors = targetNode->neighbors;

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
			if (node->type == NavNodeType::HALLWAY || node->type == NavNodeType::ROOM)
			{
				newNeighbors.Add(node);
			}
		}
	}
	
	isNew = true;
	return newNeighbors;
}

TArray<ANavigationNode*> ANavigationNetworkManager::GetPoiNeighbors(ANavigationNode* targetNode, bool& hasPoi) const
{
	TArray<ANavigationNode*> allNeighbors = targetNode->neighbors;
	hasPoi = true;

	// get list of all points of interest attached to the target node
	TArray<ANavigationNode*> poiNeighbors;
	for (ANavigationNode* node : allNeighbors)
	{
		if (node->type == NavNodeType::POI)
		{
			poiNeighbors.Add(node);
		}
	}

	// check if the target node has no points of interest
	if (poiNeighbors.Num() == 0)
	{
		hasPoi = false;
		return poiNeighbors;
	}

	return poiNeighbors;
}

ANavigationNode* ANavigationNetworkManager::GetNearestNode(FVector origin) const
{
	//UE_LOG(LogTemp, Warning, TEXT("Get Nearest Node test"));
	
	float closestDist = -1.0;
	ANavigationNode* closestNode = nullptr;
	
	for (ANavigationNode* testNode : navigationNetwork)
	{
		float testDist = FVector::Dist(origin, testNode->GetActorLocation());

		//UE_LOG(LogTemp, Warning, TEXT("TestDist: %f with node: %p"), testDist, testNode);

		if (closestDist < 0 || testDist < closestDist)
		{
			closestDist = testDist;
			closestNode = testNode;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Get Nearest Node end"));
	return closestNode;
}


