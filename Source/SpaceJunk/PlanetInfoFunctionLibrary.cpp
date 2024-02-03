// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetInfoFunctionLibrary.h"

#include "APlanetWorldSettings.h"

UPlanetInfo* UPlanetInfoFunctionLibrary::GetCurrentPlanetInfo(const UObject* WorldContextObject)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		if (const AAPlanetWorldSettings* WorldSettings = Cast<AAPlanetWorldSettings>(World->GetWorldSettings()))
		{
			return WorldSettings->PlanetInfo;
		}
	}
	return nullptr;
}
