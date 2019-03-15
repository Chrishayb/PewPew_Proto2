// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class JUNGLEGYM_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	// BlurprintImplementable event called when enemy takes damage
	virtual void ReceivePlayerDamage(float _damage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "TakeDamage"))
	void Receive_ReceivePlayerDamage(float _damage);
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnemyTakeDamage(float _damage);

	virtual void OnDeath();
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "OnDeath"))
	void Receive_OnDeath();

	void Death();
};
