// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/LevelAttributeHelper.h"

// Sets default values for this component's properties
ULevelAttributeHelper::ULevelAttributeHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	
}


// Called when the game starts
void ULevelAttributeHelper::BeginPlay()
{
	Super::BeginPlay();

	if (const IAbilitySystemInterface* OwnerWithAttributes = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		ASC = OwnerWithAttributes->GetAbilitySystemComponent();

		if (const ULevelAttributeSet* AS = Cast<ULevelAttributeSet>(ASC->GetAttributeSet(ULevelAttributeSet::StaticClass())))
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			LevelEffectSpecHandle = ASC->MakeOutgoingSpec(LevelUpStackingEffect, 1, EffectContext);

			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetLevelAttribute()).AddUObject(this, &ULevelAttributeHelper::LevelChanged);
		}
	}
}

void ULevelAttributeHelper::LevelChanged(const FOnAttributeChangeData& Data)
{
	if (LevelEffectSpecHandle.IsValid())
	{
		int NewValue = (int)Data.NewValue;
		int dif = NewValue - (int)Data.OldValue;
		if (dif == 0) {
			return;
		}

		if (dif > 0) {
			for (dif; dif > 0; dif--) {
				LevelGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*LevelEffectSpecHandle.Data.Get(), ASC);
			}
		}
		else if (LevelGameplayEffectHandle.IsValid()) {
			for (dif; dif < 0; dif++) {
				ASC->RemoveActiveGameplayEffect(LevelGameplayEffectHandle, 1);
			}
		}
	}
}


// Called every frame
void ULevelAttributeHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

