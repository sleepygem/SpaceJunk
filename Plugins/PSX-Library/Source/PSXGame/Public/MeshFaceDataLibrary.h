﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshFaceDataLibrary.generated.h"

/**
 * 
 */

UCLASS()
class PSXGAME_API UMeshFaceDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="MeshData")
	static void GetVerticesOfFaceIndex(UStaticMeshComponent* MeshComponent, const int FaceIndex, TArray<FVector>& OutVertices, TArray<FLinearColor>& OutColours);

	UFUNCTION(BlueprintCallable, Category="Calculation")
	static FVector GetBaryCentric(FVector Point, FVector A, FVector B, FVector C);

	UFUNCTION(BlueprintCallable, Category="Calculation")
	static FLinearColor InterpolateVertexColors(FVector BarycentricCoords, TArray<FLinearColor> Colors);
	
};
