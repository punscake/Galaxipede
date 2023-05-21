// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/HealthAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldMaxHealth);
}

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldHealth);
}

void UHealthAttributeSet::OnRep_PassiveRegenFlat(const FGameplayAttributeData& OldPassiveRegenFlat)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, PassiveRegenFlat, OldPassiveRegenFlat);
}

void UHealthAttributeSet::OnRep_PassiveRegenPercent(const FGameplayAttributeData& OldPassiveRegenPercent)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, PassiveRegenPercent, OldPassiveRegenPercent);
}

void UHealthAttributeSet::OnRep_PassiveRegenCooldown(const FGameplayAttributeData& OldPassiveRegenCooldown)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, PassiveRegenCooldown, OldPassiveRegenCooldown);
}

void UHealthAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Armor, OldArmor);
}

void UHealthAttributeSet::OnRep_PercentDamageReduction(const FGameplayAttributeData& OldPercentDamageReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, PercentDamageReduction, OldPercentDamageReduction);
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, PassiveRegenFlat, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, PassiveRegenPercent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, PassiveRegenCooldown, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, PercentDamageReduction, COND_None, REPNOTIFY_Always);
}