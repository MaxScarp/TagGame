// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIBlackboardData.h"

UEnemyAIBlackboardData::UEnemyAIBlackboardData()
{
    FBlackboardEntry Entry;
    Entry.EntryName = TEXT("NearestGrabbableObject");
    Entry.KeyType = NewObject<UBlackboardKeyType_Object>();
    Keys.Add(Entry);
}