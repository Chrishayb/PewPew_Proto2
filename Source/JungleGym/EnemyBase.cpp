// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	CanMove = true;
}

void AEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::ReceivePlayerDamage(float _damage)
{
	// Call the blueprint event
	Receive_ReceivePlayerDamage(_damage);

	if (CurrentHealth <= 0)
	{
		EnemyDeath();
	}
}

void AEnemyBase::EnemyDeath()
{
	Receive_OnDeath();
}

void AEnemyBase::GravityGrenadeEffectByDuration(float _durartion)
{
	CanMove = false;
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetEnableGravity(false);

	FTimerHandle endEffectHandle;
	GetWorld()->GetTimerManager().SetTimer(endEffectHandle, this, &AEnemyBase::EndGravityGrenadeEffect, _durartion);

	Receive_GravityGrenadeEffect(_durartion);
}

void AEnemyBase::EndGravityGrenadeEffect()
{
	CanMove = true;
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetEnableGravity(true);
	SetActorRelativeRotation(DefaultRotation);

	Receive_EndGravityGrenadeEffect();
}
