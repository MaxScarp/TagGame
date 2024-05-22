// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "EnemyAIBlackboardData.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API UEnemyAIBlackboardData : public UBlackboardData
{
	GENERATED_BODY()
	
public:
	UEnemyAIBlackboardData();
};
