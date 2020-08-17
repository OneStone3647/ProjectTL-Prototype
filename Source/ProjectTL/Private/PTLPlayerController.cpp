// Fill out your copyright notice in the Description page of Project Settings.


#include "PTLPlayerController.h"

void APTLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 뷰포트에 마우스를 가둡니다.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
