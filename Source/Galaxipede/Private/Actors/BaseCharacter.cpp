// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAbilitiesInitialized = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("MovementAttributeSet"));
	LevelAttributeSet = CreateDefaultSubobject<ULevelAttributeSet>(TEXT("LevelAttributeSet"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AddStartingGameplayEffects();
}

void ABaseCharacter::BindControls()
{
	if (AbilitySystemComponent && InputComponent)
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/Galaxipede"), FName("EGalaxipedeAbilityInputID"));
		const FGameplayAbilityInputBinds Binds(
			FString("Confirm"),
			FString("Cancel"),
			AbilityEnumAssetPath,
			static_cast<int32>(EGalaxipedeAbilityInputID::Confirm),
			static_cast<int32>(EGalaxipedeAbilityInputID::Cancel)
		);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

	}
}

void ABaseCharacter::AddStartingGameplayAbilities()
{
	if (!bAbilitiesInitialized && AbilitySystemComponent && HasAuthority())
	{
		for (TSubclassOf<UMyGameplayAbility>& StartingAbility : StartingGameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartingAbility,
				1,
				static_cast<int32>(StartingAbility.GetDefaultObject()->AbilityInputID),
				this
			));
		}
		bAbilitiesInitialized = true;
	}
}

void ABaseCharacter::AddStartingGameplayEffects()
{
	if (AbilitySystemComponent && HasAuthority())
	{
		for (TSubclassOf<UGameplayEffect>& StartingEffect : StartingGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StartingEffect, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
}

bool ABaseCharacter::AttemptAttachSegments(ABaseCharacter* Segment)
{
	if (!bCanBeAddedAsSegment || !Segment->bCanBeAddedAsSegment || !Controller)
		return false;
	ABaseCharacter* Head = Segment->GetHeadSegment();
	if (Head->Controller)
		return false;
	Segment->PreviousSegment = this;
	NextSegment = Segment;
	AbilitySystemComponent->CancelAllAbilities();
	AbilitySystemComponent->ClearAllAbilities(); //TODO replace with tag
	AController* C = Controller;
	C->UnPossess();
	C->Possess(Head);
	return true;
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCustomCharacterMovementComponent* ABaseCharacter::GetCustomCharacterMovementComponent() const
{
	return CustomCharacterMovementComponent;
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(Super::GetMovementComponent());
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindControls();
}

ABaseCharacter* ABaseCharacter::GetNextSegment()
{
	return NextSegment;
}

ABaseCharacter* ABaseCharacter::GetPreviousSegment()
{
	return PreviousSegment;
}

ABaseCharacter* ABaseCharacter::GetHeadSegment()
{
	ABaseCharacter* Segment = NextSegment;
	ABaseCharacter* Head = this;
	while (Segment)
	{
		Head = Segment;
		Segment = Segment->GetNextSegment();
	}
	return Head;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Initialize GAS on server
	if (AbilitySystemComponent)
	{
		AddStartingGameplayAbilities();
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Initialize GAS on client
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	BindControls();
}

