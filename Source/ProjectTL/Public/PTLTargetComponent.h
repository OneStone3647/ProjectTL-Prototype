// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 타겟의 Transform을 가진 SceneComponent입니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "Components/SceneComponent.h"
#include "PTLTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTL_API UPTLTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPTLTargetComponent();
};
