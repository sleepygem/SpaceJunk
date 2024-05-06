// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCPPFunctions.h"

#include "Engine/UserInterfaceSettings.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UCommonCPPFunctions::SetAnimRootMotionTranslationScale(ACharacter* Character, float TranslationScale)
{
	Character->SetAnimRootMotionTranslationScale(TranslationScale);
}

void UCommonCPPFunctions::RLerpBasedOnGroundAngle(USceneComponent* Component, float AngleLimit, float Speed, float DeltaSeconds, float TraceHeight = 100.0f, EDrawDebugTrace::Type DrawDebug = EDrawDebugTrace::None)
{
	AActor* ComponentActor = Component->GetOwner();
	
	const FVector TraceStart = ComponentActor->GetActorLocation();
	const FVector TraceEnd = TraceStart + FVector::UpVector * (-TraceHeight) ;

	

	const TArray<AActor*> IgnoreArray({ComponentActor});

	FHitResult HitResult;
	
	if (UKismetSystemLibrary::LineTraceSingle(Component,
		TraceStart, TraceEnd, ETraceTypeQuery::TraceTypeQuery1, false,
		IgnoreArray, DrawDebug, HitResult, true))
	{
		const FVector NormalCrossProduct = HitResult.ImpactNormal.Cross(ComponentActor->GetActorRightVector());
		FRotator NormalRotation = UKismetMathLibrary::MakeRotFromYZ(NormalCrossProduct * -1.0f, HitResult.ImpactNormal);
		//NormalRotation.SetComponentForAxis(EAxis::Y, FMath::Clamp(NormalRotation.Pitch, 0.f, AngleLimit));
		//const FRotator CurrentRotation = UKismetMathLibrary::ComposeRotators(FRotator(0.f, -270.0f, 0.f), Component->GetComponentRotation());
		const FQuat LerpedRotator = FQuat::Slerp(Component->GetComponentRotation().Quaternion(),NormalRotation.Quaternion(), Speed * DeltaSeconds);
		
		//const FQuat ResultRotation = LerpedRotator * FRotator(0.f, 270.0f, 0.f).Quaternion();
		
		Component->SetWorldRotation(LerpedRotator);
	}
	else
	{
		
		const FQuat LerpedRotator = FQuat::Slerp(Component->GetRelativeRotation().Quaternion(),FRotator(0.f, 270.0f, 0.f).Quaternion(), Speed * 2.0f * DeltaSeconds);
		Component->SetRelativeRotation(LerpedRotator);
	}
}

AActor* UCommonCPPFunctions::GetNearestValidInteractable(AActor* OwnerActor, TArray<AActor*> InActorList, bool& bIsValid)
{

    if (InActorList.Num() == 0)
    {
    	bIsValid = false;
	    return nullptr;
    }
    
	InActorList.Sort([&](const AActor& ActorA, const AActor& ActorB)
	{
		const float DistanceA = FVector::Dist(OwnerActor->GetActorLocation(), ActorA.GetActorLocation());
		const float DistanceB = FVector::Dist(OwnerActor->GetActorLocation(), ActorB.GetActorLocation());
		return DistanceA < DistanceB;
	});
	
	for (int i = 0; i < InActorList.Num(); i++)
	{
		AActor* Interactable = InActorList[i];
		const TArray<AActor*> ActorsToIgnore({OwnerActor});
		FHitResult HitResult;
		
		if (UKismetSystemLibrary::LineTraceSingle(OwnerActor,
			OwnerActor->GetActorLocation(),
			Interactable->GetActorLocation(),
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			HitResult,
			true))
		{
			if (HitResult.GetActor() == Interactable)
			{
				bIsValid = true;
				UE_LOG(LogTemp, Warning, TEXT("Interactable Hit: %s"), *Interactable->GetName())
				return Interactable;
			}
		}
	}
	
	bIsValid = false;
	return nullptr;
}

void UCommonCPPFunctions::SetUIScale(float NewScale)
{
	UUserInterfaceSettings* UISettings = GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());

	if (UISettings)
	{
		UISettings->ApplicationScale = NewScale;
	}
}
