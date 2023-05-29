// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GMMC_SprintRegen.generated.h"

/**
 * 
 */
UCLASS()
class GALAXIPEDE_API UGMMC_SprintRegen : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UGMMC_SprintRegen();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};