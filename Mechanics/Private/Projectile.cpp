// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "../MechanicsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	StaticMesh->SetupAttachment(Sphere);


	//sphere overlap event
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//SetFireDirection(5.0f, FVector(0.0f, 1.0f, 0.0f));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	
	
}

void AProjectile::Destroyed()
{
	FVector Spawn = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, Spawn, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + MoveDirection);

}

void AProjectile::SetFireDirection(float speed, FVector direction)
{
	MoveDirection = speed * direction;
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if it hits an enemy do damage to them
	UE_LOG(LogTemp, Warning, TEXT("Destroying projectile..."));
	Destroy();

}

