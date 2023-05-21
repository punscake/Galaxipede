// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../Galaxipede.h"
#include "MyGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GALAXIPEDE_API UMyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		EGalaxipedeAbilityInputID AbilityInputID = EGalaxipedeAbilityInputID::None;
};
