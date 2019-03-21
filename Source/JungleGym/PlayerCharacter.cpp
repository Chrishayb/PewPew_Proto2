// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PostProcessComponent.h"
#include "WeaponComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "PortalDefenseGameMode.h"
#include "EnemyBase.h"
#include "Pinecone.h"

#include "Engine.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default value
	BaseTurnRate = 45.f;
	BaseLookupRate = 45.f;

	// Setup component hierachy
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(FPSCamera);

	GunShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GunShootingPoint"));
	GunShootingPoint->SetupAttachment(GetMesh(), TEXT("MuzzlePoint"));

	PlayerPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PlayerPostProcess"));
	PlayerPostProcess->SetupAttachment(RootComponent);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get essential stuff
	PDGamemode = Cast<APortalDefenseGameMode>(UGameplayStatics::GetGameMode(this));
	if (PDGamemode)
	{
		PDGamemode->OnToggleToRealWorld.AddDynamic(this, &APlayerCharacter::SwapToRealWorld);
		PDGamemode->OnToggleToImagineWorld.AddDynamic(this, &APlayerCharacter::SwapToImagineWorld);
	}

	// Create crosshair
	CrosshairWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), CrosshairWidgetClass);
	if (CrosshairWidget)
		CrosshairWidget->AddToViewport();

	// Set default values
	SetDefaultMovementValue();
	BaseFOV = FPSCamera->FieldOfView;
	SprintingFOV = BaseFOV * SprintFOVMultiplier;
	bSprinting = false;
	bRapidFire = false;
	OverheatCurrent = 0.0f;
	bInForceCoolDown = false;
	bCoolingDown = false;
	DefaultBaseMaxWalkSpeed = BaseMaxWalkSpeed;
	DefaultSprintMaxWalkSpeed = SprintMaxWalkSpeed;

	CurrentHydration = MaxHydration;
	CurrentEnergy = MaxEnergy;

	// Create speed line material instance
	SpeedLineInstance = UMaterialInstanceDynamic::Create(SpeedLineMaterial, this);
	PlayerPostProcess->AddOrUpdateBlendable(SpeedLineInstance, 1.0f);
}

void APlayerCharacter::MoveForward(float _value)
{
	if ((Controller != NULL) && (_value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, _value);
	}

	// If detected player is not moving forward, cancel the sprint
	if (_value <= 0.0f)
	{
		UnSprint();
	}
}

void APlayerCharacter::MoveRight(float _value)
{
	if ((Controller != nullptr) && (_value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, _value);
	}
}

void APlayerCharacter::TurnAtRate(float _value)
{
	if (_value != 0.0f)
	{
		float turnValue = BaseTurnRate * _value * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddControllerYawInput(turnValue);
	}
}

void APlayerCharacter::LookUpAtRate(float _value)
{
	if (_value != 0.0f)
	{
		float turnValue = BaseLookupRate * _value * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddControllerPitchInput(turnValue);
	}
}

void APlayerCharacter::Turn(float _value)
{
	if (_value != 0.0f)
	{
		AddControllerYawInput(_value);
	}
}

void APlayerCharacter::LookUp(float _value)
{
	if(_value != 0.0f)
	{
		AddControllerPitchInput(_value);
	}
}

void APlayerCharacter::Sprint()
{
	// Give the speed a multiplier base on the original max speed
	float axisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	if (axisValue >= 0.0f && bAbleToSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMaxWalkSpeed;
		bSprinting = true;
		bRapidFire = false;
	}
}

void APlayerCharacter::UnSprint()
{
	// Reset the max speed to original
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;
	bSprinting = false;
}

void APlayerCharacter::BoostSpeed() {
	SprintMaxWalkSpeed = SprintMaxWalkSpeed * 3;
	BaseMaxWalkSpeed = BaseMaxWalkSpeed * 5;
	FTimerHandle endEffectHandle;
	GetWorld()->GetTimerManager().SetTimer(endEffectHandle, this, &APlayerCharacter::DefaultSpeed, 10);
}

void APlayerCharacter::DefaultSpeed() {
	SprintMaxWalkSpeed = DefaultSprintMaxWalkSpeed;
	BaseMaxWalkSpeed = DefaultBaseMaxWalkSpeed;
}

