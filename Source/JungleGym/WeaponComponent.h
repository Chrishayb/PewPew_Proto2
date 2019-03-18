// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JUNGLEGYM_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The Data of the weapon preseted in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon: Preset")
	TSubclassOf<class UWeaponDataBase> WeaponDataPresetClass;

	// Timer to handle fire rate count down
	FTimerHandle FireRateCountDownTimerHandle;

	// Data Monitor in run time
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	class UParticleSystem* BulletTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float WeaponBaseDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float WeaponRPM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float WeaponOverheatRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float HydrationDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float WeaponMaxRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	bool bReadyToShoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float CoolDownBetweenShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float PitchRecoilMin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float PitchRecoilMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float YawRecoilMin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float YawRecoilMax;


protected:

	// Called for reset the data according to the weapon data preset
	UFUNCTION()
	void WeaponDataResetAndCalculate();

	// Called after finish cool down
	UFUNCTION()
	void OnFireRateCountDownFinish();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the player is performing fire with this weapon
	UFUNCTION()
	void WeaponPerformFiring(APawn* _weaponUser, FTransform _cameraTransform, FVector _muzzleLocation);

	// Called to return the state of the gun to see if its able to shoot
	UFUNCTION()
	FORCEINLINE bool IsReadyToShoot() { return bReadyToShoot; }

	// Called to return the overheat rate of this weapon
	UFUNCTION()
	FORCEINLINE float GetOverheatRate() { return WeaponOverheatRate; }

	// Called to return the time of cool down between shot
	UFUNCTION()
	FORCEINLINE float GetCoolDownBetweenShot() { return CoolDownBetweenShot; }

	// Called to return the hydration drain rate of this weapon
	UFUNCTION()
	FORCEINLINE float GetHydrationDrainRate() { return HydrationDrainRate; }
};
