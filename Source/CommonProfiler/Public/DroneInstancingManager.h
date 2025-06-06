#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "DroneInstancingManager.generated.h"

USTRUCT(BlueprintType)
struct FDroneInstanceData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 InstanceIndex;

    UPROPERTY()
    FVector InitialPosition;

    UPROPERTY()
    float ZStartOffset;

    UPROPERTY()
    FVector XYTargetOffset;

    UPROPERTY()
    TArray<int32> RotorIndices;  // 🟢 Stores rotor indices for this drone
};


UCLASS(Blueprintable)
class VVRARENA_API ADroneInstancingManager : public AActor
{
    GENERATED_BODY()

public:
    ADroneInstancingManager();

    UFUNCTION(BlueprintCallable, Category = "Drone Manager")
    void AddDrone(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Drone Manager")
    void RemoveDrone(int32 DroneIndex);

    UFUNCTION(BlueprintCallable, Category = "Drone Manager")
    void UpdateDroneMovement(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Drone Manager")
    void UpdateRotorRotation(float DeltaTime);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Settings")
    UInstancedStaticMeshComponent* DroneISM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Settings")
    UInstancedStaticMeshComponent* RotorISM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Settings")
    TArray<FVector> RotorOffsets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float ZSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float ZMaxOffset = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float XYSpeed = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float XYTargetUpdateFrequency = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float RotorRotationSpeed = 200.0f; // Degrees per second


private:
    UPROPERTY()
    TArray<FDroneInstanceData> Drones;

    FTimerHandle UpdateXYTimerHandle;

    void UpdateXYOffsets();
};
