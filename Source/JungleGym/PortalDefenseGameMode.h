// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalDefenseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefenseGameplayDelegate);

/**
 * This gamemode is for the lolz
 */
UCLASS()
class JUNGLEGYM_API APortalDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DefenseGameMode")
	bool bInRealWorld;

	// Delegate on switching to real world
	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToRealWorld;

	// Delegate on switching to imagine world
	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToImagineWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefenseGameMode")
	TArray<class AEnemyBase*> EnemyContainer;


public:

	UFUNCTION(BlueprintCallable, Category = "PortalDefenseGamemode")
	void ToggleWorld();

	void SwapToRealWorld();
	void SwapToImagineWorld();

	/** AActor Interface */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	/** End Interface */

	/** AGamemodeBase Interface */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	/** End Interface */


};
