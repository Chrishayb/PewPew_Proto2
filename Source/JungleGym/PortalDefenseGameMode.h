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




	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToRealWorld;

	UPROPERTY(BlueprintAssignable, Category = "PortalDefenseGameplay")
	FDefenseGameplayDelegate OnToggleToImagineWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DefenseGameMode")
	TArray<class AEnemyBase*> EnemyContainer;

	
public:

	UFUNCTION(BlueprintCallable)


	/** AActor Interface */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	/** End Interface */

	/** AGamemodeBase Interface */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	/** End Interface */


};
