// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.h"
#include "PlayerInteractableTracker.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACEJUNK_API UPlayerInteractableTracker : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInteractableTracker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> TrackedActors;

	UPROPERTY()
	bool IsInteractionEnabled = true;
	
public:

	UFUNCTION(BlueprintCallable)
	UPlayerInteractionComponent* GetNearestInteractable();

	UFUNCTION(BlueprintPure)
	bool DoInteractablesExist();
	
	UFUNCTION(BlueprintCallable)
	void AddActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemoveActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void SetInteractionEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure)
	bool GetInteractionEnabled();
};
