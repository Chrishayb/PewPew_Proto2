// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalDefenseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefenseGameplayDelegate);

namespace PortalDefenseState
{
	extern JUNGLEGYM_API const FName WaitingToStart;		// Player is in the map waiting to start the map
	extern JUNGLEGYM_API const FName AddsPhase;				// During normal phase
	extern JUNGLEGYM_API const FName BossPhase;				// Boss has spawned
	extern JUNGLEGYM_API const FName End;					// The game has ended
}

/**
 * This gamemode is for the lolz
 */
UCLASS()
class JUNGLEGYM_API APortalDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	APortalDefenseGameMode();

	// Delegate on switching to real world
	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToRealWorld;

	// Delegate on switching to imagine world
	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToImagineWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DefenseGamemode: Monitor")
	bool bInRealWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DefenseGamemode: WorldToggle")
	TSubclassOf<AActor> SkyBoxClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DefenseGamemode: WorldToggle")
	AActor* SkyBoxActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefenseGamemode: WorldToggle")
	TArray<class AEnemyBase*> EnemyContainer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DefenseGamemode: WorldToggle")
	class UPostProcessComponent* WorldTogglePostProcessing;

	// The bin in the world that needs to be deposit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DefenseGamemode: DefaultClasses")
	TSubclassOf<AActor> BinClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefenseGamemode: Monitor")
	TArray<AActor*> Bins;

	// The portal that teleports the player in between worlds as well as boss spawn mechanic
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DefenseGamemode: DefaultClasses")
	TSubclassOf<AActor> PortalClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefenseGamemode: Monitor")
	AActor* Portal;

	// Game State
	FName PortalDefenseState;

public:

	// World toggles
	UFUNCTION(BlueprintCallable, Category = "DefenseGamemode: WorldToggle")
	void ToggleWorld();

	void SwapToRealWorld();
	void SwapToImagineWorld();

	// Game State
		//Updates the match state and calls the appropriate transition functions
	virtual void SetMatchState(FName _newState);


	/** AActor Interface */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	/** End Interface */

	/** AGamemodeBase Interface */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	/** End Interface */


};
