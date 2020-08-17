// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 플레이어 캐릭터의 컨트롤러 클래스입니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "GameFramework/PlayerController.h"
#include "PTLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTL_API APTLPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
