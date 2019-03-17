// Fill out your copyright notice in the Description page of Project Settings.

#include "Pinecone.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
APinecone::APinecone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetCollisionProfileName(TEXT("Pinecone"));
	RootComponent = SphereCollider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APinecone::BeginPlay()
{
	Super::BeginPlay();
	
}

void APinecone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear all the timer handle that belongs to this object
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void APinecone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APinecone::InitPineconeDetonation()
{
	FTimerHandle DetonationHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DetonationHandle, 
		this,
		&APinecone::OnDetonation, 
		DetonationTime);
}

void APinecone::OnDetonation()
{
	Receive_OnDetonation();
}

