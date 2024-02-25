// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CollectableItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class SPACEJUNK_API UCollectableItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ItemName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Value;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 MaxStack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText InteractPrompt;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AttackPrompt;
	
};


USTRUCT(BlueprintType)
struct FCollectedItemStack
{

	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCollectableItem* ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> ActorKeys;

	FCollectedItemStack() : ItemType(nullptr), Quantity(0) {}
	
	FCollectedItemStack(UCollectableItem* InItemType, const int32 InQuantity)
	: ItemType(InItemType), Quantity(InQuantity){}
	
	bool AddToStack(FName InKey)
	{
		if (Quantity < ItemType->MaxStack)
		{
			ActorKeys.Add(InKey);
			Quantity++;
			return true;
		}
		return false;
	}
	
	FName RemoveFromStack()
	{
		if (ActorKeys.IsEmpty()){ return FName();}
		
		return ActorKeys.Pop();
	}
};