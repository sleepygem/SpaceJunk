// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "PlanetInfo.h"
#include "APlanetWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API AAPlanetWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPlanetInfo* PlanetInfo;
};
