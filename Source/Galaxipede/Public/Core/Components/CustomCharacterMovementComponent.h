// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
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

	virtual bool HandlePendingLaunch() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity);

	//Set Max Fly Speed
	uint8 bRequestMaxFlySpeedChange : 1;

	UFUNCTION(Unreliable, Server, WithValidation)
		void Server_SetMaxFlySpeed(const float NewMaxFlySpeed);

	float CustomNewMaxFlySpeed;

	//Set Max Fly Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "Max Fly Speed")
		void SetMaxFlySpeed(float NewMaxFlySpeed);
};
