// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerCharacterAnimInstance.h"
#include "MultiplayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMultiplayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<AMultiplayerCharacter>(TryGetPawnOwner());
}

void UMultiplayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter = Cast<AMultiplayerCharacter>(TryGetPawnOwner());
	}

	if (PlayerCharacter == nullptr) return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	Speed = Velocity.Length();

	bInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0 ? true : false;
	bWeaponEquipped = PlayerCharacter->IsWeaponEquipped();
	bIsCrouched = PlayerCharacter->bIsCrouched;
	bIsAiming = PlayerCharacter->IsAiming();
}
