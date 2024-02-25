// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CollectableFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API UCollectableFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static bool AddItemToStack(UPARAM(ref) FCollectedItemStack& ItemStack, FName InKey)
	{
		return ItemStack.AddToStack(InKey);
	}

	UFUNCTION(BlueprintCallable)
	static FName RemoveItemFromStack(UPARAM(ref) FCollectedItemStack& ItemStack)
	{
		return ItemStack.RemoveFromStack();
	}

	UFUNCTION(BlueprintPure)
	static bool DoesMatchItemType(UPARAM(ref) const FCollectedItemStack& ItemStack, const UCollectableItem* ItemType)
	{
		return ItemStack.ItemType == ItemType;
	}
};
