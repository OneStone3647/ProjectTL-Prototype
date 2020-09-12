// Fill out your copyright notice in the Description page of Project Settings.

/*=========================================================================*
 * NPC 클래스입니다.
 *=========================================================================*/

#include "PTLNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"

APTLNPC::APTLNPC()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	ActorNameText->SetText(TEXT("NPC"));
}
