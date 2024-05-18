// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomUtility.h"
#include "Grabbable.h"
#include "Ball.generated.h"

UCLASS()
class TAGGAME_API ABall : public AActor, public IGrabbable
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Grab_Implementation(APawn* Pawn) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
