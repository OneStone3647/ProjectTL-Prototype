// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 타겟의 Transform을 가진 SceneComponent입니다.
 *=========================================================================*/

#include "PTLTargetComponent.h"

UPTLTargetComponent::UPTLTargetComponent()
{
	// TickComponent를 사용하지 않기 때문에 false로 설정합니다.
	PrimaryComponentTick.bCanEverTick = false;
}
