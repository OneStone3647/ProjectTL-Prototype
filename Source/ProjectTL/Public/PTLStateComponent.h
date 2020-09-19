// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 상태를 나타내는 클래스입니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "Components/ActorComponent.h"
#include "PTLStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTL_API UPTLStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTLStateComponent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

public:
	FORCEINLINE bool GetIsDead() const
	{
		return bIsDead;
	}
	FORCEINLINE void SetIsDead(bool bFlag)
	{
		bIsDead = bFlag;
	}
};
