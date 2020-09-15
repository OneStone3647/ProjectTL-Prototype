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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/* Tolerance for a mouse movement to be considered an input to switch targets */
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetSwitchMouseDelta;

	/* Tolerance for a mouse movement to be considered an input to switch targets */
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetSwitchAnalogValue;

	/* True if player returned analog-stick to center after last target switch */
	bool bAnalogSettledSinceLastTargetSwitch;

	/* Cooldown time before another target switch can occur, used to make target switching more controllable */
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetSwitchMinDelaySeconds;

	// 마지막으로 타겟을 바꾼 시간
	UPROPERTY(BlueprintReadOnly, Category = Camera)
	float LastTargetSwitchTime;

	/* Tolerance for a mouse movement to be considered an input to break target lock */
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float BreakLockMouseDelta;

	/* Time to wait after breaking lock with mouse movement before player gets control of the camera back.  Prevents over scrolling camera after breaking lock. */
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float BrokeLockAimingCooldown;

	/* Time that player broke camera lock at */
	float BrokeLockTime;

	// Get함수 FORCEINLINE 매크로로 inline함수로 작성
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

#pragma region Target

private:
	UPROPERTY(VisibleAnywhere, Category = Target)
	class UPTLTargetLockComponent* TargetLockComponent;

	// Get함수 FORCEINLINE 매크로로 inline함수로 작성
public:
	UFUNCTION(BlueprintCallable, Category = "Target")			// 임시로 사용하는 블루프린트 함수 선언
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

#pragma endregion

private:
	void Attack();
};
