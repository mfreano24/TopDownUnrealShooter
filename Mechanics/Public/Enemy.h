// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class MECHANICS_API AEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* HitSphere;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
};

//////////////////////////////////////////////////////////////////////////////////

UCLASS()
class MECHANICS_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
