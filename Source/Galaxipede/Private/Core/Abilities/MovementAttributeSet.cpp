// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/MovementAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UMovementAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MaxSpeed, OldMaxSpeed);
}

void UMovementAttributeSet::OnRep_SurgeSpeedMulti(const FGameplayAttributeData& OldSurgeSpeedMulti)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, SurgeSpeedMulti, OldSurgeSpeedMulti);
}

void UMovementAttributeSet::OnRep_SurgeTime(const FGameplayAttributeData& OldSurgeTime)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, SurgeTime, OldSurgeTime);
}

void UMovementAttributeSet::OnRep_MaxSurgeTime(const FGameplayAttributeData& OldMaxSurgeTime)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MaxSurgeTime, OldMaxSurgeTime);
}

void UMovementAttributeSet::OnRep_PercentMaxSurgeRecovery(const FGameplayAttributeData& OldPercentMaxSurgeRecovery)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, PercentMaxSurgeRecovery, OldPercentMaxSurgeRecovery);
}

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttributeSet, MaxSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttributeSet, SurgeSpeedMulti, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttributeSet, SurgeTime, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttributeSet, MaxSurgeTime, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttributeSet, PercentMaxSurgeRecovery, COND_None, REPNOTIFY_Always);
}
