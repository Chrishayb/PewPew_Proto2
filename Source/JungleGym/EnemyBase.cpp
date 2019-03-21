// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/Material.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "PortalDefenseGameMode.h"

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
	
	APortalDefenseGameMode* PDGamemode = Cast<APortalDefenseGameMode>(UGameplayStatics::GetGameMode(this));
	if (PDGamemode)
	{
		PDGamemode->OnToggleToRealWorld.AddDynamic(this, &AEnemyBase::SwapToRealWorld);
		PDGamemode->OnToggleToImagineWorld.AddDynamic(this, &AEnemyBase::SwapToImagineWorld);

		if (PDGamemode->bInRealWorld)
		{
			SwapToRealWorld();
		}
		else SwapToImagineWorld();

	}

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

void AEnemyBase::SwapToRealWorld()
{
	GetMesh()->SetMaterial(0, RealWorldSeeThroughMat);
}

void AEnemyBase::SwapToImagineWorld()
{
	GetMesh()->SetMaterial(0, DefaultMaterial);
}

void AEnemyBase::ChangeTarget(AActor* _newTarget)
{
	Receive_ChangeTarget(_newTarget);
}

void AEnemyBase::ChangeToImaginarySpeed()
{
	Recieve_ChangeToImaginarySpeed();
}

void AEnemyBase::ChangeToRealSpeed()
{
	Recieve_ChangeToRealSpeed();
}

void AEnemyBase::SetWorldMoveSpeeds(float _newSpeed)
{
	ImaginaryMoveSpeed = _newSpeed;
	RealMoveSpeed = _newSpeed / 2;
}
