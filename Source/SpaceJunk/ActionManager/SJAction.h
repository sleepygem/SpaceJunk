// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SJAction.generated.h"

/**
 * 
 */
class UActionComponent;

UCLASS(Blueprintable, Abstract)
class SPACEJUNK_API USJAction : public UObject
{
	GENERATED_BODY()
	
public:

	USJAction()
	{
		OwnerActionComponent = nullptr;
	};

	UPROPERTY(BlueprintReadOnly)
	bool IsActive = false;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ActionName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag GrantsTag;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer BlockedTags;

	UPROPERTY(BlueprintReadOnly)
	UActionComponent* OwnerActionComponent;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopAction(AActor* Instigator, bool bIsBeingReplaced);

	UFUNCTION(BlueprintImplementableEvent)
	void TickAction(AActor* Instigator, float DeltaSeconds);
	
	virtual class UWorld* GetWorld() const override;
};
