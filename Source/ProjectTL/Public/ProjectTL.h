// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"					
#include "UObject/ConstructorHelpers.h"	

// 로그 카테고리 정의
DECLARE_LOG_CATEGORY_EXTERN(ProjectTL_Log, Log, All);

/*-----------------------------------------------------------------------------------------------------------------------------------*
 * FString으로 선언된 변수에서 문자열 정보를 얻어오려면 반드시 * 연산자를 앞에 지정해줘야 합니다.
 * 액터의 이름 값을 FString으로 반환하는 GetName() 함수를 형식 문자열의 %s에 대응하는 경우,
 * 반드시 함수 앞에 * 연산자를 선언해야합니다.
 * FString::Printf(TEXT("Actor Name : %s, ID : %d, Location X : %.3f"),
 *							*GetName(), ID, GetActorLocation().X);
 *-----------------------------------------------------------------------------------------------------------------------------------*/

// 코드가 들어 있는 파일 이름과 함수, 라인 정보를 로그로 남기는 매크로
#define PTL_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// PTL_LOG_CALLINFO와 함께 아무 내용이 없는 로그를 남기는 매크로
/*-----------------------------------------------------------------------------------------------------------------------------------*
 * PTL_LOG_S(Error);
 * PTL_LOG(Warning);
 *-----------------------------------------------------------------------------------------------------------------------------------*/
#define PTL_LOG_S(Verbosity) UE_LOG(ProjectTL_Log, Verbosity, TEXT("%s"), *PTL_LOG_CALLINFO)

// PTL_LOG_S와 함께 문자열로 추가 정보를 로그로 남기는 매크로
/*-----------------------------------------------------------------------------------------------------------------------------------*
 * PTL_LOG(Error, "Error"); 
 * PTL_LOG(Warning, "Warning");
 *-----------------------------------------------------------------------------------------------------------------------------------*/
#define PTL_LOG(Verbosity, Format, ...) UE_LOG(ProjectTL_Log, Verbosity, TEXT("%s %s"), *PTL_LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Screen에 LOG를 밀면서 출력하는 매크로
/*-----------------------------------------------------------------------------------------------------------------------------------*
 * PTL_LOCE_SCREEN("Actor Name : %s, ID : %d, Location X : %.3f"),
 *							GetName(), ID, GetActorLocation().X);
 *-----------------------------------------------------------------------------------------------------------------------------------*/
#define PTL_LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Screen에 LOG를 Index 순서대로 고정하여 갱신하는 매크로
#define PTL_LOG_SCREEN_INFO(Index, Format, ...) GEngine->AddOnScreenDebugMessage(Index, 0.0f, FColor::Cyan, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// PTL_LOG_SCREEN에 시간을 넣은 매크로, Tick에서 DeltaTime을 인자로 넣어서 사용합니다.
#define PTL_LOG_SCREEN_T(Time, Format, ...) GEngine->AddOnScreenDebugMessage(-1 Time, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// PTL_LOG에 Verbosity를 인자에서 뺀 매크로
#define PTL_LOG_WARNING(Format, ...) UE_LOG(ProjectTL_Log, Warning, TEXT("%s %s"), *PTL_LOG_CALLINFO *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define PTL_LOG_ERROR(Format, ...) UE_LOG(ProjectTL_Log, Error, TEXT("%s %s"), *PTL_LOG_CALLINFO *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// 런타임에서 문제가 발생할 때 붉은색으로 에러 로그를 뿌리고 바로 함수를 반환합니다.
#define PTL_CHECK(Expr, ...) {if(!(Expr)) {PTL_LOG(Error, TEXT("ASSERTION: %s")), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

