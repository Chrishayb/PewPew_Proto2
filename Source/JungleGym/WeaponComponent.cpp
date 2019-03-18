// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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

void UWeaponComponent::WeaponPerformFiring(APawn* _weaponUser, FTransform _cameraTransform, FVector _muzzleLocation)
{
	// Cooldown system init
	bReadyToShoot = false;
	GetWorld()->GetTimerManager().SetTimer(
		FireRateCountDownTimerHandle, 
		this, 
		&UWeaponComponent::OnFireRateCountDownFinish, 
		CoolDownBetweenShot);

	// Bullet(Hit scan)
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
			enemyGotHit->ReceivePlayerDamage(WeaponBaseDamage);
		}
	}
	FRotator traceDirection = (weaponHitResult.TraceEnd - _muzzleLocation).Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrace, _muzzleLocation, traceDirection);
	//DrawDebugLine(GetWorld(), _muzzleLocation, weaponHitResult.TraceEnd, FColor::Cyan, false, 2.0f, 0, 1.0f);

	// Recoil system
	float pitchRecoil = FMath::RandRange(PitchRecoilMin, PitchRecoilMax);
	float yawRecoil = FMath::RandRange(YawRecoilMin, YawRecoilMax);
	_weaponUser->AddControllerPitchInput(pitchRecoil);
	_weaponUser->AddControllerYawInput(yawRecoil);
}

void UWeaponComponent::WeaponDataResetAndCalculate()
{
	UWeaponDataBase* WeaponPresetData = WeaponDataPresetClass->GetDefaultObject<UWeaponDataBase>();
	if (WeaponPresetData)
	{
		WeaponBaseDamage = WeaponPresetData->WeaponBaseDamage;
		WeaponOverheatRate = WeaponPresetData->WeaponOverheatRate;
		HydrationDrainRate = WeaponPresetData->HydrationDrainRate;
		WeaponRPM = WeaponPresetData->WeaponRPM;
		WeaponMaxRange = WeaponPresetData->WeaponMaxRange;
		BulletTrace = WeaponPresetData->BulletTrace;
		PitchRecoilMin = WeaponPresetData->PitchRecoilMin;
		PitchRecoilMax = WeaponPresetData->PitchRecoilMax;
		YawRecoilMin = WeaponPresetData->YawRecoilMin;
		YawRecoilMax = WeaponPresetData->YawRecoilMax;

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

