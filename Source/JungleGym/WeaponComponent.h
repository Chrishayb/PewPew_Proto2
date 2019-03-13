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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon: Preset")
	class UWeaponDataBase* WeaponDataPreset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon: Stats")
	float WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon: Stats")
	float WeaponRPM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon: Stats")
	float WeaponMaxRange;

	// Timer to handle fire rate count down
	FTimerHandle FireRateCountDownTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	bool bReadyToShoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon: Stats Monitor")
	float CoolDownBetweenShot;


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

	// Called to return the state of the gun to see if its able to shoot
	UFUNCTION()
	FORCEINLINE bool IsReadyToShoot() { return bReadyToShoot; }

	// Called when the player is performing fire with this weapon
	UFUNCTION()
	void WeaponPerformFiring(FTransform _muzzleWorldLocation);
	
};
