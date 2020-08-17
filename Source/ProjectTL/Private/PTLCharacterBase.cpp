// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 기본 베이스 클래스입니다.
 *=========================================================================*/

#include "PTLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PTLCharacterAnimInstance.h"
#include "..\Public\PTLCharacterBase.h"

APTLCharacterBase::APTLCharacterBase()
{
	// Tick을 사용하지 않기 때문에 false로 설정합니다.
	PrimaryActorTick.bCanEverTick = false;

	// 메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);		// 애니메이션 블루프린트를 사용하여 애니메이션을 재생합니다.

	// 애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> PTLCharacter_Anim(TEXT("AnimBlueprint'/Game/Blueprints/AnimBP_PTLCharacterAnim.AnimBP_PTLCharacterAnim_C'"));
	if (PTLCharacter_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PTLCharacter_Anim.Class);
	}

	// 캡슐 콜리전 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);					// 캡슐의 반경과 캡슐의 절반 높이를 설정합니다.

	// CharacterMovement 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;		// 움직이는 방향으로 캐릭터를 회전합니다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);	// 캐릭터의 회전 속도입니다.
	GetCharacterMovement()->JumpZVelocity = 600.0f;						// 점프할 때 초기 속도입니다.
	GetCharacterMovement()->AirControl = 0.2f;								// 떨어질 때 캐릭터에서 사용 가능한 측면 이동 제어의 양입니다
}

void APTLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
