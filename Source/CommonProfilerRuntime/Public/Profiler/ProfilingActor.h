// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfilingActor.generated.h"

UCLASS()
class AProfilingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProfilingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR

private:
	
	UFUNCTION(BlueprintCallable)
	UWorld* GetViewportWorld() const;

	UFUNCTION(BlueprintCallable)
	TArray <ULevelStreaming*> GetStreamingLevels() const;

	UFUNCTION(BlueprintCallable)
	FTransform GetLevelInstanceLevelTransform(AActor* actor) const;
#endif
};