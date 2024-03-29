// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Components/CustomCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/BaseCharacter.h"
#include "DrawDebugHelpers.h"
UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CachedSurgeMultiplier = 0.f;
	DistanceBetweenSegments = 150.f;
}


void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseCharacterOwner = Cast<ABaseCharacter>(GetOwner());
	LastTrailPoint = GetActorLocation();

	// Subscribes to movement attribute set's speed multiplier for more accurate validation
	if (const IAbilitySystemInterface* OwnerWithAttributes = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		ASC = OwnerWithAttributes->GetAbilitySystemComponent();

		if (const UMovementAttributeSet* MovementAttributeSet = Cast<UMovementAttributeSet>(ASC->GetAttributeSet(UMovementAttributeSet::StaticClass())))
		{
			SurgeSpeedMultiplierAttribute = MovementAttributeSet->GetSurgeSpeedMultiAttribute();
			ASC->GetGameplayAttributeValueChangeDelegate(SurgeSpeedMultiplierAttribute).AddUObject(this, &UCustomCharacterMovementComponent::SurgeMultiplierChanged);

			MaxSpeedAttribute = MovementAttributeSet->GetMaxSpeedAttribute();
			ASC->GetGameplayAttributeValueChangeDelegate(MaxSpeedAttribute).AddUObject(this, &UCustomCharacterMovementComponent::MaxSpeedChanged);
		}
	}
}

void UCustomCharacterMovementComponent::SurgeMultiplierChanged(const FOnAttributeChangeData& Data)
{
	CachedSurgeMultiplier = ASC->GetNumericAttribute(SurgeSpeedMultiplierAttribute);
}

FTrail UCustomCharacterMovementComponent::UpdateTrail(float Distance)
{
	if (!bRunTrailLogic || !PawnOwner->HasAuthority() || TrailQueue.IsEmpty())
		return;
	ABaseCharacter* PrevSegment = BaseCharacterOwner->GetPreviousSegment();
	UCustomCharacterMovementComponent* PrevSegmentCMC = nullptr;
	if (PrevSegment)
		PrevSegmentCMC = PrevSegment->GetCustomCharacterMovementComponent();
	TrailQueue.
	FTrail CurrentPoint = FTrail(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), true);
	FTrail NextPoint = FTrail();
	TrailQueue.Dequeue(NextPoint);
	float DistanceToNextPoint;
	float DistanceToTravel = Distance;
	do
	{
		DistanceToNextPoint = FVector::Dist(CurrentPoint.Location, NextPoint.Location);
		if (NextPoint.bTeleportLogic)
		{
			PrevSegmentCMC ? PrevSegmentCMC->TrailQueue.Enqueue(NextPoint) : false;
			CurrentPoint = NextPoint;
			TrailQueue.Pop();
		}
		else if (DistanceToTravel > DistanceToNextPoint)
		{
			PrevSegmentCMC ? PrevSegmentCMC->TrailQueue.Enqueue(NextPoint) : false;
			DistanceToTravel -= DistanceToNextPoint;
			CurrentPoint = NextPoint;
			TrailQueue.Pop();
		}
		else
		{
			CurrentPoint.Location = UKismetMathLibrary::VLerp(CurrentPoint.Location, NextPoint.Location, DistanceToTravel / DistanceToNextPoint);
			CurrentPoint.bTeleportLogic = false;
			break;
		}
	} while (TrailQueue.Peek(NextPoint));

	GetOwner()->TeleportTo(CurrentPoint.Location, CurrentPoint.Rotation);

	if (PrevSegmentCMC) {
		PrevSegmentCMC->TrailQueue.Enqueue(CurrentPoint);
		PrevSegmentCMC->TravelAlongTrail(Distance);
	}
}

