// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/LevelAttributeSet.h"
#include "Net/UnrealNetwork.h"

void ULevelAttributeSet::OnRep_MaxLevel(const FGameplayAttributeData& OldMaxLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelAttributeSet, MaxLevel, OldMaxLevel);
}

void ULevelAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelAttributeSet, Level, OldLevel);
}

void ULevelAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULevelAttributeSet, MaxLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULevelAttributeSet, Level, COND_None, REPNOTIFY_Always);
}
