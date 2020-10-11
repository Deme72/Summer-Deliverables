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
		for (ANavigationNode* node : navigationNetwork)
		{
			// nodes
			if (bDebugNodes)
			{
				if (node->type == NavNodeType::HALLWAY)
				{
					DrawDebugPoint(GetWorld(), node->GetActorLocation(), 10, FColor::Cyan, true, -1, 1);
				}
				else if (node->type == NavNodeType::ROOM)
				{
					DrawDebugPoint(GetWorld(), node->GetActorLocation(), 10, FColor::Red, true, -1, 1);
				}
			}

			// edges
			if (bDebugEdges)
			{
				for (ANavigationNode* connection : node->neighbors)
				{
					if (node->type == NavNodeType::HALLWAY && connection->type == NavNodeType::HALLWAY)
					{
						DrawDebugLine(GetWorld(), node->GetActorLocation(), connection->GetActorLocation(), FColor::Blue, true, -1, 0, 10);
					}
					else if (node->type == NavNodeType::ROOM && connection->type == NavNodeType::ROOM)
					{
						DrawDebugLine(GetWorld(), node->GetActorLocation(), connection->GetActorLocation(), FColor::Red, true, -1, 0, 10);
					}
					else
					{
						DrawDebugLine(GetWorld(), node->GetActorLocation(), connection->GetActorLocation(), FColor::Purple, true, -1, 0, 10);
					}
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

	// check if neighbors has another node other than prevNode
	if (allNeighbors.Num() < 2)
	{
		// only has prevNode
		isNew = false;
		return allNeighbors;
	}

	// has new nodes
	TArray<ANavigationNode*> newNeighbors;
	for (ANavigationNode* node : allNeighbors)
	{
		if (prevNode != node)
		{
			newNeighbors.Add(node);
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


