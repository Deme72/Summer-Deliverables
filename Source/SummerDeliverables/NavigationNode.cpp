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
		// node
		if (type == NavNodeType::HALLWAY)
		{
			DrawDebugPoint(GetWorld(), GetActorLocation(), 10, FColor::Cyan, true, -1, 1);
		}
		else if (type == NavNodeType::ROOM)
		{
			DrawDebugPoint(GetWorld(), GetActorLocation(), 10, FColor::Red, true, -1, 1);
		}

		// edges
		for (ANavigationNode* connection : neighbors)
		{
			if (type == NavNodeType::HALLWAY && connection->type == NavNodeType::HALLWAY)
			{
				DrawDebugLine(GetWorld(), GetActorLocation(), connection->GetActorLocation(), FColor::Blue, true, -1, 0, 10);
			}
			else if (type == NavNodeType::ROOM && connection->type == NavNodeType::ROOM)
			{
				DrawDebugLine(GetWorld(), GetActorLocation(), connection->GetActorLocation(), FColor::Red, true, -1, 1, 10);
			}
			else
			{
				DrawDebugLine(GetWorld(), GetActorLocation(), connection->GetActorLocation(), FColor::Purple, true, -1, 1, 10);
			}
		}
	}
}
