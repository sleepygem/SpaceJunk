// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlanetInfoFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API UPlanetInfoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UPlanetInfo* GetCurrentPlanetInfo(const UObject* WorldContextObject);
};
