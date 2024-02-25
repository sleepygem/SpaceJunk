// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCollectionSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UItemCollectionSubsystem::MakeCheckpointSnapshot(TArray<FCollectedItemStack> PlayerInventory)
{
	CollectedItemsSnapshot = CollectedItems;
	PlayerInventorySnapshot = PlayerInventory;
	TotalScrapSnapshot = TotalScrap; 
}

void UItemCollectionSubsystem::SetKeyAsCollected(FName Key)
{
	if (!Key.IsNone())
	{
		CollectedItems.Add(Key, true);
		UE_LOG(LogTemp, Warning, TEXT("%s key added to collection"), *Key.ToString());
	}
}

void UItemCollectionSubsystem::RemoveKeyFromCollection(const FName Key)
{
	CollectedItems.Remove(Key);
}

bool UItemCollectionSubsystem::IsCollected(const FName Key) const
{
	if (CollectedItemsSnapshot.Find(Key))
	{
		return true;
	}
	return false;
}

void UItemCollectionSubsystem::AddToShipInventory(FCollectedItemStack& ItemStack)
{
	ShipInventory.Add(ItemStack);
	TotalScrap += ItemStack.ItemType->Value * ItemStack.Quantity;
	
}


void UItemCollectionSubsystem::ResetToSnapshot()
{
	CollectedItems = CollectedItemsSnapshot;
}

void UItemCollectionSubsystem::ClearCollectedItems()
{
    CollectedItems.Empty();
	CollectedItemsSnapshot.Empty();
	PlayerInventorySnapshot.Empty();
}

void UItemCollectionSubsystem::AddScrapValue(int32 Amount)
{
	TotalScrap += Amount;
}

int32 UItemCollectionSubsystem::GetTotalScrapValue() const
{
	return TotalScrap;
}


