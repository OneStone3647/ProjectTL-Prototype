// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 기본 베이스 클래스입니다.
 *=========================================================================*/

#include "PTLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APTLCharacterBase::APTLCharacterBase()
{
	// Tick 함수를 사용하지 않습니다.
	PrimaryActorTick.bCanEverTick = false;

	// SkeletalMesh 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// AnimInstance 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> Character_Anim(TEXT("/Game/Blueprints/AnimBP_PTLCharacterAnim"));
	if (Character_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Character_Anim.Class);
	}

	// Collision 설정
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// CharacterMovement 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;		// 움직이는 방향으로 캐릭터를 회전합니다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);	// 캐릭터의 회전 속도입니다.
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// StateComponent 생성 및 설정
	StateComponent = CreateDefaultSubobject<UPTLStateComponent>(TEXT("StateComponent"));
	StateComponent->SetIsDead(false);
}
