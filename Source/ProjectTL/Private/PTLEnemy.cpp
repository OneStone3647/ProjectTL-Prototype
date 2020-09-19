// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 적 클래스입니다.
 *=========================================================================*/

#include "PTLEnemy.h"
#include "PTLTargetComponent.h"
#include "PTLStateComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

APTLEnemy::APTLEnemy()
{
	// Tick함수를 사용하기 때문에 true로 설정합니다.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	// TargetComponent 설정
	TargetComponent = CreateDefaultSubobject<UPTLTargetComponent>(TEXT("TargetComponent"));
	TargetComponent->SetupAttachment(RootComponent);

	// TextRender 설정
	ActorNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ActorNameText"));
	ActorNameText->SetupAttachment(RootComponent);
	ActorNameText->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 90.0f), FRotator(0.0f, 0.0f, 0.0f));
	ActorNameText->SetWorldSize(30.0f);
	ActorNameText->SetTextRenderColor(FColor::Blue);
	ActorNameText->SetHorizontalAlignment(EHTA_Center);			// 텍스트 수평 정렬
	ActorNameText->SetVerticalAlignment(EVRTA_TextCenter);		// 텍스트 세로 정렬
	ActorNameText->SetText(TEXT("Enemy"));
}

void APTLEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintText();
}

// TextRender를 플레이어 카메라를 바라보도록 출력합니다.
void APTLEnemy::PrintText()
{
	APlayerCameraManager* PlayerCameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (PlayerCameraManager)
	{
		FRotator NewRotation = PlayerCameraManager->GetCameraRotation();

		ActorNameText->SetRelativeRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	}
}

void APTLEnemy::IsDead()
{
	if (StateComponent->GetIsDead() != true)
	{
		// 레그돌 설정
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetSimulatePhysics(true);

		// CapsuleComponent를 삭제합니다.
		GetCapsuleComponent()->DestroyComponent();

		StateComponent->SetIsDead(true);
	}
}
