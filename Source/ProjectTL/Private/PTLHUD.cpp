// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 락온한 Target에 Texture를 표시해줄 HUD 클래스입니다.
 *=========================================================================*/

#include "PTLHUD.h"
#include "PTLTargetLockComponent.h"
#include "PTLPlayer.h"
#include "PTLPlayerController.h"
#include "PTLEnemy.h"
#include "PTLTargetComponent.h"
#include "Kismet/GameplayStatics.h"

APTLHUD::APTLHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture> Texture_Indicator(TEXT("Texture2D'/Game/Texture/T_TargetLockIndicator.T_TargetLockIndicator'"));
	if (Texture_Indicator.Succeeded())
	{
		IndicatorTexture = Texture_Indicator.Object;
	}

	IndicatorSize = 25.0f;
}

void APTLHUD::DrawHUD()
{
	Super::DrawHUD();

	APTLPlayerController* PlayerController = Cast<APTLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APTLPlayer* Player = Cast<APTLPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APTLEnemy* TargetEnemy = Cast<APTLEnemy>(Player->GetTargetLockComponent()->GetTargetActor());
	if (TargetEnemy)
	{
		FVector2D ScreenLocation;
		UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetEnemy->GetTargetComponent()->GetComponentLocation(), ScreenLocation, false);

		ScreenLocation = FVector2D(ScreenLocation.X - IndicatorSize / 2.0f, ScreenLocation.Y - IndicatorSize / 2.0f);

		DrawTexture(IndicatorTexture, ScreenLocation.X, ScreenLocation.Y, IndicatorSize, IndicatorSize, 0.0f, 0.0f, 1.0f, 1.0f);
	}
}
