// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableChangedSignature, UPlayerInteractionComponent*, Component);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACEJUNK_API UPlayerInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInteractionComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPromptVisible;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PromptText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InteractionTag;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsInteractable;

	UPROPERTY(BlueprintAssignable)
	FOnInteractableChangedSignature OnInteractableChangedDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayerInteract(APawn* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable)
	void SetIsInteractable(bool bEnable);
};
