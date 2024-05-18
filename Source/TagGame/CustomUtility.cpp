// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUtility.h"

void UCustomUtility::ShuffleActorArray(TArray<AActor*>& Array)
{
	const int32 LastIndex = Array.Num() - 1;
	for (int32 Index = 0; Index <= LastIndex; ++Index)
	{
		const int32 RandomIndex = FMath::RandRange(Index, LastIndex);
		if (Index != RandomIndex)
		{
			Array.Swap(Index, RandomIndex);
		}
	}
}

void UCustomUtility::AttachActorToPawnRightHand(APawn* Pawn, AActor* GrabbableObject)
{
	if (!Pawn || !GrabbableObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn or GrabbableObject is null!"));
		return;
	}

	USkeletalMeshComponent* MeshComponent = Pawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn does not have a SkeletalMeshComponent!"));
		return;
	}

	FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GrabbableObject->AttachToComponent(MeshComponent, AttachmentRules, FName("hand_r"));
}