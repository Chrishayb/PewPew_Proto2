// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pinecone.generated.h"

UCLASS()
class JUNGLEGYM_API APinecone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APinecone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pinecone")
	float DetonationTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pinecone")
	float BlastRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the object is about to get deleted
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to start pinecone detonation count down
	UFUNCTION(BlueprintCallable)
	void InitPineconeDetonation();

	// Called on pinecone detonation
	UFUNCTION()
	virtual void OnDetonation();
	UFUNCTION(BlueprintImplementableEvent, Category = "Pinecone", meta = (DisplayName = "On Detonation"))
	void Receive_OnDetonation();


};
