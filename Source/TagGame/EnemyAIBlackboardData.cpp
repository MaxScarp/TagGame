// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIBlackboardData.h"

UEnemyAIBlackboardData::UEnemyAIBlackboardData()
{
    FBlackboardEntry NearestGrabbableObjectEntry;
    NearestGrabbableObjectEntry.EntryName = TEXT("NearestGrabbableObject");
    NearestGrabbableObjectEntry.KeyType = NewObject<UBlackboardKeyType_Object>();
    Keys.Add(NearestGrabbableObjectEntry);

    FBlackboardEntry TargetPointToFlee;
    TargetPointToFlee.EntryName = TEXT("TargetPointToFlee");
    TargetPointToFlee.KeyType = NewObject<UBlackboardKeyType_Object>();
    Keys.Add(TargetPointToFlee);
}