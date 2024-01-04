// Fill out your copyright notice in the Description page of Project Settings.

#include "VertexSpotLightVisualizer.h"
#include "VertexSpotlightComponent.h"

void FVertexSpotLightVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
	FPrimitiveDrawInterface* PDI)
{
	const UVertexSpotlightComponent* Spotlight = Cast<UVertexSpotlightComponent>(Component);

	if (Spotlight)
	{
		FTransform TransformNoScale = Spotlight->GetComponentTransform();
		TransformNoScale.RemoveScaling();

		//Draw Outer Cone
		DrawWireSphereCappedCone(PDI, TransformNoScale, Spotlight->Radius, Spotlight->OuterAngle,
			32, 8, 10, Spotlight->LightColor, SDPG_World);

		if (Spotlight->InnerAngle > KINDA_SMALL_NUMBER)
		{
			const FLinearColor InnerConeColour = (Spotlight->LightColor * 0.5f);
			//Draw Inner Cone
			DrawWireSphereCappedCone(PDI, TransformNoScale, Spotlight->Radius, Spotlight->InnerAngle,
				32, 8, 10, InnerConeColour, SDPG_World);
		}
	}
	
}
