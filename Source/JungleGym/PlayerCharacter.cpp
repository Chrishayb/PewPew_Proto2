// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyBase.h"

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
	GunShootingPoint->SetupAttachment(GetMesh());



}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set default values
	BaseMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DesireFOV = BaseFOV = FPSCamera->FieldOfView;

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
	if (axisValue >= 0.0f)
	{
		float sprintSpeed = BaseMaxWalkSpeed * SprintMultiplier;
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
		DesireFOV = BaseFOV * SprintFOVMultiplier;
	}
}

void APlayerCharacter::UnSprint()
{
	// Reset the max speed to original
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;
	DesireFOV = BaseFOV;
}

void APlayerCharacter::UpdateFOV(float _deltaTime)
{
	float currentFOV = FPSCamera->FieldOfView;
	if (currentFOV < DesireFOV)
	{
		float resultFOV = FMath::Min(currentFOV + (_deltaTime * SprintFOVZoomSpeed), DesireFOV);
		FPSCamera->SetFieldOfView(resultFOV);
	}
	else if (currentFOV > DesireFOV)
	{
		float resultFOV = FMath::Max(currentFOV - (_deltaTime * SprintFOVZoomSpeed), DesireFOV);
		FPSCamera->SetFieldOfView(resultFOV);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the FOV for the player
	UpdateFOV(DeltaTime);

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

}

