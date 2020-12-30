// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * 플레이어 캐릭터 클래스입니다.
 *=========================================================================*/

#include "PTLPlayer.h"
#include "PTLEnemy.h"
#include "PTLStateComponent.h"
#include "PTLPlayerController.h"
#include "PTLTargetLockComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

APTLPlayer::APTLPlayer()
{
	// Tick함수를 사용하기 때문에 true로 설정합니다.
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
	CameraBoom->bUsePawnControlRotation = true;													// 컨프롤러를 기준으로 회전합니다.
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;

	// 카메라 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;												// 스프링 암 기준으로 회전하지 않습니다.

	// TargetLockComponent 설정
	TargetLockComponent = CreateDefaultSubobject<UPTLTargetLockComponent>(TEXT("TargetLockComponent"));
	MouseValueToSwitchTarget = 3.0f;
	AnalogValueToSwitchTarget = 0.2f;
	MinDelaySecondsToSwitchTarget = 0.5f;
}

void APTLPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APTLPlayerController>(GetController());
}

void APTLPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LockOnTarget();
}

void APTLPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &APTLPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APTLPlayer::MoveRight);
	
	// 마우스 입력
	PlayerInputComponent->BindAxis("Turn", this, &APTLPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APTLPlayer::LookUp);
	// 게임페드 입력
	PlayerInputComponent->BindAxis("TurnRate", this, &APTLPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APTLPlayer::LookUpAtRate);
	
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APTLPlayer::Attack);
	
	PlayerInputComponent->BindAction("TriggerDebug", IE_Pressed, TargetLockComponent, &UPTLTargetLockComponent::SetDebug);
	PlayerInputComponent->BindAction("TargetLock", IE_Pressed, TargetLockComponent, &UPTLTargetLockComponent::LockOnTarget);
	//PlayerInputComponent->BindAction<TBaseDelegate<void, EDirection>>("TargetSwitchLeft", IE_Pressed, TargetLockComponent, &UPTLTargetLockComponent::LockOnSwitchTarget, EDirection::Left);
	//PlayerInputComponent->BindAction<TBaseDelegate<void, EDirection>>("TargetSwitchRight", IE_Pressed, TargetLockComponent, &UPTLTargetLockComponent::LockOnSwitchTarget, EDirection::Right);
}

void APTLPlayer::LockOnTarget()
{
	if (TargetLockComponent->GetIsLockOnTarget() == true)
	{
		APTLEnemy* TargetEnemy = Cast<APTLEnemy>(TargetLockComponent->GetTargetActor());
		if (TargetEnemy->GetStateComponent()->GetIsDead() != true)
		{
			// APTLPlayerController* PlayerController = Cast<APTLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerController)
			{
				PlayerController->SetControlRotation(TargetLockComponent->RInterpToTarget());
			}
		}
		else
		{
			TargetLockComponent->UnLockOnTarget();
			TargetLockComponent->LockOnTarget();
		}
	}
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

void APTLPlayer::Turn(float Value)
{
	if (TargetLockComponent->GetIsLockOnTarget() == true)
	{
		// 마지막으로 Target을 바꾼 이후의 시간
		float SinceTimeLastSwitchTarget = GetWorld()->GetRealTimeSeconds() - LastTimeSwitchTarget;

		if (FMath::Abs(Value) > MouseValueToSwitchTarget && SinceTimeLastSwitchTarget > MinDelaySecondsToSwitchTarget)
		{
			if (Value < 0)
			{
				TargetLockComponent->SwitchLockOnTarget(EDirection::Direction_Left);
			}
			else
			{
				TargetLockComponent->SwitchLockOnTarget(EDirection::Direction_Right);
			}

			LastTimeSwitchTarget = GetWorld()->GetRealTimeSeconds();
		}
	}
	else
	{
		APawn::AddControllerYawInput(Value);
	}
}

void APTLPlayer::LookUp(float Value)
{
	if (TargetLockComponent->GetIsLockOnTarget() != true)
	{
		APawn::AddControllerPitchInput(Value);
	}
}

void APTLPlayer::TurnAtRate(float Rate)
{
	// 마지막 Target 전환 후 아날로그 스틱이 중립으로 돌아 왔는지 확인합니다.
	if (FMath::Abs(Rate) < 0.1f)
	{
		bAnalogSettledSinceLastSwitchTarget = true;
	}

	if (TargetLockComponent->GetIsLockOnTarget() == true)
	{
		if (FMath::Abs(Rate) > AnalogValueToSwitchTarget && bAnalogSettledSinceLastSwitchTarget)
		{
			if (Rate < 0)
			{
				TargetLockComponent->SwitchLockOnTarget(EDirection::Direction_Left);
			}
			else
			{
				TargetLockComponent->SwitchLockOnTarget(EDirection::Direction_Right);
			}

			bAnalogSettledSinceLastSwitchTarget = false;
		}
	}
	else
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APTLPlayer::LookUpAtRate(float Rate)
{
	if (TargetLockComponent->GetIsLockOnTarget() != true)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void APTLPlayer::Attack()
{
	AActor* TargetActor = TargetLockComponent->GetTargetActor();
	if (IsValid(TargetActor))
	{
		APTLEnemy* TargetEnemy = Cast<APTLEnemy>(TargetActor);
		if (TargetEnemy)
		{
			TargetEnemy->IsDead();
		}
	}
}
