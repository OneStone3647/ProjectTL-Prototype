// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 플레이어 캐릭터 클래스입니다.
 *=========================================================================*/

#include "PTLPlayer.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APTLPlayer::APTLPlayer()
{
	// Tick을 사용하지 않기 때문에 true로 설정합니다.
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 스프링암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 15.0f);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;			// 컨프롤러를 기준으로 회전합니다.
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;

	// 카메라 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;		// 스프링 암 기준으로 회전하지 않습니다.
}

void APTLPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APTLPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APTLPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APTLPlayer::MoveRight);

	// 마우스 입력
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// 게임페드 입력
	PlayerInputComponent->BindAxis("TurnRate", this, &APTLPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APTLPlayer::LookUpAtRate);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APTLPlayer::Attack);

	//PlayerInputComponent->BindAction("SetDebug", IE_Pressed, CameraLockArm, &UTLSpringArmComponent::SetDebug);
	//PlayerInputComponent->BindAction("TargetLock", IE_Pressed, CameraLockArm, &UTLSpringArmComponent::LockToTarget);
	//PlayerInputComponent->BindAction<TBaseDelegate<void, EDirection>>("TargetSwitchLeft", IE_Pressed, CameraLockArm, &UTLSpringArmComponent::LockToSwitchTarget, EDirection::Left);
	//PlayerInputComponent->BindAction<TBaseDelegate<void, EDirection>>("TargetSwitchRight", IE_Pressed, CameraLockArm, &UTLSpringArmComponent::LockToSwitchTarget, EDirection::Right);
}

void APTLPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APTLPlayer::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APTLPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APTLPlayer::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APTLPlayer::Attack()
{
}
