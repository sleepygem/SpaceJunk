// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ContraptionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API UContraptionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> DeployedActors;

	UPROPERTY(BlueprintReadOnly)
	TArray<UCollectableItem*> DeployedContraptions;

public:

	UFUNCTION(BlueprintCallable)
	void TrackContraption(UCollectableItem* ItemData, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RetrieveDeployedActors(TArray<UCollectableItem*>& OutContraptions);
	
};


USTRUCT()
struct FContraptionDeploymentInfo
{
	GENERATED_BODY()

public:

	FTransform OriginalTransform;

	UPROPERTY()
	UCollectableItem* ItemType;
	
};
