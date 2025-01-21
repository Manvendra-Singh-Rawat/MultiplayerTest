// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMultiplayerCharacter::AMultiplayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMultiplayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMultiplayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMultiplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRIght", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AMultiplayerCharacter::MoveForward(float Value)
{
	if (Controller == nullptr && Value == 0.0f)
	{
		return;
	}

	FRotator ControlRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);
}

void AMultiplayerCharacter::MoveRight(float Value)
{
	if (Controller == nullptr && Value == 0.0f)
	{
		return;
	}

	FRotator ControlRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Value);
}

void AMultiplayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMultiplayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMultiplayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMultiplayerCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void AMultiplayerCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon != nullptr)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void AMultiplayerCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeaponUsed)
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeaponUsed != nullptr)
	{
		LastWeaponUsed->ShowPickupWidget(false);
	}
}
