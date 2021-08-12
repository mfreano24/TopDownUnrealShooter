// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechanicsGameMode.h"
#include "MechanicsPlayerController.h"
#include "MechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMechanicsGameMode::AMechanicsGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMechanicsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}