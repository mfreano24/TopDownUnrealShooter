// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Public/Projectile.h"

AMechanicsCharacter::AMechanicsCharacter()
{
	//create static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetCapsuleComponent());


	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 80.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	MaxHealth = 3;
	CurrentHealth = MaxHealth;
}

void AMechanicsCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	//MOVEMENT
	AddMovementInput(MoveDirection);

	//CURSOR
	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{

			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);

			//rotate the player to face the cursor
			if (IsLocallyControlled()) {
				FVector rot = FVector(TraceHitResult.Location.X, TraceHitResult.Location.Y, GetActorLocation().Z);
				FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), rot);
				SetActorRotation(lookAt.Add(0.0f, -90.0f, 0.0f));
			}
			
		}
	}


}

void AMechanicsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	MoveDirection = FVector(0.0f);
	InputComponent->BindAxis("MoveForward", this, &AMechanicsCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMechanicsCharacter::MoveRight);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMechanicsCharacter::FireProjectile);
}


int AMechanicsCharacter::Damage(AActor* Shooter)
{
	CurrentHealth--;
	if (CurrentHealth <= 0) {
		//die
	}
	return CurrentHealth;
}


void AMechanicsCharacter::MoveForward(float scale)
{
	MoveDirection.X = scale;
}

void AMechanicsCharacter::MoveRight(float scale)
{
	MoveDirection.Y = scale;
}

void AMechanicsCharacter::FireProjectile()
{
	FVector Direction = FVector(TraceHitResult.Location.X, TraceHitResult.Location.Y, GetActorLocation().Z) - GetActorLocation();
	Direction.Normalize();

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(this->ProjectileClass, GetActorLocation() + 100.0f * Direction, FRotator::ZeroRotator);
	if (projectile) {
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));
		
		
		projectile->SetFireDirection(20.0f, Direction);
	}
}

