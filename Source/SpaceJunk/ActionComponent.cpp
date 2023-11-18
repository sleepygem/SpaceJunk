// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionComponent.h"
#include "Engine/LatentActionManager.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentAction = nullptr;
	// ...
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(CurrentAction))
	{
		CurrentAction->TickAction(GetOwner(), DeltaTime);
	}
	// ...
}

void UActionComponent::StartAction(TSubclassOf<USJAction> Action)
{
	if (IsValid(Action))
	{
		if (IsValid(CurrentAction))
		{
			USJAction* DefaultNewAction = Cast<USJAction>(Action->GetDefaultObject(true));
			//Check if new action's tag is blocked by current action
			if (!CurrentAction->BlockedTags.HasTag(DefaultNewAction->GrantsTag))
			{
				StopAction();
				CurrentAction = NewObject<USJAction>(this, Action);
				CurrentAction->OwnerActionComponent = this;
				CurrentAction->IsActive = true;
				CurrentTags.AddTag(CurrentAction->GrantsTag);
				CurrentAction->StartAction(GetOwner());
			}
		}
		else
		{
			CurrentAction = NewObject<USJAction>(this, Action);
			CurrentAction->OwnerActionComponent = this;
			CurrentAction->IsActive = true;
			CurrentTags.AddTag(CurrentAction->GrantsTag);
			CurrentAction->StartAction(GetOwner());
		}
		
	}
}

void UActionComponent::StopAction()
{
	if (IsValid(CurrentAction))
	{
		CurrentAction->StopAction(GetOwner());
		CurrentAction->IsActive = false;
		CurrentTags.RemoveTag(CurrentAction->GrantsTag);

		if (UWorld* World = GEngine->GetWorldFromContextObject(GetOuter(), EGetWorldErrorMode::LogAndReturnNull))
		{
			FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
			LatentActionManager.RemoveActionsForObject(CurrentAction);
		}
		
		CurrentAction = nullptr;
	}
}

