// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Grabbable.h"
#include "CustomUtility.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API UCustomUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CUSTOM UTILITY")
	static void ShuffleActorArray(TArray<AActor*>& Array);

	UFUNCTION(BlueprintCallable, Category = "CUSTOM UTILITY")
	static void AttachActorToPawnRightHand(APawn* Pawn, AActor* GrabbableObject);
};