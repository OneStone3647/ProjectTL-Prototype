// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 캐릭터의 기본 베이스 클래스입니다.
 *=========================================================================*/

#include "PTLCharacterBase.h"
#include "PTLStatecomponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APTLCharacterBase::APTLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Character_Anim(TEXT("AnimBlueprint'/Game/Blueprints/AnimBP_PTLCharacterAnim.AnimBP_PTLCharacterAnim_C'"));
	if (Character_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Character_Anim.Class);
	}

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;		// 움직이는 방향으로 캐릭터를 회전합니다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);	// 캐릭터의 회전 속도입니다.
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	StateComponent = CreateDefaultSubobject<UPTLStateComponent>(TEXT("StateComponent"));
	StateComponent->SetIsDead(false);
}
