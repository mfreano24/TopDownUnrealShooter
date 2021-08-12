// Fill out your copyright notice in the Description page of Project Settings.


#include "KothZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
AKothZone::AKothZone()
{
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));

	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxTrigger->SetBoxExtent(FVector(200.0f));
	RootComponent = BoxTrigger;

	FloorDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	FloorDecal->DecalSize = FVector(200.0f);
	FloorDecal->SetupAttachment(BoxTrigger);
}

// Called when the game starts or when spawned
void AKothZone::BeginPlay()
{
	Super::BeginPlay();
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKothZone::HandleOverlap);
	
}

void AKothZone::HandleOverlap(UPrimitiveComponent* comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with player!"));
}
