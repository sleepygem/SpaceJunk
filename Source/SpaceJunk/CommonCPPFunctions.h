﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonCPPFunctions.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API UCommonCPPFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static void SetAnimRootMotionTranslationScale(ACharacter* Character, float TranslationScale);
};