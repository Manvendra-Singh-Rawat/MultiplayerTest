#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMultiplayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Character Defaults")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Character Defaults")
	class UCameraComponent* Camera;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleANywhere, Category = "Weapon Properties")
	class UCombatComponent* Combat;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void EquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetOverlappingWeapon(AWeapon* Weapon);
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeaponUsed);

	bool IsWeaponEquipped();
};
