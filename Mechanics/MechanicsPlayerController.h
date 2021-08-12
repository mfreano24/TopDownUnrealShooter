// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MechanicsPlayerController.generated.h"

UCLASS()
class AMechanicsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMechanicsPlayerController();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();
};


