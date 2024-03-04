// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraptionSubsystem.h"

void UContraptionSubsystem::TrackContraption(UCollectableItem* ItemData, AActor* Actor)
{
	DeployedActors.Add(Actor);
	DeployedContraptions.Add(ItemData);
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
