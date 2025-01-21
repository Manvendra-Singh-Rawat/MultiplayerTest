// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Net/UnrealNetwork.h"
#include "CombatComponent.h"

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

	// Components are not needed to be registered for replication
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Comp"));
	Combat->SetIsReplicated(true);

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

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRIght", this, &ThisClass::MoveRight);

	// Looking
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);

	// Jump action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Equip
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ThisClass::EquipButtonPressed);
	//PlayerInputComponent->BindAction("Equip", IE_Released, this, &ThisClass::);
}

void AMultiplayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat != nullptr)
	{
		Combat->Character = this;
	}
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

void AMultiplayerCharacter::EquipButtonPressed()
{
	if (Combat != nullptr)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void AMultiplayerCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat != nullptr)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
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

void AMultiplayerCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeaponOverlapped)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OnRep_OverlappingWeapon() called")));
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeaponOverlapped != nullptr)
	{
		LastWeaponOverlapped->ShowPickupWidget(false);
	}
}

bool AMultiplayerCharacter::IsWeaponEquipped()
{
	return (Combat != nullptr && Combat->EquippedWeapon != nullptr) ? true : false;
}
