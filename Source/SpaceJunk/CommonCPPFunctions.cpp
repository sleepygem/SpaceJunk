// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCPPFunctions.h"

#include "GameFramework/Character.h"

void UCommonCPPFunctions::SetAnimRootMotionTranslationScale(ACharacter* Character, float TranslationScale)
{
	Character->SetAnimRootMotionTranslationScale(TranslationScale);
}
