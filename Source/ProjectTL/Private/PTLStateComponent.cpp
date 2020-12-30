// Fill out your copyright notice in the Description page of Project Settings.

#include "PTLStateComponent.h"

UPTLStateComponent::UPTLStateComponent()
{
	// Tick 함수를 사용하지 않습니다.
	PrimaryComponentTick.bCanEverTick = false;

	bIsDead = false;
}

 bool UPTLStateComponent::GetIsDead() const
 {
	 return bIsDead;
 }

 void UPTLStateComponent::SetIsDead(bool bFlag)
 {
	 bIsDead = bFlag;
 }

