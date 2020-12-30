// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PTLCharacterBase.h"
#include "PTLPlayer.generated.h"

/*=========================================================================*
 * 플레이어 캐릭터 클래스입니다.
 *=========================================================================*/
UCLASS()
class PROJECTTL_API APTLPlayer : public APTLCharacterBase
{
	GENERATED_BODY()
	
public:
	APTLPlayer();

protected:
	virtual void BeginPlay() override;
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

#pragma region TargetLockSystem
private:
	UFUNCTION(BlueprintCallable, Category = "TargetLockSystem")
	void LockOnTarget();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetLockSystem", meta = (AllowPrivateAccess = "true"))
	class UPTLTargetLockComponent* TargetLockComponent;

private:
	// Target을 전환하기 위해 필요한 마우스 움직임의 값입니다.
	UPROPERTY(EditAnywhere, Category = "TargetLockSystem")
	float MouseValueToSwitchTarget;

	// Target을 전환하기 위해 필요한 아날로그 스틱 움직임의 값입니다.
	UPROPERTY(EditAnywhere, Category = "TargetLockSystem")
	float AnalogValueToSwitchTarget;

	// 다른 Target으로 전환하기 전의 쿨타임입니다.
	// 너무 빠르게 다음 Target을 넘어가는 것을 방지합니다.
	UPROPERTY(EditAnywhere, Category = "TargetLockSystem")
	float MinDelaySecondsToSwitchTarget;

	// 마지막 Target 전환 후 아날로그 스틱을 중앙으로 되돌리면 true로 설정됩니다.
	UPROPERTY(VisibleInstanceOnly, Category = "TargetLockSystem")
	bool bAnalogSettledSinceLastSwitchTarget;

	// 마지막으로 Target을 바꾼 시간입니다.
	UPROPERTY(VisibleInstanceOnly, Category = "TargetLockSystem")
	float LastTimeSwitchTarget;

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
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class APTLPlayerController* PlayerController;
#pragma endregion

private:
	void Attack();
};
