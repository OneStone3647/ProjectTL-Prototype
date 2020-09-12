// Fill out your copyright notice in the Description page of Project Settings.


#include "PTLGameMode.h"
#include "PTLPlayer.h"
#include "PTLHUD.h"
#include "PTLPlayerController.h"

APTLGameMode::APTLGameMode()
{
	// DefaultPawnClass 설정
	static ConstructorHelpers::FClassFinder<APawn> BP_PTLPlayer(TEXT("Blueprint'/Game/Blueprints/BP_PTLPlayer.BP_PTLPlayer_C'"));
	if (BP_PTLPlayer.Class != NULL)
	{
		DefaultPawnClass = BP_PTLPlayer.Class;
	}

	// HUD 설정
	static ConstructorHelpers::FClassFinder<APTLHUD> BP_PTLHUD(TEXT("Blueprint'/Game/Blueprints/BP_PTLHUD.BP_PTLHUD_C'"));
	if (BP_PTLHUD.Class != NULL)
	{
		HUDClass = BP_PTLHUD.Class;
	}

	// PlayerController 설정
	PlayerControllerClass = APTLPlayerController::StaticClass();
}
