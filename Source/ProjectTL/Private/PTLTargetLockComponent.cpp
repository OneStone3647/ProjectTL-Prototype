// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 타겟락 기능을 가진 액터 컴포넌트입니다.
 * 일정 범위 내의 타겟을 검색하고 그 중 한 타겟을 스프링암을 통해 바라보게 합니다.
 *=========================================================================*/

#include "PTLTargetLockComponent.h"
#include "PTLStateComponent.h"
#include "PTLTargetComponent.h"
#include "PTLEnemy.h"
#include "PTLPlayer.h"
#include "PTLPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"		// FindLookAtRotation() 함수를 사용하기 위해 필요한 헤더 파일
#include "Kismet/GameplayStatics.h"			// GetPlayerCameraManager() 함수를 사용하기 위해 필요한 헤더 파일


UPTLTargetLockComponent::UPTLTargetLockComponent()
{
	// TickComponent함수를 사용하하지 않기 때문에 false로 설정합니다.
	PrimaryComponentTick.bCanEverTick = false;

	TargetActor = nullptr;
	bLockedOnTarget = false;

	bDrawDebug = false;
	DebugLifeTime = 2.0f;
	MaxTargetDistance = 1000.0f;
}

// Target을 조준합니다.
void UPTLTargetLockComponent::LockOnTarget()
{
	if (TargetActor && bLockedOnTarget)
	{
		// Target이 있을 경우 TargetActor를 비웁니다.
		UnlockTarget();
	}
	else
	{
		AActor* NewTargetActor = SetTarget(GetTargetableActors());
		if (NewTargetActor)
		{
			TargetActor = NewTargetActor;
			bLockedOnTarget = true;

			if (bDrawDebug)
			{
				APTLEnemy* TargetEnemy = Cast<APTLEnemy>(TargetActor);
				DrawDebugPoint(GetWorld(), TargetEnemy->GetTargetComponent()->GetComponentLocation(), 30.0f, FColor::Red, false, DebugLifeTime);
			}
		}
	}
}

// 변경된 Target을 조준합니다.
void UPTLTargetLockComponent::LockOnSwitchTarget(EDirection Direction)
{
	AActor* NewTargetActor = SwitchTarget(Direction);
	if (NewTargetActor)
	{
		TargetActor = NewTargetActor;
		bLockedOnTarget = true;
		APTLPlayer* Player = Cast<APTLPlayer>(GetOwner());
		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		Player->GetCharacterMovement()->bUseControllerDesiredRotation = true;

		if (bDrawDebug)
		{
			APTLEnemy* TargetEnemy = Cast<APTLEnemy>(TargetActor);
			DrawDebugPoint(GetWorld(), TargetEnemy->GetTargetComponent()->GetComponentLocation(), 30.0f, FColor::Red, false, DebugLifeTime);
		}
	}
}

// Target을 해제합니다.
void UPTLTargetLockComponent::UnlockTarget()
{
	TargetActor = nullptr;
	bLockedOnTarget = false;
	APTLPlayer* Player = Cast<APTLPlayer>(GetOwner());
	Player->GetCharacterMovement()->bOrientRotationToMovement = true;
	Player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

// 조준 가능한 Target의 배열 생성합니다.
TArray<AActor*> UPTLTargetLockComponent::GetTargetableActors()
{
	TArray<FHitResult> HitResult;
	FVector PlayerLocation = GetOwner()->GetActorLocation();

	// 타겟범위를 드로우 디버그합니다.
	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), PlayerLocation, MaxTargetDistance, 50.0f, FColor::Green, false, DebugLifeTime);
	}

	// ECollisionChannel은 DefaultEngine.ini파일에서 확인 가능합니다.
	// ECollisionChannel::ECC_GameTraceChannel1: Target Trace Channel입니다.
	bool bIsHit = GetWorld()->SweepMultiByChannel(HitResult, PlayerLocation, PlayerLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(MaxTargetDistance));

	TArray<AActor*> TargetLockableActors;
	if (bIsHit)
	{
		for (auto& Hit : HitResult)
		{
			APTLEnemy* HitedActor = Cast<APTLEnemy>(Hit.Actor);
			if (Hit.Actor.IsValid() && !HitedActor->GetStateComponent()->GetIsDead())
			{
				if (bDrawDebug)
				{
					DrawDebugPoint(GetWorld(), HitedActor->GetTargetComponent()->GetComponentLocation(), 30.0f, FColor::Green, false, DebugLifeTime);
					LOG_SCREEN("Hit Actor: %s", *HitedActor->GetName());
				}

				// 중복되지 않게 TargetLockableActors 배열에 넣습니다.
				TargetLockableActors.AddUnique(Cast<AActor>(Hit.Actor));
			}
		}
	}
	else
	{
		LOG_SCREEN("Hit Failed!!");
	}

	return TargetLockableActors;
}

