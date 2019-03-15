// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

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

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::ReceivePlayerDamage(float _damage)
{
	// Call the blueprint event
	Receive_ReceivePlayerDamage(_damage);
}

void AEnemyBase::EnemyTakeDamage(float _damage)
{
	ReceivePlayerDamage(_damage);

	if (CurrentHealth <= 0)
	{
		Death();
	}
}

void AEnemyBase::OnDeath()
{
	Receive_OnDeath();
}

void AEnemyBase::Death()
{
	OnDeath();
	K2_DestroyActor();
}
