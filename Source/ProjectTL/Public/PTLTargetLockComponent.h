// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTL.h"
#include "Components/ActorComponent.h"
#include "PTLTargetLockComponent.generated.h"

 /*=========================================================================*
  * 좌우를 나타낼 enum 클래스입니다.
  * BlueprintType을 인자값으로 넣으면 블루프린트에서도 사용할 수 있습니다. 
  * UENUM(BlueprintType)
 *=========================================================================*/
UENUM()
enum class EDirection : uint8
{
	Direction_Left	UMETA(DisplayName = "Left"),
	Direction_Right	UMETA(DisplayName = "Right")
};

/*=========================================================================*
 * 타겟락 기능을 가진 액터 컴포넌트입니다.
 * 일정 범위 내의 타겟을 검색하고 그 중 한 타겟을 스프링암을 통해 바라보게 합니다.
 *=========================================================================*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTL_API UPTLTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTLTargetLockComponent();

#pragma region Debug
public:
	void SetDebug();

private:
	// 디버그 설정 변수
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug;

	// 디버그 시간
	UPROPERTY(EditAnywhere, Category = "Debug")
	float DrawDebugTime;
#pragma endregion

#pragma region TargetLockSystem
public:
	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	void LockOnTarget();

	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	void SwitchLockOnTarget(EDirection NewDirection);

	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	void UnLockOnTarget();

	// Target에 대해 회전보간값을 계산합니다.
	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	FRotator RInterpToTarget();

private:
	TArray<AActor*> GetTargetableActors();

	AActor* GetNearestTargetActor(TArray<AActor*> TargetableActors);

	AActor* GetNearestTargetActorToDirection(EDirection Direction);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetLockSystem", meta = (AllowPrivateAccess = "true"))
	bool bIsLockOnTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetLockSystem", meta = (AllowPrivateAccess = "true"))
	class AActor* TargetActor;

	// Traget을 탐색할 최대 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetLockSystem", meta = (AllowPrivateAccess = "true"))
	float MaxTargetDistance;

public:
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	void SetTargetActor(AActor* NewTargetActor);

	bool GetIsLockOnTarget() const;
#pragma endregion
};
