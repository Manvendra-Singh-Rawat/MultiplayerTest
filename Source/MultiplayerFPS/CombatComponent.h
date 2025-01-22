#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERFPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	friend class AMultiplayerCharacter;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AMultiplayerCharacter* Character;
	UPROPERTY(Replicated)
	class AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming{ false };

public:
	void EquipWeapon(AWeapon* WeaponToEquip);
	void SetIsAiming(bool Value);

	UFUNCTION(Server, Reliable)
	void ServerSetIsAiming(bool Value);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
