// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintCallable)
	static void GetVerticesOfFaceIndex(UStaticMeshComponent* MeshComponent, const int FaceIndex, TArray<FVector>& OutVertices, TArray<FLinearColor>& OutColours);

	UFUNCTION(BlueprintCallable)
	static FVector GetBaryCentryic(FVector Point, FVector A, FVector B, FVector C);

	UFUNCTION(BlueprintCallable)
	static FLinearColor InterpolateVertexColors(FVector BarycentricCoords, TArray<FLinearColor> Colors);
	
};
