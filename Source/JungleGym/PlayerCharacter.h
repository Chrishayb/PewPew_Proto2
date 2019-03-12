// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class JUNGLEGYM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** FPS camera component for the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FPSCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunPoint")
	class USceneComponent* GunShootingPoint;

protected:

	// Values that sets the turnning speed
	float BaseTurnRate;
	float BaseLookupRate;

	// This value gets the default MaxWalkSpeed from movement component and remember it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float BaseMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintFOVMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintFOVZoomSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float BaseFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float DesireFOV;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: Shooting")
	float BaseDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the player move forward and backward
	void MoveForward(float _value);

	// Called when the player move left and right
	void MoveRight(float _value);

	// FPS Camera turnning
		// For Controller
	void TurnAtRate(float _value);
	void LookUpAtRate(float _value);
		// For Mouse
	void Turn(float _value);
	void LookUp(float _value);

	// Player sprint and un-sprint
	void Sprint();
	void UnSprint();

private:

	// Called in tick to interp the FOV to its desire
	void UpdateFOV(float _deltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
