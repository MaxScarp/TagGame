// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomUtility.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Grabbable.h"
#include "Kismet/GameplayStatics.h"
#include "TagGameGameMode.generated.h"

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	TArray<AActor*> TargetPoints;
	TArray<AActor*> GrabbableObjects;
	TArray<APawn*> Enemies;

public:
	ATagGameGameMode();

	void BeginPlay() override;
	
	void ResetMatch();
	const TArray<AActor*>& GetGrabbableObjects() const;
};