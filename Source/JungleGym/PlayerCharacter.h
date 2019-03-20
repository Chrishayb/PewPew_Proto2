// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamePlayDelegate);

USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float WalkableAngle;

	UPROPERTY(EditDefaultsOnly)
	float StepHeight;
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class USceneComponent* GunShootingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PostProcess")
	class UPostProcessComponent* PlayerPostProcess;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FMovementData DefaultMovementData;

	// Values that sets the turnning speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Camera")
	float BaseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Camera")
	float BaseLookupRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	bool bAbleToSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintFOVMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	class UMaterialInterface* SpeedLineMaterial;
	class UMaterialInstanceDynamic* SpeedLineInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement: Sprint")
	float HydrationDrainPerSecOnSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float BaseMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float DefaultBaseMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float DefaultSprintMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	bool bSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float BaseFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement: Sprint")
	float SprintingFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: General")
	float MaxEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: General")
	float CurrentEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: General")
	float EnergyDrainPerSec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: General")
	float MaxHydration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: General")
	float CurrentHydration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: General")
	bool bDeHydrated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: Shooting")
	bool bRapidFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: Shooting")
	float OverheatMax;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: Shooting")
	float OverheatCurrent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: Shooting")
	float CoolDownRatePerSec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: Shooting")
	float ForceCoolDownMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: Shooting")
	bool bCoolingDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat: Shooting")
	bool bInForceCoolDown;

	// Handle to regular cool down time
	FTimerHandle CoolDownInit_Handle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat: WorldToggle")
	bool bInImagineWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat: Pinecone")
	TArray<TSubclassOf<class APinecone>> PineconeTemplates;
	
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	class APortalDefenseGameMode* PDGamemode;


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

	/** General Combat */

		// Check hydration level
	void CheckHydrationLevel();
		// Blueprint event for dyehydration
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat", meta = (DisplayName = "OnDehydration"))
	void Receive_OnDehydration();
		// Check energy level (Death)
	void CheckEnergyLevel();

	/** Shooting machanic */

		// Check if the player is capable of shooting
	bool bPlayerCanShoot();
		// Called when successfully shoot, spawn raycast and test to see if hit
	UFUNCTION(BlueprintCallable)
	void FireWeapon();
		// StopRapidFire
	void EndRapidFire();
		// Called after PerformFiring() to increase the overheat percentage
	void OverHeatWeapon(float _value);
		// Called when cool down is finished which creats cool down chain until timer gets destroyed
	void CoolDownInit();

	/** Piencone(Grenade) Mechanic */

		// Throw the pinecone!!!!!!!!!!!!!
	UFUNCTION(BlueprintCallable)
	void ThrowPinecone(FVector _spawnLocation, FRotator _spawnDirection, float _force);

	/** Reality switch mechanic */

		// Toggle the dimension of imagination and reality
	void RealityToggle();
		// Called when swap to real world
	UFUNCTION()
	void SwapToRealWorld();
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay", meta = (DisplayName = "OnSwapToRealWorld"))
	void Recevie_OnSwapToRealWorld();
		// Called when swap to imagine world
	UFUNCTION()
	void SwapToImagineWorld();
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay", meta = (DisplayName = "OnSwapToImagineWorld"))
	void Recevie_OnSwapToImagineWorld();


	/** Animation Implementation for blueprint */

		// Shooting animation
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation", meta = (DisplayName = "PlayerFire"))
	void Receive_OnPlayerFire();

private:

	// Called in BeginPlay() to set the default value
	void SetDefaultMovementValue();

	// Called in tick to drain energy
	void DrainEnergy(float _deltaTime);

	// Called in tick to interp the FOV to its desire
	void SprintEffect(float _deltaTime);

	// Called in tick to rapid fire weapon
	void RapidFire();

	// Called in tick to cool down the weapon
	void CoolDown(float _deltaTime);

	// Called in tick to check what player is standing on
	void CheckGround();

	// Called in tick to Finger Glow!!!!
	void GlowyFingersLOL();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** General Combat */

		// Dehydrate the player
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DehydrateByValue(float _value);
		// Hydrate the player
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HydratingByValue(float _value);
		// Reduce the energy(health) of the player
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnergyDropByValue(float _value);
		// Increase the energy(health) of the player
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnergyGainByValue(float _value);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BoostSpeed();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DefaultSpeed();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FGamePlayDelegate PlayerDeath;

};
