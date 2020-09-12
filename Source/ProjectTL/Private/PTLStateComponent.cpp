// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 상태를 나타내는 클래스입니다.
 *=========================================================================*/

#include "PTLStateComponent.h"

UPTLStateComponent::UPTLStateComponent()
{
	// TickComponent함수를 사용하지 않기 때문에 false로 설정합니다.
	PrimaryComponentTick.bCanEverTick = false;

	bIsDead = false;
}
