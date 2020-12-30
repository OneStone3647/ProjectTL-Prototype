// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTL.h"
#include "Components/ActorComponent.h"
#include "PTLStateComponent.generated.h"

/*=========================================================================*
 * 캐릭터의 상태를 나타내는 클래스입니다.
 *=========================================================================*/
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
	bool GetIsDead() const;
	void SetIsDead(bool bFlag);
};
