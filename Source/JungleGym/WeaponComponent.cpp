// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

#include "WeaponDataBase.h"
#include "PlayerCharacter.h"
#include "EnemyBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	WeaponDataResetAndCalculate();
	bReadyToShoot = true;
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::WeaponPerformFiring(FTransform _cameraTransform, FVector _muzzleLocation)
{
	bReadyToShoot = false;
	GetWorld()->GetTimerManager().SetTimer(
		FireRateCountDownTimerHandle, 
		this, 
		&UWeaponComponent::OnFireRateCountDownFinish, 
		CoolDownBetweenShot);

	FHitResult weaponHitResult;
	FVector hitStart = _cameraTransform.GetLocation();
	FVector hitEnd = hitStart + (_cameraTransform.GetRotation().GetForwardVector() * WeaponMaxRange);

	FCollisionObjectQueryParams queryObjectTypes;
	queryObjectTypes.AddObjectTypesToQuery(ECC_Pawn);
	queryObjectTypes.AddObjectTypesToQuery(ECC_WorldStatic);
	queryObjectTypes.AddObjectTypesToQuery(ECC_WorldDynamic);
	queryObjectTypes.AddObjectTypesToQuery(ECC_PhysicsBody);
	bool bHitSomething = GetWorld()->LineTraceSingleByObjectType(weaponHitResult, hitStart, hitEnd, queryObjectTypes);
	if (bHitSomething)
	{
		// Check if the hit actor is a enemy
		if (AEnemyBase* enemyGotHit = Cast<AEnemyBase>(weaponHitResult.GetActor()))
		{
			enemyGotHit->EnemyTakeDamage(WeaponBaseDamage);
		}
	}
	DrawDebugLine(GetWorld(), _muzzleLocation, weaponHitResult.TraceEnd, FColor::Red, false, 3.0f, 0, 2.0f);
}

void UWeaponComponent::WeaponDataResetAndCalculate()
{
	UWeaponDataBase* WeaponPresetData = WeaponDataPresetClass->GetDefaultObject<UWeaponDataBase>();
	if (WeaponPresetData)
	{
		WeaponBaseDamage = WeaponPresetData->WeaponBaseDamage;
		WeaponOverheatRate = WeaponPresetData->WeaponOverheatRate;
		WeaponRPM = WeaponPresetData->WeaponRPM;
		WeaponMaxRange = WeaponPresetData->WeaponMaxRange;

		CoolDownBetweenShot = 60.0f / WeaponRPM;
	}
	else
	{

	}
}

void UWeaponComponent::OnFireRateCountDownFinish()
{
	bReadyToShoot = true;
}

