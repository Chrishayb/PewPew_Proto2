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

	// BlurprintImplementable event called when enemy takes damage
	virtual void TakeDamage(float _damage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "TakeDamage"))
	void ReceieveTakeDamage(float _damage);

};