void APlayerCharacter::DehydrateByValue(float _value)
{
	CurrentHydration = FMath::Max(CurrentHydration - _value, 0.0f);
	if (CurrentHydration <= 0.0f)
	{
		// Force player to switch to real world if dehydrated
		if (APortalDefenseGameMode* PDGamemode = Cast<APortalDefenseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (!PDGamemode->bInRealWorld)
			{
				PDGamemode->SwapToRealWorld();
			}
		}

		Receive_OnDehydration();
	}

	CheckHydrationLevel();
}

void APlayerCharacter::HydratingByValue(float _value)
{
	CurrentHydration = FMath::Min(CurrentHydration + _value, MaxHydration);
	CheckHydrationLevel();
}

void APlayerCharacter::CheckHydrationLevel()
{
	PewVolModifier = FMath::Min(CurrentHydration / (MaxHydration * 0.2f), 1.0f);
	PewVolModifier = PewVolModifier * (MaxPewVol - MinPewVol) + MinPewVol;

	if (CurrentHydration <= 0.0f)
	{
		UnSprint();

		bDeHydrated = true;
		bAbleToSprint = false;
	}
	else
	{
		bDeHydrated = false;
		bAbleToSprint = true;
	}
}

void APlayerCharacter::CheckEnergyLevel()
{
	if (CurrentEnergy <= 0.0f)
	{
		PlayerDeath.Broadcast();
	}
}

void APlayerCharacter::EnergyDropByValue(float _value)
{
	CurrentEnergy = FMath::Max(CurrentEnergy - _value, 0.0f);
	CheckEnergyLevel();
}

void APlayerCharacter::EnergyGainByValue(float _value)
{
	CurrentEnergy = FMath::Min(CurrentEnergy + _value, MaxEnergy);
	CheckEnergyLevel();
}

bool APlayerCharacter::bPlayerCanShoot()
{
	if (bInForceCoolDown || bSprinting || bDeHydrated || !bInImagineWorld)
	{
		bRapidFire = false;
		return false;
	}

	return true;
}

void APlayerCharacter::FireWeapon()
{
	if (bPlayerCanShoot() && WeaponComponent->IsReadyToShoot())
	{
		bRapidFire = true;

		WeaponComponent->WeaponPerformFiring(
			this,
			FPSCamera->GetComponentTransform(), 
			GunShootingPoint->GetComponentLocation());

		GetWorld()->GetTimerManager().SetTimer(
			CoolDownInit_Handle,
			this,
			&APlayerCharacter::CoolDownInit,
			WeaponComponent->GetCoolDownBetweenShot() * 2);
		bCoolingDown = false;

		//OverHeatWeapon(WeaponComponent->GetOverheatRate());
		DehydrateByValue(WeaponComponent->GetHydrationDrainRate());

		// Plays the animation montage and sound in BP
		Receive_OnPlayerFire();
	}
}

void APlayerCharacter::EndRapidFire()
{
	bRapidFire = false;
}

void APlayerCharacter::OverHeatWeapon(float _value)
{
	OverheatCurrent = FMath::Min(OverheatCurrent + _value, OverheatMax);
	if (OverheatCurrent == OverheatMax)
	{
		//GetWorld()->GetTimerManager().ClearTimer(CoolDownInit_Handle);
		bInForceCoolDown = true;
	}
}

void APlayerCharacter::CoolDownInit()
{
	bCoolingDown = true;
}

void APlayerCharacter::ThrowPinecone(FVector _spawnLocation, FRotator _spawnDirection, float _force)
{
	if (PineconeCount <= 0)
	{
		return;
	}
	APinecone* pinecone;
	pinecone = GetWorld()->SpawnActor<APinecone>(PineconeTemplates[0], _spawnLocation, _spawnDirection);
	pinecone->SphereCollider->AddImpulse(pinecone->GetActorForwardVector() * _force);
	pinecone->InitPineconeDetonation();
	PineconeCount--;
}

void APlayerCharacter::RealityToggle()
{
	if (PDGamemode)
	{
		PDGamemode->ToggleWorld();
	}
}

void APlayerCharacter::SwapToRealWorld()
{
	Recevie_OnSwapToRealWorld();

}

void APlayerCharacter::SwapToImagineWorld()
{
	Recevie_OnSwapToImagineWorld();

}

