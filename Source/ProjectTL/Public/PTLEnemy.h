// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 적 클래스입니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "PTLCharacterBase.h"
#include "PTLEnemy.generated.h"

UCLASS()
class PROJECTTL_API APTLEnemy : public APTLCharacterBase
{
	GENERATED_BODY()
	
public:
	APTLEnemy();

protected:
	virtual void Tick(float DeltaTime) override;

#pragma region Target

protected:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Target, meta = (AllowPrivateAccess = "true"))
	class UPTLTargetComponent* TargetComponent;

	// Get함수 FORCEINLINE 매크로로 Inline함수로 작성
public:
	// TargetComponent Get함수
	UFUNCTION(BlueprintCallable, Category = "Target")			// 임시로 사용하는 블루프린트 함수 선언
	FORCEINLINE class UPTLTargetComponent* GetTargetComponent() const
	{
		return TargetComponent;
	}
	
#pragma endregion

#pragma region TextRender

protected:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = TextRender, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ActorNameText;

protected:
	void PrintText();

#pragma endregion

public:
	void IsDead();
};
