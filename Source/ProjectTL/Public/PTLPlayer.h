// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 플레이어 캐릭터 클래스입니다.
 *=========================================================================*/

#pragma once

#include "PTLCharacterBase.h"
#include "PTLPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTL_API APTLPlayer : public APTLCharacterBase
{
	GENERATED_BODY()
	
public:
	APTLPlayer();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Camera

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

	FORCEINLINE class UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

#pragma endregion

#pragma region TargetLock

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TargetLock, meta = (AllowPrivateAccess = "true"))
	class UPTLTargetLockComponent* TargetLockComponent;

private:
	// 타겟을 전환하기위해 필요한 마우스 움직임의 값
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetSwitchMouseValue;

	// 타겟을 전환하기 위해 필요한 아날로그 스틱 움직임의 값
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetSwitchAnalogValue;

	// 마지막 타겟 전환 후 아날로그 스틱을 중앙으로 되돌리면 true
	UPROPERTY(VisibleInstanceOnly, Category = TargetLock)
	bool bAnalogSettledSinceLastTargetSwitch;

	// 다른 타겟으로 전환하기 전의 쿨타임
	// 너무 빠르게 다음 타겟으로 넘어가는 것을 방지합니다.
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetSwitchMinDelaySeconds;

	// 마지막으로 타겟을 바꾼 시간
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float LastTargetSwitchTime;

public:
	FORCEINLINE UPTLTargetLockComponent* GetTargetLockComponent() const
	{
		return TargetLockComponent;
	}

#pragma endregion

#pragma region Movement

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

private:
	void Attack();

#pragma endregion
};
