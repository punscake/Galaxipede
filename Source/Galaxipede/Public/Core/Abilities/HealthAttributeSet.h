// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HealthAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GALAXIPEDE_API UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "MaxHealth", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "PassiveRegenFlat", ReplicatedUsing = OnRep_PassiveRegenFlat)
	FGameplayAttributeData PassiveRegenFlat;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, PassiveRegenFlat)

	UPROPERTY(BlueprintReadOnly, Category = "PassiveRegenPercent", ReplicatedUsing = OnRep_PassiveRegenPercent)
	FGameplayAttributeData PassiveRegenPercent;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, PassiveRegenPercent)

	UPROPERTY(BlueprintReadOnly, Category = "PassiveRegenCooldown", ReplicatedUsing = OnRep_PassiveRegenCooldown)
	FGameplayAttributeData PassiveRegenCooldown;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, PassiveRegenCooldown)

	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "PercentDamageReduction", ReplicatedUsing = OnRep_PercentDamageReduction)
	FGameplayAttributeData PercentDamageReduction;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, PercentDamageReduction)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage)

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_PassiveRegenFlat(const FGameplayAttributeData& OldPassiveRegenFlat);

	UFUNCTION()
	virtual void OnRep_PassiveRegenPercent(const FGameplayAttributeData& OldPassiveRegenPercent);

	UFUNCTION()
	virtual void OnRep_PassiveRegenCooldown(const FGameplayAttributeData& OldPassiveRegenCooldown);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_PercentDamageReduction(const FGameplayAttributeData& OldPercentDamageReduction);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
