// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTL.h"
#include "Components/SceneComponent.h"
#include "PTLTargetComponent.generated.h"

/*=========================================================================*
 * 플레이어의 HUD에 락온을 표시할 위치를 잡는 SceneComponent입니다.
 *=========================================================================*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTL_API UPTLTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPTLTargetComponent();
};
