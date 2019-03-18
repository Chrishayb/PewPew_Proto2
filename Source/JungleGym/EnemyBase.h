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

	// Called when detroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool CanDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultValue")
	FRotator DefaultRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefaultValue")
	class UMaterial* DefaultMaterial;

	// BlurprintImplementable event called when enemy takes damage
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ReceivePlayerDamage(float _damage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "TakeDamage"))
	void Receive_ReceivePlayerDamage(float _damage);
	
	// BlurprintImplementable event called when enemny dies
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void EnemyDeath();
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "OnDeath"))
	void Receive_OnDeath();

	// BlurprintImplementable event called when enemny dies
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void GravityGrenadeEffectByDuration(float _durartion);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "GravityGrenadeEffect"))
	void Receive_GravityGrenadeEffect(float _durartion);

	// BlurprintImplementable event called when enemny dies
	UFUNCTION()
	virtual void EndGravityGrenadeEffect();
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "EndGravityGrenadeEffect"))
	void Receive_EndGravityGrenadeEffect();
};
