// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectTLGameMode.h"
#include "ProjectTLCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PTLPlayerController.h"

AProjectTLGameMode::AProjectTLGameMode()
{
	// PlayerBP를 DefaultPawnClass로 설정합니다
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// PTLPlayerController를 PlayerController 설정합니다.
	PlayerControllerClass = APTLPlayerController::StaticClass();
}
