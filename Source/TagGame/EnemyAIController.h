// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "TagGameGameMode.h"
#include "EnemyAIController.generated.h"

struct CustomAIState : public TSharedFromThis<CustomAIState>
{
	TFunction<void(AAIController*)> Enter;
	TFunction<void(AAIController*)> Exit;
	TFunction<TSharedPtr<CustomAIState>(AAIController*, const float)> Stay;

	CustomAIState()
	{
		Enter = nullptr;
		Exit = nullptr;
		Stay = nullptr;
	}

	CustomAIState(
		TFunction<void(AAIController*)> InEnter = nullptr,
		TFunction<void(AAIController*)> InExit = nullptr,
		TFunction<TSharedPtr<CustomAIState>(AAIController*, const float)> InStay = nullptr
	) :
		Enter(InEnter),
		Exit(InExit),
		Stay(InStay)
	{}

	CustomAIState(const CustomAIState& Other) = delete;
	CustomAIState(const CustomAIState&& Other) = delete;
	CustomAIState& operator=(CustomAIState& Other) = delete;
	CustomAIState& operator=(CustomAIState&& Other) = delete;

	void CallEnter(AAIController* AIController)
	{
		if (Enter)
		{
			Enter(AIController);
		}
	}

	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Exit(AIController);
		}
	}

	TSharedPtr<CustomAIState> CallStay(AAIController* AIController, const float DeltaTime)
	{
		if (Stay)
		{
			TSharedPtr<CustomAIState> NewState = Stay(AIController, DeltaTime);
			if (NewState != nullptr && NewState != AsShared())
			{
				CallExit(AIController);
				NewState->CallEnter(AIController);

				return NewState;
			}
		}

		return AsShared();
	}
};

/**
 *
 */
UCLASS()
class TAGGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	TSharedPtr<CustomAIState> CurrentState;
	TSharedPtr<CustomAIState> ChasePlayer;
	TSharedPtr<CustomAIState> SearchForGrabbableObject;
	TSharedPtr<CustomAIState> GoToNearestGrabbableObject;
	TSharedPtr<CustomAIState> TakeGrabbableObject;

	AActor* NearestGrabbableObject;
	const double MaxDistanceFromPlayer = 100.0;
	const double MinDistanceFromGrabbable = 100.0;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
};