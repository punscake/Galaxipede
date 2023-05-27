// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GALAXIPEDE_API UMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "MaxSpeed", ReplicatedUsing = OnRep_MaxSpeed)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxSpeed)
		
	UPROPERTY(BlueprintReadOnly, Category = "SurgeSpeedMulti", ReplicatedUsing = OnRep_SurgeSpeedMulti)
	FGameplayAttributeData SurgeSpeedMulti;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, SurgeSpeedMulti)

	UPROPERTY(BlueprintReadOnly, Category = "SurgeTime", ReplicatedUsing = OnRep_SurgeTime)
	FGameplayAttributeData SurgeTime;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, SurgeTime)
		
	UPROPERTY(BlueprintReadOnly, Category = "MaxSurgeTime", ReplicatedUsing = OnRep_MaxSurgeTime)
	FGameplayAttributeData MaxSurgeTime;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxSurgeTime)

	UPROPERTY(BlueprintReadOnly, Category = "PercentMaxSurgeRecovery", ReplicatedUsing = OnRep_PercentMaxSurgeRecovery)
	FGameplayAttributeData PercentMaxSurgeRecovery;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, PercentMaxSurgeRecovery)

	UFUNCTION()
	virtual void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed);

	UFUNCTION()
	virtual void OnRep_SurgeSpeedMulti(const FGameplayAttributeData& OldSurgeSpeedMulti);

	UFUNCTION()
	virtual void OnRep_SurgeTime(const FGameplayAttributeData& OldSurgeTime);

	UFUNCTION()
	virtual void OnRep_MaxSurgeTime(const FGameplayAttributeData& OldMaxSurgeTime);

	UFUNCTION()
	virtual void OnRep_PercentMaxSurgeRecovery(const FGameplayAttributeData& OldPercentMaxSurgeRecovery);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
