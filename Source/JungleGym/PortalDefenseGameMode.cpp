// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalDefenseGameMode.h"

#include "Components/PostProcessComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

#include "Engine.h"

namespace PortalDefenseState
{
	const FName WaitingToStart = FName(TEXT("WaitingToStart"));
	const FName AddsPhase = FName(TEXT("AddsPhase"));
	const FName BossPhase = FName(TEXT("BossPhase"));
	const FName End = FName(TEXT("End"));
}

APortalDefenseGameMode::APortalDefenseGameMode()
{
	WorldTogglePostProcessing = CreateDefaultSubobject<UPostProcessComponent>(TEXT("WorldTogglePostProcessing"));
	WorldTogglePostProcessing->SetupAttachment(RootComponent);
}

void APortalDefenseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void APortalDefenseGameMode::StartPlay()
{
	Super::StartPlay();

	bInRealWorld = true;

	SetPortalDefenseState(PortalDefenseState::WaitingToStart);

}

void APortalDefenseGameMode::ToggleWorld()
{
	if (bInRealWorld)
	{
		SwapToImagineWorld();
	}
	else SwapToRealWorld();
}

void APortalDefenseGameMode::SwapToRealWorld()
{
	bInRealWorld = true;
	SkyBoxActor->SetActorHiddenInGame(true);
	WorldTogglePostProcessing->bEnabled = false;

	OnToggleToRealWorld.Broadcast();
}

void APortalDefenseGameMode::SwapToImagineWorld()
{
	bInRealWorld = false;
	SkyBoxActor->SetActorHiddenInGame(false);
	WorldTogglePostProcessing->bEnabled = true;

	OnToggleToImagineWorld.Broadcast();
}

void APortalDefenseGameMode::HandleWaitingToStartState()
{
	// Give the player some time b4 starting the game
	GetWorld()->GetTimerManager().SetTimer(WaitGameStartHandle, this, &APortalDefenseGameMode::StartAddsPhase, WaitingGameStartTime);
	
	Receive_HandleWaitingToStartState();
}

void APortalDefenseGameMode::HandleAddsPhaseState()
{

	Receive_HandleAddsPhaseState();
}

void APortalDefenseGameMode::HandleBossPhaseState()
{

	Receive_HandleBossPhaseState();
}

void APortalDefenseGameMode::HandleEndState()
{

	Receive_HandleEndState();
}

void APortalDefenseGameMode::StartAddsPhase()
{
	SetPortalDefenseState(PortalDefenseState::AddsPhase);
}

void APortalDefenseGameMode::SetPortalDefenseState(FName _newState)
{
	if (PortalDefenseState == _newState)
	{
		return;
	}

	PortalDefenseState = _newState;
	
	FString SwapGameStateMsg = 
		FString(TEXT("Swapped to game state: ") + PortalDefenseState.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, SwapGameStateMsg);

	OnPortalDefenseStateSet();
	Receive_OnPortalDefenseStateSet(_newState);
}

void APortalDefenseGameMode::OnPortalDefenseStateSet()
{
	if (PortalDefenseState == PortalDefenseState::WaitingToStart)
	{
		HandleWaitingToStartState();
	}
	else if (PortalDefenseState == PortalDefenseState::AddsPhase)
	{
		HandleAddsPhaseState();
	}
	else if (PortalDefenseState == PortalDefenseState::BossPhase)
	{
		HandleBossPhaseState();
	}
	else if (PortalDefenseState == PortalDefenseState::End)
	{
		void HandleEndState();
	}
}

void APortalDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	// World Toggle (Set to real world at beginning)
	SkyBoxActor = GetWorld()->SpawnActor<AActor>(SkyBoxClass, FTransform::Identity);
	SkyBoxActor->SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));
	SwapToRealWorld();

	// Get bin actor placed in the game
	UGameplayStatics::GetAllActorsOfClass(this, BinClass, Bins);
	UGameplayStatics::GetAllActorsOfClass(this, EnemySpawnerClass, EnemySpawners);

}

void APortalDefenseGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);







}
