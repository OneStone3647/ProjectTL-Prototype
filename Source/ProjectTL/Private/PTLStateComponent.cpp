// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 상태를 나타내는 클래스입니다.
 *=========================================================================*/

#include "PTLStateComponent.h"

UPTLStateComponent::UPTLStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsDead = false;
}
