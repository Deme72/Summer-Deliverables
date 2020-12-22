// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NavigationNetworkManager.generated.h"


UCLASS(Blueprintable, BlueprintType)
class POLTERHEIST_API ANavigationNetworkManager : public AActor
{
	GENERATED_BODY()

protected:
	/// list of navigation nodes in the network
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ANavigationNode*> NavigationNetwork;

	/// true: shows all nodes
	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
	bool bDebugNodes;

	/// true: shows all edges
	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
	bool bDebugEdges;
	
public:	
	/// Sets default values for this actor's properties
	ANavigationNetworkManager();

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Gets the neighboring nodes of the given node
	UFUNCTION(BlueprintCallable)
	TArray<ANavigationNode*> GetNodeNeighbors(ANavigationNode* targetNode, ANavigationNode* prevNode, bool &isNew) const;

	UFUNCTION(BlueprintCallable)
	TArray<ANavigationNode*> GetPoiNeighbors(ANavigationNode* targetNode, bool& hasPoi, ANavigationNode* prevPoi = nullptr) const;

	UFUNCTION(BlueprintCallable)
	ANavigationNode* GetNearestNode(FVector origin) const;
};
