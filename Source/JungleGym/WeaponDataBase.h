// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataBase.generated.h"

/**
 * 
 */
UCLASS()
class JUNGLEGYM_API UWeaponDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponOverheatRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponRPM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponMaxRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* BulletTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PitchRecoilMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PitchRecoilMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YawRecoilMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YawRecoilMax;

};
