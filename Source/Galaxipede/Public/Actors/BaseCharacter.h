// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Core/Abilities/MyGameplayAbility.h"
#include "Core/Abilities/HealthAttributeSet.h"
#include "Core/Abilities/MovementAttributeSet.h"
#include "Core/Abilities/LevelAttributeSet.h"
#include "BaseCharacter.generated.h"

UCLASS()
class GALAXIPEDE_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// Override for custom ability binds
	virtual void BindControls();

	virtual void AddStartingGameplayAbilitiesAndEffects();

	// Abilities
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UMovementAttributeSet> MovementAttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ULevelAttributeSet> LevelAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartingGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UMyGameplayAbility>> StartingGameplayAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

public:	

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