void UCustomCharacterMovementComponent::TravelAlongTrail(float Distance)
{
	if (!bRunTrailLogic || !PawnOwner->HasAuthority() || TrailQueue.IsEmpty())
		return;
	ABaseCharacter* PrevSegment = BaseCharacterOwner->GetPreviousSegment();
	UCustomCharacterMovementComponent* PrevSegmentCMC = nullptr;
	if (PrevSegment)
		PrevSegmentCMC = PrevSegment->GetCustomCharacterMovementComponent();
	
	FTrail CurrentPoint = FTrail(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), true);
	FTrail NextPoint = FTrail();
	TrailQueue.Dequeue(NextPoint);
	float DistanceToNextPoint;
	float DistanceToTravel = Distance;
	do
	{
		DistanceToNextPoint = FVector::Dist(CurrentPoint.Location, NextPoint.Location);
		if (NextPoint.bTeleportLogic)
		{
			PrevSegmentCMC ? PrevSegmentCMC->TrailQueue.Enqueue(NextPoint) : false;
			CurrentPoint = NextPoint;
			TrailQueue.Pop();
		}
		else if (DistanceToTravel > DistanceToNextPoint)
		{
			PrevSegmentCMC ? PrevSegmentCMC->TrailQueue.Enqueue(NextPoint) : false;
			DistanceToTravel -= DistanceToNextPoint;
			CurrentPoint = NextPoint;
			TrailQueue.Pop();
		}
		else
		{
			CurrentPoint.Location = UKismetMathLibrary::VLerp(CurrentPoint.Location, NextPoint.Location, DistanceToTravel / DistanceToNextPoint);
			CurrentPoint.bTeleportLogic = false;
			break;
		}
	}
	while (TrailQueue.Peek(NextPoint));

	GetOwner()->TeleportTo(CurrentPoint.Location, CurrentPoint.Rotation);

	if (PrevSegmentCMC) {
		PrevSegmentCMC->TrailQueue.Enqueue(CurrentPoint);
		PrevSegmentCMC->TravelAlongTrail(Distance);
	}
}

void UCustomCharacterMovementComponent::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxSpeed = ASC->GetNumericAttribute(MaxSpeedAttribute);
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, AActor::GetDebugName(BaseCharacterOwner->GetPreviousSegment()));

	if (bRunTrailLogic && PawnOwner->HasAuthority())
	{
		if (ABaseCharacter* PrevSegment = BaseCharacterOwner->GetPreviousSegment())
		{
			float DistanceMoved = FVector::Dist(LastTrailPoint, BaseCharacterOwner->GetActorLocation());
			if (DistanceMoved > DistanceThreshold)
			{
				LastTrailPoint = BaseCharacterOwner->GetActorLocation();
				FTrail Point = FTrail(LastTrailPoint, BaseCharacterOwner->GetActorRotation(), bTeleportMovement);
				PrevSegment->GetCustomCharacterMovementComponent()->TrailQueue.Enqueue(Point);
				PrevSegment->GetCustomCharacterMovementComponent()->TravelAlongTrail(DistanceMoved);
			}
		}
		else
		{
			LastTrailPoint = BaseCharacterOwner->GetActorLocation();
		}
	}

	if (!CharacterOwner)
	{
		return;
	}

	//Set Max Walk Speed
	if (bRequestMaxFlySpeedChange)
	{
		bRequestMaxFlySpeedChange = false;
		MaxFlySpeed = CustomNewMaxFlySpeed;
	}
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
/* SKIP VALIDATION, AS MOVE SPEED LIMIT IS DYNAMIC AND THE CLIENT MAY NOT BE AWARE OF RECENT CHANGES
bool UCustomCharacterMovementComponent::Server_SetMaxFlySpeed_Validate(const float NewMaxFlySpeed)
{
	return true;
	float UpperFlySpeedLimit = 2000.f;
	if (const IAbilitySystemInterface* OwnerWithAttributes = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (const ULevelAttributeSet * AS = Cast<ULevelAttributeSet>(OwnerWithAttributes->GetAbilitySystemComponent()->GetAttributeSet(ULevelAttributeSet::StaticClass())))
		{
			UpperFlySpeedLimit = OwnerWithAttributes->GetAbilitySystemComponent()->GetNumericAttribute(AS->GetLevelAttribute());
		}

	}
	if (NewMaxFlySpeed < 0.f || NewMaxFlySpeed > 2000.f) {
		return false;}
	else
		return true;
}
*/

// Instead clamp speed on server
void UCustomCharacterMovementComponent::Server_SetMaxFlySpeed_Implementation(const float NewMaxFlySpeed)
{
	CustomNewMaxFlySpeed = FMath::Clamp(NewMaxFlySpeed, 0.f, CachedMaxSpeed * CachedSurgeMultiplier);
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