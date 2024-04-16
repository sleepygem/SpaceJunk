// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraptionSubsystem.h"

#include "K2Node_SpawnActorFromClass.h"

void UContraptionSubsystem::TrackContraption(UCollectableItem* ItemData, AActor* Actor)
{
	//DeployedActors.Add(Actor);
	//DeployedContraptions.Add(ItemData);

	const FDeployedContraption ContraptionInfo = FDeployedContraption(Actor, ItemData);
	
	if (FCheckpointContraptions* CheckpointContraptions = TrackedContraptions.Find(CurrentCheckpointId))
	{
		CheckpointContraptions->Contraptions.Add(ContraptionInfo);
		return;
	}

	FCheckpointContraptions NewCheckpointContraptionList = FCheckpointContraptions();
	NewCheckpointContraptionList.Contraptions.Add(ContraptionInfo);
	TrackedContraptions.Add(CurrentCheckpointId, NewCheckpointContraptionList);
	
	OnContraptionAddedDelegate.Broadcast(Actor);
}

void UContraptionSubsystem::RetrieveDeployedActors(TArray<UCollectableItem*>& OutContraptions)
{
	for (AActor* Actor : DeployedActors)
	{
		Actor->Destroy();
	}
	DeployedActors.Empty();

	OutContraptions = DeployedContraptions;
	DeployedContraptions.Empty();
}

void UContraptionSubsystem::RetrieveDeployedActorsForCheckpoint(int32 CheckpointId,
	TArray<UCollectableItem*>& OutContraptions)
{
	if (FCheckpointContraptions* CheckpointContraptions = TrackedContraptions.Find(CurrentCheckpointId))
	{
		for (FDeployedContraption& Contraption : CheckpointContraptions->Contraptions)
		{
			if (Contraption.DeployedActor != nullptr)
			{
				Contraption.DeployedActor->Destroy();
			}
			OutContraptions.Add(Contraption.ItemData);
		}
		CheckpointContraptions->Contraptions.Empty();
	}
}

bool UContraptionSubsystem::DoesCheckpointHaveContraptions(int32 CheckpointId)
{
	if (const FCheckpointContraptions* CheckpointContraptions = TrackedContraptions.Find(CheckpointId))
	{
		return !CheckpointContraptions->Contraptions.IsEmpty();
	}
	return false;
}

void UContraptionSubsystem::SetCurrentCheckpoint(int32 CheckpointId)
{
	CurrentCheckpointId = CheckpointId;
	SnapshotContraptions = TrackedContraptions;
}

void UContraptionSubsystem::RespawnAllContraptions(UWorld* WorldContext)
{
	TArray<FCheckpointContraptions> CheckpointList;
	SnapshotContraptions.GenerateValueArray(CheckpointList);
	
	for (FCheckpointContraptions& Checkpoint : CheckpointList)
	{
		for (FDeployedContraption& Contraption : Checkpoint.Contraptions)
		{
			const UCollectableItem* ItemData = Contraption.ItemData;
			Contraption.DeployedActor = WorldContext->SpawnActor(ItemData->ActorClass, &Contraption.ActorTransform);
		}	
	}

	TrackedContraptions = SnapshotContraptions;
}
