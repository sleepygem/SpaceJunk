// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCollectionSubsystem.h"

#include "HairStrandsInterface.h"
#include "Kismet/GameplayStatics.h"

void UItemCollectionSubsystem::MakeCheckpointSnapshot(TArray<FCollectedItemStack> PlayerInventory,
	TArray<FCollectedItemStack> ToolInventory)
{
	CollectedItemsSnapshot = CollectedItems;
	PlayerInventorySnapshot = PlayerInventory;
	PlayerToolsSnapshot = ToolInventory;
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

void UItemCollectionSubsystem::ResetAllData()
{
	CollectedItemsSnapshot.Empty();
	PlayerInventorySnapshot.Empty();
	PlayerToolsSnapshot.Empty();
	TotalScrapSnapshot = 0;
	TotalScrap = 0;

	CollectedItems.Empty();

	CutsceneViewedFlags.Empty();

	ShipInventory.Empty();
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
	const int32 NewValue = ItemStack.ItemType->Value * ItemStack.Quantity;
	TotalScrap += NewValue;
	TotalScrapSnapshot += NewValue;
	
	for (FName ActorKey : ItemStack.ActorKeys)
	{
		CollectedItemsSnapshot.Add(ActorKey, true);
	}
}


void UItemCollectionSubsystem::ResetToSnapshot()
{
	CollectedItems = CollectedItemsSnapshot;
	TotalScrap = TotalScrapSnapshot;
}

void UItemCollectionSubsystem::ClearCollectedItems()
{
    CollectedItems.Empty();
	CollectedItemsSnapshot.Empty();
	PlayerInventorySnapshot.Empty();
	PlayerToolsSnapshot.Empty();
	TotalScrapSnapshot = 0;
	TotalScrap = 0;
}

void UItemCollectionSubsystem::AddScrapValue(int32 Amount)
{
	TotalScrap += Amount;
}

int32 UItemCollectionSubsystem::GetTotalScrapValue() const
{
	return TotalScrap;
}

void UItemCollectionSubsystem::GetPlayerInventorySnapshot(TArray<FCollectedItemStack>& PlayerInventory) const
{
	PlayerInventory = PlayerInventorySnapshot;
}

void UItemCollectionSubsystem::GetShipInventory(TArray<FCollectedItemStack>& OutShipInventory) const
{
	OutShipInventory = ShipInventory;
}

void UItemCollectionSubsystem::GetPlayerTools(TArray<FCollectedItemStack>& PlayerTools) const
{
	PlayerTools = PlayerToolsSnapshot;
}

void UItemCollectionSubsystem::SetCutsceneViewedFlag(FName SceneName, bool bViewed)
{
	CutsceneViewedFlags.Add(SceneName, bViewed);
}

bool UItemCollectionSubsystem::HasCutsceneBeenViewed(const FName SceneName)
{
	if (bool* ViewFlag = CutsceneViewedFlags.Find(SceneName))
	{
		return true;
	}
	return false;
}



