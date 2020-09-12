// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 기본 베이스 클래스입니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "GameFramework/Character.h"
#include "PTLCharacterBase.generated.h"

UCLASS()
class PROJECTTL_API APTLCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APTLCharacterBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	class UPTLStateComponent* StateComponent;

	// Get함수 FORCEINLINE 매크로로 Inline함수로 작성
public:
	// StateComponent Get함수
	UFUNCTION(BlueprintCallable, Category = "State")			// 임시로 사용하는 블루프린트 함수 선언
	FORCEINLINE class UPTLStateComponent* GetStateComponent() const
	{
		return StateComponent;
	}
};
