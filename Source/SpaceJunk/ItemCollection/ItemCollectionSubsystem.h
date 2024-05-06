// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemCollectionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPACEJUNK_API UItemCollectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	
	TMap<FName, bool> CollectedItems;
	TMap<FName, bool> CollectedItemsSnapshot;

	TArray<FCollectedItemStack> PlayerInventorySnapshot;
	TArray<FCollectedItemStack> PlayerToolsSnapshot;
	TArray<FCollectedItemStack> ShipInventory;

	int32 TotalScrap;
	int32 TotalScrapSnapshot;
	
public:

	UFUNCTION(BlueprintCallable)
	void MakeCheckpointSnapshot(TArray<FCollectedItemStack> PlayerInventory, TArray<FCollectedItemStack> ToolInventory);

	UFUNCTION(BlueprintCallable)
	void SetKeyAsCollected(FName Key);

	UFUNCTION(BlueprintCallable)
	void RemoveKeyFromCollection(const FName Key);

	UFUNCTION(BlueprintPure)
	bool IsCollected(const FName Key) const;

	UFUNCTION(BlueprintCallable)
	void AddToShipInventory(UPARAM(ref) FCollectedItemStack& ItemStack);
	
	UFUNCTION(BlueprintCallable)
	void ResetToSnapshot();
	
	UFUNCTION(BlueprintCallable)
	void ClearCollectedItems();

	UFUNCTION(BlueprintCallable)
	void AddScrapValue(int32 Amount);
	
	UFUNCTION(BlueprintPure)
	int32 GetTotalScrapValue() const;

	UFUNCTION(BlueprintPure)
	void GetPlayerInventorySnapshot(TArray<FCollectedItemStack>& PlayerInventory) const;

	UFUNCTION(BlueprintPure)
	void GetShipInventory(TArray<FCollectedItemStack>& OutShipInventory) const;

	UFUNCTION(BlueprintPure)
	void GetPlayerTools(TArray<FCollectedItemStack>& PlayerTools) const;
};