void APlayerCharacter::SetDefaultMovementValue()
{
	UCharacterMovementComponent* movementComp = GetCharacterMovement();
	movementComp->MaxWalkSpeed = DefaultMovementData.WalkSpeed;
	BaseMaxWalkSpeed = DefaultMovementData.WalkSpeed;
	SprintMaxWalkSpeed = BaseMaxWalkSpeed * SprintMultiplier;
	bAbleToSprint = true;
	movementComp->SetWalkableFloorAngle(DefaultMovementData.WalkableAngle);
	movementComp->MaxStepHeight = DefaultMovementData.StepHeight;
}

void APlayerCharacter::DrainEnergy(float _deltaTime)
{
	EnergyDropByValue(EnergyDrainPerSec * _deltaTime);
}

void APlayerCharacter::SprintEffect(float _deltaTime)
{
	// Speed line and FOV change
	float currentMoveSpeed = (GetCharacterMovement()->Velocity * GetActorForwardVector()).Size();
	float speedDiffWalkSprint = SprintMaxWalkSpeed - BaseMaxWalkSpeed;
	float clampValue = (currentMoveSpeed - BaseMaxWalkSpeed) / (speedDiffWalkSprint);
	float alphaClampResult = FMath::Clamp(clampValue, 0.0f, 1.0f);
	SpeedLineInstance->SetScalarParameterValue(FName("Weight"), alphaClampResult);
	FPSCamera->SetFieldOfView(FMath::Lerp(BaseFOV, SprintingFOV, alphaClampResult));

	// Dehydration
	if (bSprinting)
	{
		DehydrateByValue(HydrationDrainPerSecOnSprint * _deltaTime);
	}
}

void APlayerCharacter::RapidFire()
{
	if (bRapidFire)
	{
		FireWeapon();
	}
}

void APlayerCharacter::CoolDown(float _deltaTime)
{
	if (bInForceCoolDown)
	{
		OverheatCurrent = FMath::Max(OverheatCurrent - (_deltaTime * (CoolDownRatePerSec * ForceCoolDownMultiplier)), 0.0f);
	}
	else if (bInForceCoolDown || bCoolingDown)
	{
		OverheatCurrent = FMath::Max(OverheatCurrent - (_deltaTime * CoolDownRatePerSec), 0.0f);	
	}

	if (OverheatCurrent == 0.0f)
	{
		bCoolingDown = false;
		bInForceCoolDown = false;
	}
}

void APlayerCharacter::CheckGround()
{
	TArray<FName> groundCompTag;
	UPrimitiveComponent* groundComponent = GetCharacterMovement()->GetMovementBase();
	if (groundComponent)
	{
		if (groundComponent->ComponentHasTag(TEXT("Sand")))
		{
			UnSprint();
			bAbleToSprint = false;
		}
		else
		{
			bAbleToSprint = true;
			CheckHydrationLevel();
		}

		if (groundComponent->ComponentHasTag(TEXT("Slide")))
		{
			GetCharacterMovement()->SetWalkableFloorAngle(0.0f);
		}
		else GetCharacterMovement()->SetWalkableFloorAngle(DefaultMovementData.WalkableAngle);
	}
}

void APlayerCharacter::GlowyFingersLOL()
{
	float paramValue = OverheatCurrent / OverheatMax * 300.0f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Burn"), paramValue);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// De-activate for now
	// Drain Energy
	//DrainEnergy(DeltaTime);

	// Update the FOV for the player
	SprintEffect(DeltaTime);

	// Update movement based on 
	CheckGround();

	// Weapon auto shoot
	RapidFire();

	// Check and cool down the weapon
	CoolDown(DeltaTime);

	// Glow!!!!
	GlowyFingersLOL();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::UnSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::EndRapidFire);

	//PlayerInputComponent->BindAction("RealityToggle", IE_Pressed, this, &APlayerCharacter::RealityToggle);

}

void APlayerCharacter::UpBroccoliCount(int _value) 
{
	iBrocolliCollected += _value;
}

void APlayerCharacter::UpMushroomCount(int _value) 
{
	iMushroomCollected += _value;
}

void APlayerCharacter::UpTomatoCount(int _value) 
{
	iTomatoCollected += _value;
}

void APlayerCharacter::UpPinecone(int _value)
{
	PineconeCount += _value;
}
