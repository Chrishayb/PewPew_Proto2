// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"

#include "WeaponDataBase.h"

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
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::WeaponPerformFiring(FTransform _muzzleWorldLocation)
{
	bReadyToShoot = false;
	GetWorld()->GetTimerManager().SetTimer(
		FireRateCountDownTimerHandle, 
		this, 
		&UWeaponComponent::OnFireRateCountDownFinish, 
		CoolDownBetweenShot);

	FHitResult weaponHitResult;
	FVector hitStart = _muzzleWorldLocation.GetLocation();
	FVector hitEnd = hitStart + (_muzzleWorldLocation.GetRotation().GetForwardVector() * WeaponMaxRange);
	FCollisionObjectQueryParams queryObjectTypes;
	queryObjectTypes.AddObjectTypesToQuery(ECC_Pawn);
	queryObjectTypes.AddObjectTypesToQuery(ECC_WorldStatic);
	queryObjectTypes.AddObjectTypesToQuery(ECC_PhysicsBody);
	
	GetWorld()->LineTraceSingleByObjectType(weaponHitResult, hitStart, hitEnd, queryObjectTypes);

}

void UWeaponComponent::WeaponDataResetAndCalculate()
{
	if (WeaponDataPreset)
	{
		WeaponBaseDamage = WeaponDataPreset->WeaponBaseDamage;

	}
	else
	{

	}
}

void UWeaponComponent::OnFireRateCountDownFinish()
{
	bReadyToShoot = true;
}

