// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "TagGameGameMode.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BlackboardData = CreateDefaultSubobject<UEnemyAIBlackboardData>(TEXT("BlackboardData"));

	NearestGrabbableObjectKey = TEXT("NearestGrabbableObject");
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBlackboard(*BlackboardComponent, *BlackboardData);

	SearchForGrabbableObject = MakeShared<CustomAIState>(
		[this](AAIController* AIController)
		{
			AGameModeBase* GameModeBase = AIController->GetWorld()->GetAuthGameMode();
			ATagGameGameMode* GameMode = Cast<ATagGameGameMode>(GameModeBase);
			const TArray<AActor*>& GrabbableObjects = GameMode->GetGrabbableObjects();

			AActor* CurrentGrabbableObject = nullptr;
			double MinDistance = DBL_MAX;
			for (AActor* GrabbableObject : GrabbableObjects)
			{
				if (GrabbableObject->GetAttachParentActor())
				{
					continue;
				}

				const double distanceFromActor = FVector::Distance(GrabbableObject->GetActorLocation(), AIController->GetPawn()->GetActorLocation());
				if (distanceFromActor < MinDistance)
				{
					MinDistance = distanceFromActor;
					CurrentGrabbableObject = GrabbableObject;
				}
			}

			BlackboardComponent->SetValueAsObject(NearestGrabbableObjectKey, CurrentGrabbableObject);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<CustomAIState>
		{
			if (BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey))
			{
				return GoToNearestGrabbableObject;
			}

			return SearchForGrabbableObject;
		}
	);

	GoToNearestGrabbableObject = MakeShared<CustomAIState>(
		[this](AAIController* AIController)
		{
			AIController->MoveToActor(Cast<AActor>(BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey)));
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<CustomAIState>
		{
			AActor* NearestGrabbableObject = Cast<AActor>(BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey));
			if (NearestGrabbableObject->GetAttachParentActor())
			{
				return SearchForGrabbableObject;
			}

			EPathFollowingStatus::Type FollowingStatus = AIController->GetMoveStatus();
			if (FollowingStatus == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			if (FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestGrabbableObject->GetActorLocation()) > MinDistanceFromGrabbable)
			{
				return GoToNearestGrabbableObject;
			}

			return TakeGrabbableObject;
		}
	);

	TakeGrabbableObject = MakeShared<CustomAIState>(
		[this](AAIController* AIController)
		{
			if (Cast<AActor>(BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey))->GetAttachParentActor())
			{
				BlackboardComponent->SetValueAsObject(NearestGrabbableObjectKey, nullptr);
			}
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<CustomAIState>
		{
			AActor* NearestGrabbableObject = Cast<AActor>(BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey));
			if (!NearestGrabbableObject)
			{
				return SearchForGrabbableObject;
			}

			if (NearestGrabbableObject->Implements<UGrabbable>())
			{
				IGrabbable::Execute_Grab(NearestGrabbableObject, AIController->GetPawn());
			}

			return ChasePlayer;
		}
	);

	ChasePlayer = MakeShared<CustomAIState>(
		nullptr,
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<CustomAIState>
		{
			AActor* NearestGrabbableObject = Cast<AActor>(BlackboardComponent->GetValueAsObject(NearestGrabbableObjectKey));
			AIController->MoveToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn());

			EPathFollowingStatus::Type FollowingStatus = AIController->GetMoveStatus();
			if (FollowingStatus == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			if (FVector::Distance(AIController->GetPawn()->GetActorLocation(), AIController->GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) > MaxDistanceFromPlayer)
			{
				return ChasePlayer;
			}

			if (NearestGrabbableObject->Implements<UGrabbable>())
			{
				IGrabbable::Execute_Grab(NearestGrabbableObject, AIController->GetWorld()->GetFirstPlayerController()->GetPawn());
				BlackboardComponent->SetValueAsObject(NearestGrabbableObjectKey, nullptr);
			}

			OnObjectPassedToPlayer.ExecuteIfBound();

			return SearchForGrabbableObject;
		}
	);

	CurrentState = SearchForGrabbableObject;
	CurrentState->CallEnter(this);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState != nullptr)
	{
		CurrentState = CurrentState->CallStay(this, DeltaTime);
	}
}
