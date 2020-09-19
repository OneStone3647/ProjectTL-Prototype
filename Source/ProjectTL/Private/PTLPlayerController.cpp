// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 플레이어 캐릭터의 컨트롤러 클래스입니다.
 *=========================================================================*/

#include "PTLPlayerController.h"

void APTLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 뷰포트에 마우스를 가둡니다.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
