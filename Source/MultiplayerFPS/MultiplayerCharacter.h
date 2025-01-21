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

private:
	UPROPERTY(VisibleAnywhere, Category = "Character Defaults")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Character Defaults")
	class UCameraComponent* Camera;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	//UFUNCTION()
	//void OnRep_OverlappingWeapon(AWeapon* LastWeaponOverlapped);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetOverlappingWeapon(AWeapon* Weapon);
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeaponUsed);
};
