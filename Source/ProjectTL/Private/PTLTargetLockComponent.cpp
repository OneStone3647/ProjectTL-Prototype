// Fill out your copyright notice in the Description page of Project Settings.

#include "PTLTargetLockComponent.h"
#include "PTLPlayer.h"
#include "PTLPlayerController.h"
#include "PTLEnemy.h"
#include "PTLStateComponent.h"
#include "PTLTargetComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UPTLTargetLockComponent::UPTLTargetLockComponent()
{
	// TickComponent를 사용하지 않습니다.
	PrimaryComponentTick.bCanEverTick = false;

	bDrawDebug = false;
	DrawDebugTime = 2.0f;

	bIsLockOnTarget = false;
	TargetActor = nullptr;
	MaxTargetDistance = 1000.0f;
}

void UPTLTargetLockComponent::SetDebug()
{	
	if (bDrawDebug)
	{
		bDrawDebug = false;
		PTL_LOG_SCREEN("DrawDebug Disabled!!");
	}
	else
	{
		bDrawDebug = true;
		PTL_LOG_SCREEN("DrawDebug Enabled!!");
	}
}

void UPTLTargetLockComponent::LockOnTarget()
{
	// Traget을 LockOn하고 있을 경우 LockOn을 해제합니다.
	if (TargetActor && bIsLockOnTarget)
	{
		UnLockOnTarget();
	}
	else
	{
		TArray<AActor*> TargetableActors = GetTargetableActors();
		AActor* NewTargetActor = GetNearestTargetActor(TargetableActors);

		SetTargetActor(NewTargetActor);
	}
}

void UPTLTargetLockComponent::SwitchLockOnTarget(EDirection NewDirection)
{
	AActor* NewTargetActor = GetNearestTargetActorToDirection(NewDirection);

	SetTargetActor(NewTargetActor);
}