// Target을 설정합니다.
AActor * UPTLTargetLockComponent::SetTarget(TArray<AActor*> TargetableActors)
{
	// 조준 가능한 Target이 없을 경우 nullptr을 반환합니다.
	if (TargetableActors.Num() == 0)
	{
		return nullptr;
	}

	float NearestDot = 0.0f;
	AActor* NearestTargetActor = nullptr;
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	for (int32 i = 0; i < TargetableActors.Num(); ++i)
	{
		/*-----------------------------------------------------------------------------------------------------------------------------------*
		 * float GetDotProductTo
		 *	(
		 *		const AActor * OtherActor
		 *	) const
		 * OtherActor의 내적 값을 반환합니다.
		 * 두 벡터의 내적의 각에 따라
		 * 평행할 경우(0)일 경우 1, 서로 직각일 경우(90) 0, 평행이면서 반대 방향일 경우(180) -1를 반환합니다.
		 * 실패하면 -2.0을 반환합니다.
		 *-----------------------------------------------------------------------------------------------------------------------------------*/
		float Dot = PlayerCameraManager->GetDotProductTo(TargetableActors[i]);
		if (i == 0)
		{
			NearestDot = Dot;
			NearestTargetActor = TargetableActors[i];
		}
		else
		{
			// 가장 가까운 Actor를 찾습니다.
			if (Dot > NearestDot)
			{
				NearestDot = Dot;
				NearestTargetActor = TargetableActors[i];
			}
		}
	}

	LOG_SCREEN("TargetActor: %s", *NearestTargetActor->GetName());

	return NearestTargetActor;
}

// Target을 변경합니다.
AActor * UPTLTargetLockComponent::SwitchTarget(EDirection Direction)
{
	AActor* NewTarget = nullptr;

	if (bLockedOnTarget)
	{
		TArray<AActor*> TargetableActors = GetTargetableActors();
		int32 CurrentTargetIndex = TargetableActors.Find(TargetActor);
		// 현재 타겟의 Index를 발견하였다면 배열에서 제거합니다.
		if (CurrentTargetIndex != INDEX_NONE)
		{
			TargetableActors.Remove(TargetActor);
		}

		// 현재 타겟을 기준으로 좌우에 있는 타겟을 탐색합니다.
		TArray<AActor*> LeftTargetableActors;
		TArray<AActor*> RightTargetableActors;
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		for (int32 i = 0; i < TargetableActors.Num(); ++i)
		{
			/*-----------------------------------------------------------------------------------------------------------------------------------*
			 * 벡터의 뺄셈을 활용하여 내가 타겟을 바라보는 방향을 구합니다.
			 * 내가 타겟을 바라보는 방향 = 타겟의 위치 - 자신의 위치
			 *-----------------------------------------------------------------------------------------------------------------------------------*/
			FVector TargetableActorVector = TargetableActors[i]->GetActorLocation() - PlayerCameraManager->GetCameraLocation();
			FVector CurrentTargetActorVector = TargetActor->GetActorLocation() - PlayerCameraManager->GetCameraLocation();

			// 두 벡터의 외적을 반환합니다.
			FVector CrossVector = FVector::CrossProduct(CurrentTargetActorVector, TargetableActorVector);
			// 정규화한 벡터의 Z축으로 좌우에 존재하는지 판별합니다.
			if (CrossVector.GetSafeNormal().Z < 0.0f)
			{
				// LeftTargetableActors 배열에 존재하지 않으면 추가합니다.
				LeftTargetableActors.AddUnique(TargetableActors[i]);
			}
			else
			{
				// RightTargetableActors 배열에 존재하지 않으면 추가합니다.
				RightTargetableActors.AddUnique(TargetableActors[i]);
			}
		}

		switch (Direction)
		{
		case EDirection::Left:
			if (LeftTargetableActors.Num() > 0)
			{
				NewTarget = SetTarget(LeftTargetableActors);
			}
			else
			{
				NewTarget = SetTarget(RightTargetableActors);
			}
			break;
		case EDirection::Right:
			if (RightTargetableActors.Num() > 0)
			{
				NewTarget = SetTarget(RightTargetableActors);
			}
			else
			{
				NewTarget = SetTarget(LeftTargetableActors);
			}
			break;
		}

	}

	return NewTarget;
}

// Target에 대해 회전보간값을 계산합니다.
FRotator UPTLTargetLockComponent::RInterpToTarget()
{
	//APTLPlayerController* PlayerController = Cast<APTLPlayerController>(GetWorld()->GetFirstPlayerController());
	APTLPlayerController* PlayerController = Cast<APTLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FRotator PlayerControllerRotator = PlayerController->GetControlRotation();
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FVector TargetActorLocation = TargetActor->GetActorLocation();

	// 2개의 위치 벡터를 입력하여 2번째 위치 벡터를 바라보는 방향정보(Rotator)를 리턴합니다.
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetActorLocation);

	FRotator RInterpToRotator = FMath::RInterpTo(PlayerControllerRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), 5.0f);

	return RInterpToRotator;
}

// 디버그를 설정합니다.
void UPTLTargetLockComponent::SetDebug()
{	
	if (bDrawDebug)
	{
		bDrawDebug = false;
		LOG_SCREEN("DrawDebug Disabled!!");
	}
	else
	{
		bDrawDebug = true;
		LOG_SCREEN("DrawDebug Enabled!!");
	}
}
