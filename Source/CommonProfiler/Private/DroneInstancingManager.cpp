#include "VVRARENA/Actors/Lobby/Public/DroneInstancingManager.h"
#include "TimerManager.h"

ADroneInstancingManager::ADroneInstancingManager()
{
    PrimaryActorTick.bCanEverTick = true;

    DroneISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DroneISM"));
    RootComponent = DroneISM;

    RotorISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RotorISM"));
    RotorISM->SetupAttachment(RootComponent);

    RotorOffsets = {
        FVector(161, 227, 36),
        FVector(-161, 227, 36),
        FVector(161, -219, 36),
        FVector(-161, -219, 36)
    };
}

void ADroneInstancingManager::BeginPlay()
{
    Super::BeginPlay();

    // Ensure all drones have correct initial positions
    for (FDroneInstanceData& Drone : Drones)
    {
        if (DroneISM->IsValidInstance(Drone.InstanceIndex))
        {
            FTransform DroneTransform;
            if (DroneISM->GetInstanceTransform(Drone.InstanceIndex, DroneTransform, false))
            {
                // Store initial position in world coordinates
                Drone.InitialPosition = GetActorTransform().TransformPosition(DroneTransform.GetLocation());
            }
        }

        // 🟢 Immediately position rotors correctly before simulation starts
        for (int32 j = 0; j < Drone.RotorIndices.Num(); j++)
        {
            if (!RotorISM->IsValidInstance(Drone.RotorIndices[j]))
            {
                continue;
            }

            FVector RotorNewLocation = Drone.InitialPosition + RotorOffsets[j];
            FTransform RotorTransform(FRotator(0, 0, 0), RotorNewLocation);
            RotorISM->UpdateInstanceTransform(Drone.RotorIndices[j], RotorTransform, true, true);
        }
    }

    RotorISM->MarkRenderStateDirty(); // Ensure render update

    // Start the XY movement update timer
    GetWorldTimerManager().SetTimer(UpdateXYTimerHandle, this, &ADroneInstancingManager::UpdateXYOffsets, XYTargetUpdateFrequency, true);
}


void ADroneInstancingManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateDroneMovement(DeltaTime);
    UpdateRotorRotation(DeltaTime);
}

void ADroneInstancingManager::AddDrone(FVector Location)
{
    // Add drone instance
    FTransform DroneTransform(FRotator(0, 0, 0), Location);
    int32 DroneIndex = DroneISM->AddInstance(DroneTransform);

    // Store drone data
    FDroneInstanceData NewDrone;
    NewDrone.InstanceIndex = DroneIndex;
    NewDrone.InitialPosition = Location;
    NewDrone.ZStartOffset = FMath::RandRange(0.0f, 360.0f);
    NewDrone.XYTargetOffset = FVector::ZeroVector;

    // Store rotor indices and place them at correct positions
    TArray<int32> RotorIndices;
    for (const FVector& Offset : RotorOffsets)
    {
        FVector RotorLocation = Location + Offset; // Set rotor relative to drone
        FTransform RotorTransform(FRotator(0, 0, 0), RotorLocation);
        int32 RotorIndex = RotorISM->AddInstance(RotorTransform);
        RotorIndices.Add(RotorIndex);
    }

    // Save rotor indices inside drone struct
    NewDrone.RotorIndices = RotorIndices;
    Drones.Add(NewDrone);
}



void ADroneInstancingManager::RemoveDrone(int32 DroneIndex)
{
    if (DroneISM->GetInstanceCount() > DroneIndex)
    {
        // Find the drone data
        int32 DroneDataIndex = Drones.IndexOfByPredicate([DroneIndex](const FDroneInstanceData& Drone) {
            return Drone.InstanceIndex == DroneIndex;
        });

        if (DroneDataIndex != INDEX_NONE)
        {
            // Remove associated rotors
            for (int32 RotorIndex : Drones[DroneDataIndex].RotorIndices)
            {
                if (RotorISM->IsValidInstance(RotorIndex))
                {
                    RotorISM->RemoveInstance(RotorIndex);
                }
            }

            // Remove drone from list
            Drones.RemoveAt(DroneDataIndex);
        }

        // Remove drone instance
        DroneISM->RemoveInstance(DroneIndex);
    }
}


