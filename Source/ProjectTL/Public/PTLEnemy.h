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
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Target)
	class UPTLTargetComponent* TargetComponent;

public:
	FORCEINLINE class UPTLTargetComponent* GetTargetComponent() const
	{
		return TargetComponent;
	}
	
#pragma endregion

#pragma region TextRender

protected:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = TextRender)
	class UTextRenderComponent* ActorNameText;

protected:
	void PrintText();

#pragma endregion

public:
	void IsDead();
};
