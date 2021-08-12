// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../MechanicsCharacter.h"

// Sets default values
ACollectible::ACollectible()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = SphereComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlap);
	
}

void ACollectible::OnOverlap(UPrimitiveComponent* comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMechanicsCharacter* Player = Cast<AMechanicsCharacter>(OtherActor);

	if (Player) {
		int newValue = Player->AddNewCollectible();
		UE_LOG(LogTemp, Warning, TEXT("Collectibles picked up: %d"), newValue);
		Destroy();
	}
}
