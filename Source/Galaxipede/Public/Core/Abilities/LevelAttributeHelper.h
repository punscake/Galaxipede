// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "LevelAttributeSet.h"
#include "LevelAttributeHelper.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GALAXIPEDE_API ULevelAttributeHelper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelAttributeHelper();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> LevelUpStackingEffect;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Never call explicitly! Only to be bound to a delegate, assumes ASC is not nullptr!
	virtual void LevelChanged(const FOnAttributeChangeData& Data);

	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayEffectSpecHandle LevelEffectSpecHandle;

	FActiveGameplayEffectHandle LevelGameplayEffectHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
