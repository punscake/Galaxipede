// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/GMMC_SprintRegen.h"
#include "Core/Abilities/MovementAttributeSet.h"
/*
struct MovespeedStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxSurgeTime);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PercentMaxSurgeRecovery);

	MovespeedStruct()
	{
		//Snapshot Damage
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMovementAttributeSet, MaxSurgeTime, Source, true);
		//Snapshot Strength
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMovementAttributeSet, PercentMaxSurgeRecovery, Source, true);
	}
};
*/
struct FMovespeedStruct
{
	FGameplayEffectAttributeCaptureDefinition MaxSurgeTime;
	FGameplayEffectAttributeCaptureDefinition PercentMaxSurgeRecovery;

	FMovespeedStruct()
	{
		MaxSurgeTime = FGameplayEffectAttributeCaptureDefinition(UMovementAttributeSet::GetMaxSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		PercentMaxSurgeRecovery = FGameplayEffectAttributeCaptureDefinition(UMovementAttributeSet::GetPercentMaxSurgeRecoveryAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};


static const FMovespeedStruct& MSStatistic()
{
	static FMovespeedStruct MStruct;
	return MStruct;
}

UGMMC_SprintRegen::UGMMC_SprintRegen()
{
	RelevantAttributesToCapture.Add(MSStatistic().MaxSurgeTime);
	RelevantAttributesToCapture.Add(MSStatistic().PercentMaxSurgeRecovery);
}

float UGMMC_SprintRegen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxSurgeTime = 0.f;
	GetCapturedAttributeMagnitude(MSStatistic().MaxSurgeTime, Spec, EvaluationParameters, MaxSurgeTime);
	float PercentMaxSurgeRecovery = 0.f;
	GetCapturedAttributeMagnitude(MSStatistic().PercentMaxSurgeRecovery, Spec, EvaluationParameters, PercentMaxSurgeRecovery);

	return MaxSurgeTime / (PercentMaxSurgeRecovery * 100) * Spec.Period;
}
