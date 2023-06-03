// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Core/Abilities/MovementAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Containers/Queue.h"
#include "CustomCharacterMovementComponent.generated.h"


USTRUCT(BlueprintType)
struct FTrail
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trail Data")
    FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trail Data")
    FRotator Rotation;

	// Specifies if travelling to this point should be instant (no distance requirement)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trail Data")
    bool bTeleportLogic;

	FTrail() {}

	FTrail(FVector loc, FRotator rot, bool bUseTeleportLogic = false) : Location(loc), Rotation(rot), bTeleportLogic(bUseTeleportLogic){}
};

UCLASS()
class GALAXIPEDE_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	class FSavedMove_Custom : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		virtual void PrepMoveFor(class ACharacter* Character) override;

		//Fly Speed Update
		uint8 bSavedRequestMaxFlySpeedChange : 1;
	};

	class FNetworkPredictionData_Client_Custom : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:

	UCustomCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer);

	//virtual bool HandlePendingLaunch() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity);

	//Set Max Fly Speed
	uint8 bRequestMaxFlySpeedChange : 1;

	// Should be set to true on all Segment character types
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trail Data")
	bool bRunTrailLogic;

	// Made up of points (defined by location and rotation), each point specifies if trave should be instant or not (bTeleportLogic)
	TQueue<FTrail, EQueueMode::Spsc> TrailQueue;

	UFUNCTION()
	void TravelAlongTrail(float Distance);

	UFUNCTION(Unreliable, Server)//, WithValidation)
	void Server_SetMaxFlySpeed(const float NewMaxFlySpeed);

	float CustomNewMaxFlySpeed;

	//Set Max Fly Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "Max Fly Speed")
		void SetMaxFlySpeed(float NewMaxFlySpeed);

protected:

	virtual void BeginPlay() override;

	// Never call explicitly! Only to be bound to a delegate, assumes ASC is not nullptr!
	virtual void SurgeMultiplierChanged(const FOnAttributeChangeData& Data);
	
	// Never call explicitly! Only to be bound to a delegate, assumes ASC is not nullptr!
	virtual void MaxSpeedChanged(const FOnAttributeChangeData& Data);

	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LastTrailPoint;

	const float DistanceThreshold = 5.f;

	UPROPERTY()
	class ABaseCharacter* BaseCharacterOwner;

	FGameplayAttribute SurgeSpeedMultiplierAttribute;

	FGameplayAttribute MaxSpeedAttribute;

	float CachedSurgeMultiplier;

	float CachedMaxSpeed;
};
