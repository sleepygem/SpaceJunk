// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshFaceDataLibrary.h"

#include "ComponentReregisterContext.h"
#include "LandscapeRender.h"
#include "StaticMeshComponentLODInfo.h"
#include "StaticMeshResources.h"
#include "Engine/StaticMesh.h"

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

FVector UMeshFaceDataLibrary::GetBaryCentric(FVector Point, FVector A, FVector B, FVector C)
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

void UMeshFaceDataLibrary::BakeDirectionalLightColors(UStaticMeshComponent* MeshComponent,
	FVertexDirectionalLightInfo& LightInfo, bool bClearExistingColor, float LightMultiplier, bool bUseSRGB)
{
	UStaticMesh* Mesh = MeshComponent->GetStaticMesh();
	FStaticMeshRenderData* RenderData = Mesh->GetRenderData();

	MeshComponent->SetFlags(RF_Transactional);
	MeshComponent->Modify();
	const int32 NumLODs = Mesh->GetNumLODs();
	MeshComponent->SetLODDataCount(NumLODs, NumLODs);
	
	UE_LOG(LogTemp, Warning, TEXT("Instance LODs: %d"), MeshComponent->LODData.Num())
	
	FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &MeshComponent->LODData[0];
	
	if (RenderData->LODResources.IsValidIndex(0))
	{
		const FStaticMeshLODResources& LODResource = RenderData->LODResources[0];

		InstanceMeshLODInfo->PaintedVertices.Empty();

		// Setup the instance vertex color array if we don't have one yet
		InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;

		TArray<FColor> NewColorArray;
		NewColorArray.Reserve(LODResource.GetNumVertices());

		for (int i = 0; i < LODResource.GetNumVertices(); i++)
		{
			//const FVector3f VertexPosition = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
			const FVector4f VertexNormal = LODResource.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
			FVector ConvertedNormal = FVector(VertexNormal.X, VertexNormal.Y, VertexNormal.Z);

			float LightInfluence = -1.0f * ConvertedNormal.Dot(LightInfo.Direction);
			LightInfluence = FMath::Clamp(LightInfluence, 0.f, 1.0f);

			const FLinearColor AmbientColor = LightInfo.AmbientColor * LightInfo.AmbientStrength;
			const FLinearColor LightColor = LightInfo.LightColor * LightInfo.LightStrength;
		
			FLinearColor InterpolatedLightColor = FMath::Lerp(AmbientColor, LightColor, LightInfluence);
			InterpolatedLightColor.A = 1.0f;

			const FColor NewColor = FMath::Lerp(FLinearColor::White, InterpolatedLightColor, LightMultiplier).ToFColor(bUseSRGB);
			NewColorArray.Add(NewColor);
			
		}

		InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(NewColorArray);
		BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
		MeshComponent->MarkRenderStateDirty();
		
		/*
		const bool bValidInstanceData = InstanceMeshLODInfo
			&& InstanceMeshLODInfo->OverrideVertexColors
			&& InstanceMeshLODInfo->OverrideVertexColors->GetNumVertices() == LODResource.GetNumVertices();

		if (!bValidInstanceData)
		{
			

			if ((int32)LODResource.VertexBuffers.ColorVertexBuffer.GetNumVertices() >= LODResource.GetNumVertices())
			{
				// copy mesh vertex colors to the instance ones
				InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODResource.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODResource.GetNumVertices());
			}
			else
			{
				// Original mesh didn't have any colors, so just use a default color
				InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, LODResource.GetNumVertices());
			}
		}
		else
		{
			// Destroy the cached paint data every paint. Painting redefines the source data.
			InstanceMeshLODInfo->PaintedVertices.Empty();
			InstanceMeshLODInfo->BeginReleaseOverrideVertexColors();
			FlushRenderingCommands();
		}
		check(InstanceMeshLODInfo->OverrideVertexColors);

		UE_LOG(LogTemp, Warning, TEXT("Valid Instance Data found: Vertices = %d, Colors = %d"), LODResource.GetNumVertices(), InstanceMeshLODInfo->OverrideVertexColors->GetNumVertices())
			
		for (int i = 0; i < (int32)InstanceMeshLODInfo->OverrideVertexColors->GetNumVertices(); i++)
		{
			//const FVector3f VertexPosition = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
			const FVector4f VertexNormal = LODResource.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
			FVector ConvertedNormal = FVector(VertexNormal.X, VertexNormal.Y, VertexNormal.Z);

			float LightInfluence = -1.0f * ConvertedNormal.Dot(LightInfo.Direction);
			LightInfluence = FMath::Clamp(LightInfluence, 0.f, 1.0f);

			const FLinearColor AmbientColor = LightInfo.AmbientColor * LightInfo.AmbientStrength;
			const FLinearColor LightColor = LightInfo.LightColor * LightInfo.LightStrength;
		
			FLinearColor InterpolatedLightColor = FMath::Lerp(AmbientColor, LightColor, LightInfluence);
			InterpolatedLightColor.A = 1.0f;
			
			if (bClearExistingColor)
			{
				const FColor NewColor = FMath::Lerp(FLinearColor::White, InterpolatedLightColor, LightMultiplier).ToFColor(true);
				InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) = NewColor;
				//InstanceMeshLODInfo->PaintedVertices[i].Color = NewColor;
			}
			else
			{
				const FLinearColor CombinedColors = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i).ReinterpretAsLinear() * InterpolatedLightColor;
				InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) = CombinedColors.ToFColorSRGB();
				//InstanceMeshLODInfo->PaintedVertices[i].Color = CombinedColors.ToFColorSRGB();
			}
		}
		// See if the component has to cache its mesh vertex positions associated with override colors
		//MeshComponent->CachePaintedDataIfNecessary();
		//MeshComponent->StaticMeshDerivedDataKey = Mesh->GetRenderData()->DerivedDataKey;

		/* Lighting does not need to be invalidated when mesh painting
		const bool bUnbuildLighting = false;
		
		// Recreate all component states using the referenced static mesh
		FStaticMeshComponentRecreateRenderStateContext RecreateRenderStateContext(Mesh, bUnbuildLighting);
		
		// We're only changing instanced vertices on this specific mesh component, so we
		// only need to detach our mesh component
		FComponentReregisterContext ComponentReregisterContext(MeshComponent);
		
		for (int32 Index = 0; Index < MeshComponent->LODData.Num(); ++Index)
		{
			BeginInitResource(MeshComponent->LODData[Index].OverrideVertexColors);
		}*/
		
	}
}

void UMeshFaceDataLibrary::ClearVertexColors(UStaticMeshComponent* MeshComponent)
{
	UStaticMesh* Mesh = MeshComponent->GetStaticMesh();
	FStaticMeshRenderData* RenderData = Mesh->GetRenderData();

	MeshComponent->SetFlags(RF_Transactional);
	MeshComponent->Modify();
	const int32 NumLODs = Mesh->GetNumLODs();
	MeshComponent->SetLODDataCount(NumLODs, NumLODs);
	
	UE_LOG(LogTemp, Warning, TEXT("Instance LODs: %d"), MeshComponent->LODData.Num())
	
	FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &MeshComponent->LODData[0];
	
	if (RenderData->LODResources.IsValidIndex(0))
	{
		const FStaticMeshLODResources& LODResource = RenderData->LODResources[0];

		const bool bValidInstanceData = InstanceMeshLODInfo
			&& InstanceMeshLODInfo->OverrideVertexColors
			&& InstanceMeshLODInfo->OverrideVertexColors->GetNumVertices() == LODResource.GetNumVertices();

		if (!bValidInstanceData)
		{
			// Setup the instance vertex color array if we don't have one yet
			InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
		}
		InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, LODResource.GetNumVertices());
	}
	
}


