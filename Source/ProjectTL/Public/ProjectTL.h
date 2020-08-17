// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"					// AddOnScreenDebugMessage()를 사용하기 위한 헤더 파일입니다.
#include "UObject/ConstructorHelpers.h"	// 콘텐츠 브라우저의 리소스나 블루프린트 클래스를 불러오기 위한 헤더 파일입니다.

#pragma region LOG

 // 로그 카테고리 정의
DECLARE_LOG_CATEGORY_EXTERN(PTL_LOG, Log, All);

/*-----------------------------------------------------------------------------------------------------------------------------------*
 * FString으로 선언된 변수에서 문자열 정보를 얻어오려면 반드시 * 연산자를 앞에 지정해줘야 합니다.
 * 액터의 이름 값을 FString으로 반환하는 GetName() 함수를 형식 문자열의 %s에 대응하는 경우,
 * 반드시 함수 앞에 * 연산자를 선언해야합니다.
 * FString::Printf(TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
 *-----------------------------------------------------------------------------------------------------------------------------------*/

 // 코드가 들어 있는 파일 이름과 함수, 라인 정보를 로그로 남기는 매크로
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

 // LOG_CALLINFO와 함께 아무 내용이 없는 로그를 남기는 매크로
/*-----------------------------------------------------------------------------------------------------------------------------------*
 * LOG_S(Error); 또는 LOG(Warning);
 *-----------------------------------------------------------------------------------------------------------------------------------*/
#define LOG_S(Verbosity) UE_LOG(PTL_LOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

 // LOG_S와 함께 문자열로 추가 정보를 로그로 남기는 매크로
/*-----------------------------------------------------------------------------------------------------------------------------------*
 * LOG(Error, "Error"); 또는 LOG(Warning, "Warning");
 *-----------------------------------------------------------------------------------------------------------------------------------*/
#define LOG(Verbosity, Format, ...) UE_LOG(PTL_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

 // Screen에 LOG를 출력하는 매크로
 /*-----------------------------------------------------------------------------------------------------------------------------------*
  * LOG_SCREEN("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
  *-----------------------------------------------------------------------------------------------------------------------------------*/
#define LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

  // LOG_SCREEN에 시간을 넣은 매크로, Tick에서 DeltaTime을 인자로 넣어서 사용
#define LOG_SCREEN_T(Time, Format, ...) GEngine->AddOnScreenDebugMessage(-1 Time, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// LOG에 Verbosity를 인자에서 뺀 매크로
#define LOG_WARNING(Format, ...) UE_LOG(PTL_LOG, Warning, TEXT("%s %s"), *LOG_CALLINFO *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(PTL_LOG, Error, TEXT("%s %s"), *LOG_CALLINFO *FString::Printf(TEXT(Format), ##__VA_ARGS__))

#pragma endregion LOG
