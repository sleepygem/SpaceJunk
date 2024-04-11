// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCPPFunctions.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UCommonCPPFunctions::SetAnimRootMotionTranslationScale(ACharacter* Character, float TranslationScale)
{
	Character->SetAnimRootMotionTranslationScale(TranslationScale);
}

void UCommonCPPFunctions::RLerpBasedOnGroundAngle(USceneComponent* Component, float AngleLimit, float Speed, float DeltaSeconds, EDrawDebugTrace::Type DrawDebug)
{
	const FVector TraceStart = Component->GetComponentLocation();
	const FVector TraceEnd = TraceStart + FVector::UpVector * -400.0f ;

	AActor* ComponentActor = Component->GetOwner();

	const TArray<AActor*> IgnoreArray({ComponentActor});

	FHitResult HitResult;
	
	if (UKismetSystemLibrary::LineTraceSingle(Component,
		TraceStart, TraceEnd, ETraceTypeQuery::TraceTypeQuery1, false,
		IgnoreArray, DrawDebug, HitResult, true))
	{
		FRotator NormalRotation = UKismetMathLibrary::MakeRotFromYZ(Component->GetForwardVector(), HitResult.ImpactNormal);
		NormalRotation.SetComponentForAxis(EAxis::Y, FMath::Clamp(NormalRotation.Pitch, 0.f, AngleLimit));
		const FRotator CurrentRotation = FRotator(0.f, -270.0f, 0.f) + Component->GetComponentRotation();
		const FQuat LerpedRotator = FQuat::Slerp(CurrentRotation.Quaternion(),NormalRotation.Quaternion(), Speed * DeltaSeconds);

		const FRotator ResultRotation = FRotator(LerpedRotator) + FRotator(0.f, 270.0f, 0.f);
		
		Component->SetWorldRotation(LerpedRotator);
	}
}
