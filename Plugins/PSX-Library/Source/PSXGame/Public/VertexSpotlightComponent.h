// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VertexSpotlightComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSXGAME_API UVertexSpotlightComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InnerAngle = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OuterAngle = 45.0f;;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor LightColor = FLinearColor(1,1,1,1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LightStrength = 1.0f;
	
	// Sets default values for this component's properties
	UVertexSpotlightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
