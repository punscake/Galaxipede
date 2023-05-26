// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Components/CustomCharacterMovementComponent.h"
#include "Actors/BaseCharacter.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	//Set Max Walk Speed
	if (bRequestMaxFlySpeedChange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), CustomNewMaxFlySpeed));
		bRequestMaxFlySpeedChange = false;
		MaxFlySpeed = CustomNewMaxFlySpeed;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), MaxFlySpeed));
	}
}

bool UCustomCharacterMovementComponent::HandlePendingLaunch()
{
	if (!PendingLaunchVelocity.IsZero() && HasValidData())
	{
		Velocity = PendingLaunchVelocity;
		//Remmed out so our dodge move won't play the falling animation.
		//SetMovementMode(MOVE_Falling);
		PendingLaunchVelocity = FVector::ZeroVector;
		bForceNextFloorCheck = true;
		return true;
	}

	return false;
}


void UCustomCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMaxFlySpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UCustomCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	//check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UCustomCharacterMovementComponent* MutableThis = const_cast<UCustomCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Custom(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UCustomCharacterMovementComponent::FSavedMove_Custom::Clear()
{
	Super::Clear();

	bSavedRequestMaxFlySpeedChange = false;
}

uint8 UCustomCharacterMovementComponent::FSavedMove_Custom::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMaxFlySpeedChange)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool UCustomCharacterMovementComponent::FSavedMove_Custom::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.	
	if (bSavedRequestMaxFlySpeedChange != ((FSavedMove_Custom*)&NewMove)->bSavedRequestMaxFlySpeedChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UCustomCharacterMovementComponent::FSavedMove_Custom::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCustomCharacterMovementComponent* CharacterMovement = Cast<UCustomCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMaxFlySpeedChange = CharacterMovement->bRequestMaxFlySpeedChange;
	}
}

void UCustomCharacterMovementComponent::FSavedMove_Custom::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCustomCharacterMovementComponent* CharacterMovement = Cast<UCustomCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		
	}
}

UCustomCharacterMovementComponent::FNetworkPredictionData_Client_Custom::FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UCustomCharacterMovementComponent::FNetworkPredictionData_Client_Custom::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Custom());
}


//Set Max Walk Speed RPC to transfer the current Max Walk Speed from the Owning Client to the Server
bool UCustomCharacterMovementComponent::Server_SetMaxFlySpeed_Validate(const float NewMaxFlySpeed)
{
	if (NewMaxFlySpeed < 0.f || NewMaxFlySpeed > 2000.f)
		return false;
	else
		return true;
}

void UCustomCharacterMovementComponent::Server_SetMaxFlySpeed_Implementation(const float NewMaxFlySpeed)
{
	CustomNewMaxFlySpeed = NewMaxFlySpeed;
}

void UCustomCharacterMovementComponent::SetMaxFlySpeed(float NewMaxFlySpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		CustomNewMaxFlySpeed = NewMaxFlySpeed;
		Server_SetMaxFlySpeed(NewMaxFlySpeed);
	}

	bRequestMaxFlySpeedChange = true;
}