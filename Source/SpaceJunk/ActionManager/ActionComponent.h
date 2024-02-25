// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SJAction.h"
#include "ActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEJUNK_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	FGameplayTagContainer CurrentTags;
	FGameplayTagContainer BlockedTags;

	UPROPERTY()
	USJAction* CurrentAction;
	
	UPROPERTY()
	TSubclassOf<USJAction> PreviousAction;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartAction(TSubclassOf<USJAction> Action);
    
	UFUNCTION(BlueprintCallable)
	void StopAction();

	UFUNCTION(BlueprintCallable)
	void ReturnToPreviousAction();
	
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetCurrentTags()
	{
		return CurrentTags;
	}

	UFUNCTION(BlueprintCallable)
	USJAction* GetCurrentAction()
	{
		return CurrentAction;
	}

	UFUNCTION(BlueprintCallable)
	TSubclassOf<USJAction> GetPreviousAction()
	{
		return PreviousAction;
	}

private:

	void StopCurrentAction(bool bIsBeingReplaced);
};
