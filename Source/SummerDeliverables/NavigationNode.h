// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NavigationNode.generated.h"


UENUM(BlueprintType)
enum NavNodeType
{
	HALLWAY,
	TRANSITION,
	ROOM,
	POI,
	TREASURE,
	ENTRANCE,
	EXIT
};
 

UCLASS(Blueprintable, BlueprintType)
class SUMMERDELIVERABLES_API ANavigationNode : public AActor
{
	GENERATED_BODY()

	friend class ANavigationNetworkManager;

protected:
	// list of navigation nodes connected to this node (enemy can travel to these nodes)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ANavigationNode*> neighbors;

	// type of node (room, hallway)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<NavNodeType> type;

	// true: shows this node
	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
	bool bDebug;

public:	
	// Sets default values for this actor's properties
	ANavigationNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
