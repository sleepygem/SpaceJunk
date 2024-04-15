// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ContraptionSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContraptionAddedSignature, AActor*, ContraptionActor);

USTRUCT()
struct FDeployedContraption
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AActor* DeployedActor;

	UPROPERTY()
	UCollectableItem* ItemData;

	UPROPERTY()
	FTransform ActorTransform;

	FDeployedContraption()
	{
		DeployedActor = nullptr;
		ItemData = nullptr;
	}
	
	FDeployedContraption(AActor* InActor, UCollectableItem* InItemData)
	{
		DeployedActor = InActor;
		ItemData = InItemData;
		ActorTransform = InActor->GetTransform();
	}
};

USTRUCT()
struct FCheckpointContraptions
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FDeployedContraption> Contraptions;
	
};

UCLASS()
class SPACEJUNK_API UContraptionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> DeployedActors;

	UPROPERTY(BlueprintReadOnly)
	TArray<UCollectableItem*> DeployedContraptions;

	UPROPERTY()
	TMap<int32, FCheckpointContraptions> TrackedContraptions;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCheckpointId = 0;
	
public:

	UFUNCTION(BlueprintCallable)
	void TrackContraption(UCollectableItem* ItemData, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RetrieveDeployedActors(TArray<UCollectableItem*>& OutContraptions);

	UFUNCTION(BlueprintCallable)
	void RetrieveDeployedActorsForCheckpoint(int32 CheckpointId, TArray<UCollectableItem*>& OutContraptions);

	UFUNCTION(BlueprintPure)
	bool DoesCheckpointHaveContraptions(int32 CheckpointId);
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentCheckpoint(int32 CheckpointId);
	
	UPROPERTY(BlueprintAssignable)
	FOnContraptionAddedSignature OnContraptionAddedDelegate;
	
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
