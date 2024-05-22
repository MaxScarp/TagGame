// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "EnemyAIController.h"
#include "UObject/ConstructorHelpers.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameGameMode::ResetMatch()
{
	if (GrabbableObjectPassedToPlayerCounter < GrabbableObjects.Num())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("MATCH RESETTED!"));

	GrabbableObjectPassedToPlayerCounter = 0;

	TargetPoints.Empty();
	GrabbableObjects.Empty();
	Enemies.Empty();

	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UGrabbable::StaticClass(), GrabbableObjects);
	for (AActor* GrabbableObject : GrabbableObjects)
	{
		if (GrabbableObject->GetAttachParentActor())
		{
			GrabbableObject->AttachToActor(nullptr, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		if (It->GetController() == GetWorld()->GetFirstPlayerController())
		{
			continue;
		}

		Enemies.Add(*It);
		AEnemyAIController* EnemyController = Cast<AEnemyAIController>(It->GetController());
		if (EnemyController)
		{
			EnemyController->OnObjectPassedToPlayer.BindLambda(
				[this]()
				{
					GrabbableObjectPassedToPlayerCounter++;
					ResetMatch();
				});
		}
	}

	if (GrabbableObjects.Num() > TargetPoints.Num() || GrabbableObjects.Num() < Enemies.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough destination points or too much enemies! Destination points: %d - Grabbable Objects: %d - Enemies %d."), TargetPoints.Num(), GrabbableObjects.Num(), Enemies.Num());
		return;
	}

	UCustomUtility::ShuffleActorArray(TargetPoints);
	UCustomUtility::ShuffleActorArray(GrabbableObjects);
	for (int32 Index = 0; Index < GrabbableObjects.Num(); Index++)
	{
		AActor* GrabbableObject = GrabbableObjects[Index];
		AActor* TargetPoint = TargetPoints[Index];

		if (GrabbableObject && TargetPoint)
		{
			GrabbableObject->SetActorLocation(TargetPoint->GetActorLocation());
		}
	}
}

const TArray<AActor*>& ATagGameGameMode::GetGrabbableObjects() const
{
	return GrabbableObjects;
}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ResetMatch();
}