void UPTLTargetLockComponent::UnLockOnTarget()
{
	TargetActor = nullptr;
	bIsLockOnTarget = false;

	APTLCharacterBase* Player = Cast<APTLCharacterBase>(GetOwner());
	if (Player)
	{
		Player->GetCharacterMovement()->bOrientRotationToMovement = true;
		Player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

FRotator UPTLTargetLockComponent::RInterpToTarget()
{
	FRotator RInterpToRotator;
	float InterpSpeed = 5.0f;

	APTLPlayerController* PlayerController = Cast<APTLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		FRotator PlayerControllerRotator = PlayerController->GetControlRotation();
		FVector PlayerLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = TargetActor->GetActorLocation();

		// 2개의 위치 벡터를 입력하여 2번째 위치 벡터를 바라보는 방향정보(Rotator)를 반환합니다.
		FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

		RInterpToRotator = FMath::RInterpTo(PlayerControllerRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	}

	return RInterpToRotator;
}

TArray<AActor*> UPTLTargetLockComponent::GetTargetableActors()
{
	FVector PlayerLocation = GetOwner()->GetActorLocation();

	// 탐지 가능한 범위를 드로우 디버그합니다.
	if (bDrawDebug)
	{
		float DebugSize = 50.0f;

		DrawDebugSphere(GetWorld(), PlayerLocation, MaxTargetDistance, DebugSize, FColor::Green, false, DrawDebugTime);
	}

	TArray<FHitResult> HitResults;
	TArray<AActor*> TargetableActors;
	// ECC_GameTraceChannel1 = Target Trace Channel
	bool bIsHit = GetWorld()->SweepMultiByChannel(HitResults, PlayerLocation, PlayerLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(MaxTargetDistance));
	if (bIsHit)
	{
		for (auto& Hit : HitResults)
		{
			APTLEnemy* HitedEnemy = Cast<APTLEnemy>(Hit.Actor);
			if (HitedEnemy && HitedEnemy->GetStateComponent()->GetIsDead() != true)
			{
				if (bDrawDebug)
				{
					float DebugSize = 30.0f;

					DrawDebugPoint(GetWorld(), HitedEnemy->GetTargetComponent()->GetComponentLocation(), DebugSize, FColor::Green, false, DrawDebugTime);
				}

				// 중복되지 않게 TargetalbeActors 배열에 넣습니다.
				TargetableActors.AddUnique(Cast<AActor>(Hit.Actor));
			}
		}
	}

	return TargetableActors;
}

AActor * UPTLTargetLockComponent::GetNearestTargetActor(TArray<AActor*> TargetableActors)
{
	// 조준 가능한 Target이 없을 경우 nullptr을 반환합니다.
	if (TargetableActors.Num() == 0)
	{
		return nullptr;
	}

	float NearestDot = 0.0f;
	AActor* NearestTargetActor = nullptr;
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

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

	if (bDrawDebug)
	{
		PTL_LOG_SCREEN("TargetActor: %s", *NearestTargetActor->GetName());
	}

	return NearestTargetActor;
}

AActor * UPTLTargetLockComponent::GetNearestTargetActorToDirection(EDirection Direction)
{
	AActor* NearestTargetActor = nullptr;

	if (bIsLockOnTarget)
	{
		TArray<AActor*> TargetableActors = GetTargetableActors();
		int32 CurrentTargetIndex = TargetableActors.Find(TargetActor);
		// 현재 Target의 Index를 발견하였다면 배열에서 제거합니다.
		if (CurrentTargetIndex != INDEX_NONE)
		{
			TargetableActors.Remove(TargetActor);
		}

		// 현재 타겟을 기준으로 좌우에 있는 타겟을 탐색합니다.
		TArray<AActor*> LeftTargetableActors;
		TArray<AActor*> RightTargetableActors;
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		for (AActor* TargetableActor : TargetableActors)
		{
			/*-----------------------------------------------------------------------------------------------------------------------------------*
			 * 벡터의 뺄셈을 활용하여 내가 타겟을 바라보는 방향을 구합니다.
			 * 내가 타겟을 바라보는 방향 = 타겟의 위치 - 자신의 위치
			 *-----------------------------------------------------------------------------------------------------------------------------------*/

			FVector LookAtTargetableActorVector = TargetableActor->GetActorLocation() - PlayerCameraManager->GetCameraLocation();
			FVector LookAtCurrentTargetActorVector = TargetActor->GetActorLocation() - PlayerCameraManager->GetCameraLocation();

			// 두 벡터의 외적을 반환합니다.
			FVector CrossVector = FVector::CrossProduct(LookAtCurrentTargetActorVector, LookAtTargetableActorVector);
			// 정규화한 벡터의 Z축으로 좌우에 존재하는지 판별합니다.
			if (CrossVector.GetSafeNormal().Z < 0.0f)
			{
				// 내가 바라보는 방향을 기준으로 왼쪽에 존재하고 LeftTargetableActors 배열에 존재하지 않을 경우 배열에 추가합니다.
				LeftTargetableActors.AddUnique(TargetableActor);
			}
			else
			{
				// 내가 바라보는 방향을 기준으로 오른쪽에 존재하고 RightTargetableActors 배열에 존재하지 않을 경우 배열에 추가합니다.
				RightTargetableActors.AddUnique(TargetableActor);
			}
		}

		switch (Direction)
		{
		case EDirection::Direction_Left:
			if (LeftTargetableActors.Num() > 0)
			{
				NearestTargetActor = GetNearestTargetActor(LeftTargetableActors);
			}
			else
			{
				NearestTargetActor = GetNearestTargetActor(RightTargetableActors);
			}
			break;
		case EDirection::Direction_Right:
			if (RightTargetableActors.Num() > 0)
			{
				NearestTargetActor = GetNearestTargetActor(RightTargetableActors);
			}
			else
			{
				NearestTargetActor = GetNearestTargetActor(LeftTargetableActors);
			}
			break;
		default:
			break;
		}
	}

	return NearestTargetActor;
}

AActor * UPTLTargetLockComponent::GetTargetActor() const
{
	return TargetActor;
}

void UPTLTargetLockComponent::SetTargetActor(AActor * NewTargetActor)
{
	if (NewTargetActor)
	{
		TargetActor = NewTargetActor;
		bIsLockOnTarget = true;

		APTLCharacterBase* Player = Cast<APTLCharacterBase>(GetOwner());
		if (Player)
		{
			Player->GetCharacterMovement()->bOrientRotationToMovement = false;
			Player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}

		if (bDrawDebug)
		{
			APTLEnemy* TargetEnemy = Cast<APTLEnemy>(TargetActor);
			float DebugSize = 30.0f;

			DrawDebugPoint(GetWorld(), TargetEnemy->GetTargetComponent()->GetComponentLocation(), DebugSize, FColor::Red, false, DrawDebugTime);
		}
	}
}

bool UPTLTargetLockComponent::GetIsLockOnTarget() const
{
	return bIsLockOnTarget;
}

