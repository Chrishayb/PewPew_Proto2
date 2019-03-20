// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalDefenseGameMode.h"

void APortalDefenseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void APortalDefenseGameMode::StartPlay()
{
	Super::StartPlay();

	bInRealWorld = true;
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
	OnToggleToRealWorld.Broadcast();
}

void APortalDefenseGameMode::SwapToImagineWorld()
{
	bInRealWorld = false;
	OnToggleToImagineWorld.Broadcast();
}

void APortalDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	SwapToRealWorld();
}

void APortalDefenseGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);







}
