// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableComponent.h"

#include "ItemCollectionSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UCollectableComponent::UCollectableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCollectableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		UItemCollectionSubsystem* CollectionSubsystem = GameInstance->GetSubsystem<UItemCollectionSubsystem>();
		
		AActor* OwnerActor = GetOwner();

		// Checking if the owner actor was spawned during gameplay. If so, we check the instigator for a collectable key
		// and copy it if found.
		if (!OwnerActor->IsNetStartupActor())
		{
			if (const AActor* OwnerSpawner = OwnerActor->GetOwner())
			{
				if (const UCollectableComponent* SpawnerCollectable =
					OwnerSpawner->FindComponentByClass<UCollectableComponent>())
				{
					Key = SpawnerCollectable->Key;
				}
			}
		}
		else
		{
			Key = FName(*OwnerActor->GetName());
			
			if(CollectionSubsystem->IsCollected(Key))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s already collected, destroying..."), *OwnerActor->GetName());
				OwnerActor->Destroy();
			}
		}
	}
}

void UCollectableComponent::Collected()
{
	if(const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		UItemCollectionSubsystem* CollectionSubsystem = GameInstance->GetSubsystem<UItemCollectionSubsystem>();
		CollectionSubsystem->SetKeyAsCollected(Key);
	}
}

