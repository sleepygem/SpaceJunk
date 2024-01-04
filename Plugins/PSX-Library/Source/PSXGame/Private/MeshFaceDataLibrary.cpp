// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshFaceDataLibrary.h"

void UMeshFaceDataLibrary::GetVerticesOfFaceIndex(UStaticMeshComponent* MeshComponent, const int FaceIndex, TArray<FVector>& OutVertices, TArray<FLinearColor>& OutColours)
{
	UStaticMesh* Mesh = MeshComponent->GetStaticMesh();
	if (Mesh && Mesh->GetRenderData() && FaceIndex >= 0)
	{
		// Get the info for the LOD that is used for collision
		const int32 LODIndex = Mesh->LODForCollision;
		FStaticMeshRenderData* RenderData = Mesh->GetRenderData();
		if (RenderData->LODResources.IsValidIndex(LODIndex))
		{
			const FStaticMeshLODResources& LODResource = RenderData->LODResources[LODIndex];
			
			const FRawStaticIndexBuffer& IndexBuffer = LODResource.IndexBuffer;

			const uint32 HitVertexIndex0 = IndexBuffer.GetIndex(FaceIndex * 3);
			const uint32 HitVertexIndex1 = IndexBuffer.GetIndex(FaceIndex * 3 + 1);
			const uint32 HitVertexIndex2 = IndexBuffer.GetIndex(FaceIndex * 3 + 2);

			const FVector3f VertPos0 = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(HitVertexIndex0);
			const FVector3f VertPos1 = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(HitVertexIndex1);
			const FVector3f VertPos2 = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(HitVertexIndex2);

			const FRotator& ComponentRotation = MeshComponent->GetComponentRotation();
			const FVector& ComponentScale = MeshComponent->GetComponentScale();
			const FVector& ComponentLocation = MeshComponent->GetComponentLocation();

			const FVector TransformedVertPos0 = (ComponentRotation.RotateVector(FVector(VertPos0.X, VertPos0.Y, VertPos0.Z) * ComponentScale)) + ComponentLocation;
			const FVector TransformedVertPos1 = (ComponentRotation.RotateVector(FVector(VertPos1.X, VertPos1.Y, VertPos1.Z) * ComponentScale)) + ComponentLocation;
			const FVector TransformedVertPos2 = (ComponentRotation.RotateVector(FVector(VertPos2.X, VertPos2.Y, VertPos2.Z) * ComponentScale)) + ComponentLocation;

			TArray<FColor> VertColors;
			LODResource.VertexBuffers.ColorVertexBuffer.GetVertexColors(VertColors);
			UE_LOG(LogTemp, Warning, TEXT("Vert Color array: %d"), VertColors.Num());
			if (VertColors.Num() >= 3)
			{
				const FLinearColor VertColor0 = LODResource.VertexBuffers.ColorVertexBuffer.VertexColor(HitVertexIndex0);
				const FLinearColor VertColor1 = LODResource.VertexBuffers.ColorVertexBuffer.VertexColor(HitVertexIndex1);
				const FLinearColor VertColor2 = LODResource.VertexBuffers.ColorVertexBuffer.VertexColor(HitVertexIndex2);
				OutColours.Add(VertColor0);
				OutColours.Add(VertColor1);
				OutColours.Add(VertColor2);
			}
			

			OutVertices.Add(TransformedVertPos0);
			OutVertices.Add(TransformedVertPos1);
			OutVertices.Add(TransformedVertPos2);

			

		}
	}
}

FVector UMeshFaceDataLibrary::GetBaryCentryic(FVector Point, FVector A, FVector B, FVector C)
{
	return FMath::ComputeBaryCentric2D(Point, A, B, C);
}

FLinearColor UMeshFaceDataLibrary::InterpolateVertexColors(FVector BarycentricCoords, TArray<FLinearColor> Colors)
{
	if (Colors.Num() >= 3)
	{
		return (Colors[0] * BarycentricCoords.X) + (Colors[1] * BarycentricCoords.Y) + (Colors[2] * BarycentricCoords.Z);
	}
	return FLinearColor(0,0,0,1);
}
