// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 타겟락 기능을 가진 액터 컴포넌트입니다.
 * 일정 범위 내의 타겟을 검색하고 그 중 한 타겟을 스프링암을 통해 바라보게 합니다.
 *=========================================================================*/

#pragma once

#include "ProjectTL.h"
#include "Components/ActorComponent.h"
#include "PTLTargetLockComponent.generated.h"

 /*-----------------------------------------------------------------------------------------------------------------------------------*
  * 좌우를 나타낼 enum 클래스입니다.
  * BlueprintType을 인자값으로 넣으면 블루프린트에서도 사용할 수 있습니다. 
  * UENUM(BlueprintType)
  *-----------------------------------------------------------------------------------------------------------------------------------*/
UENUM()
enum class EDirection : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTL_API UPTLTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTLTargetLockComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Target, meta = (AllowPrivateAccess = "true"))
	class AActor* TargetActor;

private:
	UPROPERTY(VisibleAnywhere, Category = Target)
	bool bLockedOnTarget;

	// 디버그 설정 변수
	UPROPERTY(VisibleAnywhere, Category = Target)
	bool bDrawDebug;

	// 디버그 시간
	UPROPERTY(VisibleAnywhere, Category = Target)
	float DebugLifeTime;

	// Traget을 탐색할 최대 거리
	UPROPERTY(VisibleAnywhere, Category = Target)
	float MaxTargetDistance;

public:
	// Target을 조준합니다.
	void LockOnTarget();

	// 변경된 Target을 조준합니다.
	void LockOnSwitchTarget(EDirection Direction);

	// Target을 해제합니다.
	void UnlockTarget();

private:
	// 조준 가능한 Target의 배열 생성합니다.
	TArray<AActor*> GetTargetableActors();

	// Target을 설정합니다.
	AActor* SetTarget(TArray<AActor*> TargetableActors);

	// Target을 변경합니다.
	AActor* SwitchTarget(EDirection Direction);

public:
	// Target에 대해 회전보간값을 계산합니다.
	FRotator RInterpToTarget();

public:
	// 디버그를 설정합니다.
	void SetDebug();

	// Get함수 FORCEINLINE 매크로로 inline함수로 작성
public:
	UFUNCTION(BlueprintCallable, Category = "Target")			// 임시로 사용하는 블루프린트 함수 선언
	FORCEINLINE AActor* GetTargetActor() const
	{
		return TargetActor;
	}

	FORCEINLINE bool GetLockOnTargetFlag() const
	{
		return bLockedOnTarget;
	}
};