void ADroneInstancingManager::UpdateDroneMovement(float DeltaTime)
{
    for (int32 i = 0; i < Drones.Num(); i++)
    {
        FDroneInstanceData& Drone = Drones[i];

        if (!DroneISM->IsValidInstance(Drone.InstanceIndex))
        {
            continue;
        }

        FTransform DroneTransform;
        if (!DroneISM->GetInstanceTransform(Drone.InstanceIndex, DroneTransform, true))
        {
            continue;
        }

        // Get current world location
        FVector CurrentLocation = DroneTransform.GetLocation();

        // Apply vertical oscillation (Z movement)
        Drone.ZStartOffset += DeltaTime * ZSpeed;
        float ZOffset = FMath::Sin(Drone.ZStartOffset) * ZMaxOffset;

        // Smooth XY movement
        FVector TargetXY = Drone.InitialPosition + Drone.XYTargetOffset;
        FVector SmoothedXY = FMath::VInterpTo(CurrentLocation, TargetXY, DeltaTime, XYSpeed);

        // Update drone position
        FVector NewLocation = FVector(SmoothedXY.X, SmoothedXY.Y, Drone.InitialPosition.Z + ZOffset);

        // Apply new transform
        FTransform NewTransform = FTransform(DroneTransform.GetRotation(), NewLocation, DroneTransform.GetScale3D());
        DroneISM->UpdateInstanceTransform(Drone.InstanceIndex, NewTransform, true, true);

        // Update rotor positions without resetting rotation
        for (int32 j = 0; j < Drone.RotorIndices.Num(); j++)
        {
            if (!RotorISM->IsValidInstance(Drone.RotorIndices[j]))
            {
                continue;
            }

            // Get current rotor transform to preserve rotation
            FTransform RotorTransform;
            if (!RotorISM->GetInstanceTransform(Drone.RotorIndices[j], RotorTransform, true))
            {
                continue;
            }

            FVector RotorNewLocation = NewTransform.TransformPosition(RotorOffsets[j]);

            // Preserve existing rotation while updating position
            FTransform UpdatedRotorTransform = FTransform(RotorTransform.GetRotation(), RotorNewLocation, RotorTransform.GetScale3D());
            RotorISM->UpdateInstanceTransform(Drone.RotorIndices[j], UpdatedRotorTransform, true, true);
        }
    }

    DroneISM->MarkRenderStateDirty();
    RotorISM->MarkRenderStateDirty();
}



void ADroneInstancingManager::UpdateRotorRotation(float DeltaTime)
{
    if (Drones.Num() == 0 || !RotorISM)
    {
        return; // No drones or rotor system not initialized
    }

    for (FDroneInstanceData& Drone : Drones)
    {
        if (Drone.RotorIndices.Num() == 0)
        {
            continue; // Skip drones without rotors
        }

        for (int32 j = 0; j < Drone.RotorIndices.Num(); j++)
        {
            if (!RotorISM->IsValidInstance(Drone.RotorIndices[j]))
            {
                continue; // Skip invalid rotors
            }

            // Get current rotor transform
            FTransform RotorTransform;
            if (!RotorISM->GetInstanceTransform(Drone.RotorIndices[j], RotorTransform, true))
            {
                continue;
            }

            // Apply rotation - попробуем Yaw вместо Roll
            FRotator NewRotation = RotorTransform.GetRotation().Rotator();
            NewRotation.Yaw += RotorRotationSpeed * DeltaTime; // Try Yaw if Roll doesn't work

            // Apply new rotation
            FTransform UpdatedRotorTransform = FTransform(FQuat(NewRotation), RotorTransform.GetLocation(), RotorTransform.GetScale3D());
            RotorISM->UpdateInstanceTransform(Drone.RotorIndices[j], UpdatedRotorTransform, true, true);
        }
    }

    RotorISM->MarkRenderStateDirty(); // Ensure rendering updates
}






void ADroneInstancingManager::UpdateXYOffsets()
{
    for (FDroneInstanceData& Drone : Drones)
    {
        float XOffset = FMath::RandRange(-50.0f, 50.0f);
        float YOffset = FMath::RandRange(-50.0f, 50.0f);

        // Set new target offset
        Drone.XYTargetOffset = FVector(XOffset, YOffset, 0.0f);
    }
}

