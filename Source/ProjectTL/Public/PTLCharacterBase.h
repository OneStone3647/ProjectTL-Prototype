// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTL.h"
#include "PTLStateComponent.h"
#include "GameFramework/Character.h"
#include "PTLCharacterBase.generated.h"

/*=========================================================================*
 * 모든 캐릭터의 베이스가 되는 클래스입니다.
 *=========================================================================*/
UCLASS()
class PROJECTTL_API APTLCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APTLCharacterBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	UPTLStateComponent* StateComponent;

public:
	FORCEINLINE UPTLStateComponent* GetStateComponent() const
	{
		return StateComponent;
	}
};
