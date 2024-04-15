// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractableTracker.h"

#include "EngineUtils.h"



// Sets default values for this component's properties
UPlayerInteractableTracker::UPlayerInteractableTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerInteractableTracker::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



UPlayerInteractionComponent* UPlayerInteractableTracker::GetNearestInteractable()
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	TrackedActors.Sort([&](const AActor& ActorA, const AActor& ActorB)
	{
		const float DistanceA = FVector::Dist(ActorA.GetActorLocation(), OwnerLocation);
		const float DistanceB = FVector::Dist(ActorB.GetActorLocation(), OwnerLocation);

		return DistanceA < DistanceB;
	});

	for (AActor* Actor : TrackedActors)
	{
		if (UPlayerInteractionComponent* InteractionComponent = Actor->FindComponentByClass<UPlayerInteractionComponent>())
		{
			if (InteractionComponent->bIsInteractable)
			{
				return InteractionComponent;
			}
		}
	}
	return nullptr;
}

bool UPlayerInteractableTracker::DoInteractablesExist()
{
	return !TrackedActors.IsEmpty();
}

void UPlayerInteractableTracker::AddActor(AActor* Actor)
{
	TrackedActors.Add(Actor);
}

void UPlayerInteractableTracker::RemoveActor(AActor* Actor)
{
	if (const int ActorIndex = TrackedActors.Find(Actor); ActorIndex != INDEX_NONE)
	{
		TrackedActors.RemoveAt(ActorIndex);
	}
}

